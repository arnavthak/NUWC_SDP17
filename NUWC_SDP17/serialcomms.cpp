#include "serialcomms.h"
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QCoreApplication>
#include <QByteArray>
#include <QStringList>
#include <QThread>
#include <QObject>
#include <QTimer>
#include <QMap>
#include <QtCore/qvariant.h>

/* COMMENTED OUT FOR PROPER IMPLEMENTATION
 *
 * SerialComms::SerialComms(QObject *parent)
    : QObject{parent}
{
    m_sendPort = new QSerialPort(this);
    m_recvPort = new QSerialPort(this);

    connect(m_recvPort, &QSerialPort::readyRead, this, &SerialComms::onReceiverReadyRead);
}
*/


SerialComms::~SerialComms() // destructor
{
    if (serialPort->isOpen()) {
        serialPort->close();
    }
}

SerialComms::SerialComms(QObject *parent) : QObject{parent} {

    QString foundPortName = ""; // init portname


    // search for available ports
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        if (info.hasVendorIdentifier() && info.hasProductIdentifier()) {
            // better to use CU or callup for immediate connection.
            // currently hardcoded to an arduino, once VID and PID for microcontroller is determined it will be swapped
            // Arduino R3 Ref: {VID, PID} = {9025, 67}
            if ((info.vendorIdentifier() == 9025 && info.productIdentifier() == 67) || ( info.vendorIdentifier() == 4292 && info.productIdentifier() == 60000) && info.portName().contains("cu")){
                foundPortName = info.portName();
                break;
            }
        }
    }

    // port initialization -- assuming hardcoded with true driver when complete
    // settings: 9600 baud rate, 8 data bits, no parity, one stop bit, and no flow control.
    if (foundPortName.isEmpty()) {
        qWarning() << "Hardware was not found";
        //QCoreApplication::quit();
    } else {
        serialPort = new QSerialPort(this);
        serialPort->setPortName(foundPortName);
        serialPort->setBaudRate(QSerialPort::Baud115200);
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setParity(QSerialPort::NoParity);
        serialPort->setStopBits(QSerialPort::OneStop);
        serialPort->setFlowControl(QSerialPort::NoFlowControl);
    }

    // port connection
    if (serialPort->open(QIODevice::ReadWrite)){
        qDebug() << "--------------------------------------------";
        qDebug() << "Port opened successfully. Waiting to boot...";
        QTimer::singleShot(8000, this, [=](){sendTestStream("Testing... ");}); // Serial port message test.
        qDebug() << "Connected on: " << foundPortName;
        qDebug() << "--------------------------------------------";
    } else {
        qWarning() << "Failed to open port:" << serialPort->errorString();
        QCoreApplication::quit();
    }


    connect(serialPort, &QSerialPort::readyRead, this, [this](){readMCU();});
}

void SerialComms::sendByteStream(QByteArray byteStream, bool useCRC) // currently waiting on proper UI implement
{
    QByteArray packet = byteStream;

    if (useCRC){        // if selected, adds cyclic redundancy checksums for data loss
        uint16_t crc = calculateCRC(byteStream);
        packet.append(static_cast<char>((crc >> 8) & 0xff));
        packet.append(static_cast<char>(crc & 0xFF));
    }

    serialPort->write(packet);   // writes to serial port, no flushing required.

    if (serialPort->waitForBytesWritten(1000)){
        qDebug() << "Data written." << packet;
    }

    QThread::msleep(100);

}

QString SerialComms::readMCU(bool useCRC, bool testArduino)
{
    // arduino test reading output to Application Output
    if (testArduino && msgBuffer.contains('\n')) {
        int lnEndIdx = msgBuffer.indexOf('\n');
        while (lnEndIdx != -1) {
            QByteArray line = msgBuffer.left(lnEndIdx);
            msgBuffer.remove(0, lnEndIdx + 1);
            qDebug() << "Parsed Line: " << line.trimmed();
            lnEndIdx = msgBuffer.indexOf('\n');
        }
    }

    if (serialPort->waitForReadyRead(1000)){     // wait 2 seconds for data
        if (serialPort->canReadLine()){          // check for full line
            QByteArray raw = serialPort->readLine();

            // verify CRC and strip
            if (useCRC){
                if (raw.length() < 3){
                    return "";
                }

                QByteArray payload = raw.left(raw.length() - 2);

                if (!verifyCRC(raw)) {
                    qDebug() << "CRC Mismatch";
                    return "";
                }

                raw = payload;
            }
            // return raw payload for processing / translation

            QString message = QString::fromUtf8(raw).trimmed();
            emit dataReceived(message);
            qDebug() << message;
            return message;
        }
    }

    // timeout / no data
    return "";
}

uint16_t SerialComms::calculateCRC(const QByteArray &data){
    // adds cyclic redundancy checksums to data, allows for integrity checks for data loss or corruption

    uint16_t crc = 0x0000;

    for (char byte : data) {
        crc ^= (static_cast<uint8_t>(byte) << 8);
        for (int i = 0; i < 8; ++i){
            if (crc & 0x8000){
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc = (crc << 1);
            }

        }
    }

    return crc;
}

bool SerialComms::verifyCRC(const QByteArray &packet){
    if (packet.size() < 3) {
        return false;       // check length
    }

    QByteArray payload = packet.left(packet.size() - 2);    // seperate payload

    QByteArray receivedCRCBytes = packet.right(2);      // seperate received CRC

    uint16_t calculatedCRC = calculateCRC(payload);     // calculate what CRC should be

    // convert received bytes back into a number
    uint8_t high = static_cast<uint8_t>(receivedCRCBytes[0]);
    uint8_t low = static_cast<uint8_t>(receivedCRCBytes[1]);

    uint16_t receivedCRC = (high <<8) | low;

    return calculatedCRC == receivedCRC;        // compare results, if true then no errors
}

// lists available ports to application output
void SerialComms::listAvailablePorts(){
    // searches device for ports
    const auto serialPortInfos = QSerialPortInfo::availablePorts();

    qDebug() << "Total ports found:" << serialPortInfos.count();

    // parse port info and output
    for (const QSerialPortInfo &portInfo : serialPortInfos) {
        qDebug() << "---------------------------------------------";
        qDebug() << "Port Name: " << portInfo.portName(); // port name
        qDebug() << "Descripton: " << portInfo.description(); // port desc
        qDebug() << "Manufacturer: " << portInfo.manufacturer(); // device manufacturer
        qDebug() << "Vendor ID: " << portInfo.vendorIdentifier(); // VID
        qDebug() << "Product ID: " << portInfo.productIdentifier(); // PID
    }
}

void SerialComms::sendSelectedFile(){
}


/* COMMENTED OUT FOR PROPER IMPLEMENTATION
 * Now obsolete. Can be deleted later. -EP :)
 *
 *
// linkage tests
void SerialComms::linkTest(const QString &senderPortName, const QString &recvPortName)
{

    // configure send port
    m_sendPort->setPortName(senderPortName);
    m_sendPort->setBaudRate(QSerialPort::Baud115200);         // 115200 baud
    m_sendPort->setDataBits(QSerialPort::Data8);              // 8 data bits
    m_sendPort->setParity(QSerialPort::NoParity);             // no parity
    m_sendPort->setStopBits(QSerialPort::OneStop);            // 1 stop bit
    m_sendPort->setFlowControl(QSerialPort::NoFlowControl);   // no flow control

    // configure receive port
    m_recvPort->setPortName(recvPortName);
    m_recvPort->setBaudRate(QSerialPort::Baud115200);         // 115200 baud
    m_recvPort->setDataBits(QSerialPort::Data8);              // 8 data bits
    m_recvPort->setParity(QSerialPort::NoParity);             // no parity
    m_recvPort->setStopBits(QSerialPort::OneStop);            // 1 stop bit
    m_recvPort->setFlowControl(QSerialPort::NoFlowControl);   // no flow control

    bool sendOpen = m_sendPort->open(QIODevice::WriteOnly);
    bool recvOpen = m_recvPort->open(QIODevice::ReadOnly);

    if (sendOpen && recvOpen) {
        qDebug() << "Ports opened successfully.:";
        qDebug() << "Sender:" << senderPortName << "Receiver:" << recvPortName;

        // send data
        m_dataSent = "CSE/ECESeniorDesign";
        m_sendPort->write(m_dataSent);
        qDebug() << "Data sent:" << m_dataSent;
    } else {
        qWarning() << "Failed to open ports:";
        if (!sendOpen) qWarning() << "Send Error:" << m_sendPort->errorString();
        if (!recvOpen) qWarning() << "Receive Error:" << m_recvPort->errorString();
        QCoreApplication::quit(); // Quit if we can't open the port
    }

} */

// linkage test read
/*** Commented out cause obsolete. Can delete later.
void SerialComms::onReceiverReadyRead()
{
    // receive data
    m_dataReceived.append(serialPort->readAll());

    if (m_dataReceived.size() >= m_dataSent.size()) {
        qDebug() << "Data Received:" << m_dataReceived;

        serialPort->close();
        qDebug() << "Port closed.";
        QCoreApplication::quit();
    }
}*/

// test serial connection with basic byte stream
void SerialComms::sendTestStream(QString stream){
    serialPort->write((stream + "\n").toUtf8());
    qDebug() << "Message written: " << stream;

    readMCU(false, true);
}


void SerialComms::executeTestSequence(const QVariantList &testSteps) {

    qDebug() << "Starting Test Sequence...";


    // iterate through test instruction steps
    for (const QVariant &stepVariant : testSteps) {
        QMap<QString, QVariant> stepMap = stepVariant.toMap();

        // each step instruction has a key and a list of pin values
        QMapIterator<QString, QVariant> i(stepMap);
        while (i.hasNext()){
            i.next();
            QString instructionHex = i.key();
            QVariantList pins = i.value().toList();

            bool ok;
            // convert "0x0F" string into integer 15
            uint8_t cmd = static_cast<uint8_t>(instructionHex.toUInt(&ok, 16));

            for (const QVariant &pinVar : pins) {
                QString pinHex = pinVar.toString();
                // convert "0x01" string into integer 1
                uint8_t pin = static_cast<uint8_t>(pinHex.toUInt(&ok, 16));

                // create a 2-byte packet: [CMD, PIN]
                QByteArray packet;
                packet.append(static_cast<char>(cmd));
                packet.append(static_cast<char>(pin));

                // send through sendByteStream with CRC
                sendByteStream(packet, true);
            }
        }
    // allow for MCU to catch up
    QThread::msleep(50);
    }
}
