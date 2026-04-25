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


SerialComms::~SerialComms() // destructor class
{
    // Ensure serial port is properly closed before destruction
    if (serialPort && serialPort->isOpen()) {
        serialPort->close();
    }
}

SerialComms::SerialComms(QObject *parent) : QObject{parent} {

    QString foundPortName = ""; // Stores detected port name

    // Search for available serial ports matching known VID/PID
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        if (info.hasVendorIdentifier() && info.hasProductIdentifier()) {

            // Currently supports Arduino (R3) and STM32 based on VID/PID
            // Also filters for "cu" ports (macOS-style callout devices)
            if ((info.vendorIdentifier() == 9025 && info.productIdentifier() == 67) ||
                (info.vendorIdentifier() == 4292 && info.productIdentifier() == 60000 &&
                 info.portName().contains("cu"))) {

                foundPortName = info.portName();
                break;
            }
        }
    }

    // If no matching hardware found, abort initialization
    if (foundPortName.isEmpty()) {
        qWarning() << "Hardware was not found";
        return;
    } else {
        // Initialize serial port with standard settings
        serialPort = new QSerialPort(this);
        serialPort->setPortName(foundPortName);
        serialPort->setBaudRate(QSerialPort::Baud9600);
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setParity(QSerialPort::NoParity);
        serialPort->setStopBits(QSerialPort::OneStop);
        serialPort->setFlowControl(QSerialPort::NoFlowControl);
    }

    // Attempt to open serial port for read/write
    if (serialPort->open(QIODevice::ReadWrite)){
        qDebug() << "--------------------------------------------";
        qDebug() << "Port opened successfully. Waiting to boot...";
        qDebug() << "Connected on: " << foundPortName;
        qDebug() << "--------------------------------------------";
    } else {
        qWarning() << "Failed to open port:" << serialPort->errorString();
        QCoreApplication::quit();
    }

    // Optional: connect readyRead signal to automatic read handler
    // connect(serialPort, &QSerialPort::readyRead, this, [this](){readMCU();});
}

void SerialComms::sendByteStream(QByteArray byteStream, bool useCRC)
{
    QByteArray packet = byteStream;

    // Optionally append CRC checksum for data integrity
    if (useCRC){
        uint16_t crc = calculateCRC(byteStream);
        packet.append(static_cast<char>((crc >> 8) & 0xff)); // high byte
        packet.append(static_cast<char>(crc & 0xFF));         // low byte
    }

    // Write packet to serial port
    serialPort->write(packet);

    qDebug() << "Data written." << packet;

    // Wait until bytes are physically written to device
    serialPort->waitForBytesWritten(5000);
    serialPort->flush();

    // Small delay to allow MCU processing time
    QThread::msleep(100);
}

// V3 multi-line implementation that works with PWRALL
QString SerialComms::readMCU(bool useCRC, bool testArduino)
{
    Q_UNUSED(useCRC); // CRC currently not used in this implementation
    Q_UNUSED(testArduino);

    qDebug() << "Waiting for MCU response...";

    QByteArray response;
    QElapsedTimer totalTimer;
    totalTimer.start();

    const int totalTimeoutMs = 10000;     // Max total wait time
    const int interByteTimeoutMs = 1000;  // Timeout between incoming chunks

    // Continuously read chunks until timeout or transmission ends
    while (totalTimer.elapsed() < totalTimeoutMs) {

        // Wait for incoming data
        if (serialPort->waitForReadyRead(interByteTimeoutMs)) {
            QByteArray chunk = serialPort->readAll();
            response.append(chunk);

            qDebug() << "Chunk received:" << chunk;
        } else {
            // If no new data arrives within interByteTimeout,
            // assume transmission is complete
            if (!response.isEmpty()) {
                break;
            }
        }
    }

    // No data received
    if (response.isEmpty()) {
        qDebug() << "No response received from MCU";
        return "";
    }

    // Split response into newline-delimited messages
    QList<QByteArray> lines = response.split('\n');
    QStringList parsedLines;

    for (QByteArray &line : lines) {
        line = line.trimmed();

        if (!line.isEmpty()) {
            qDebug() << "Parsed Line:" << line;

            // Emit signal for each parsed message
            emit dataReceived(QString::fromUtf8(line));

            parsedLines.append(QString::fromUtf8(line));
        }
    }

    // Return combined response (joined with newline)
    return parsedLines.join('\n');
}

uint16_t SerialComms::calculateCRC(const QByteArray &data){
    // Computes CRC-16 (polynomial 0x1021) for error detection

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
    // Packet must contain at least payload + 2 CRC bytes
    if (packet.size() < 3) {
        return false;
    }

    // Separate payload and CRC
    QByteArray payload = packet.left(packet.size() - 2);
    QByteArray receivedCRCBytes = packet.right(2);

    // Compute expected CRC
    uint16_t calculatedCRC = calculateCRC(payload);

    // Reconstruct received CRC value (big-endian)
    uint8_t high = static_cast<uint8_t>(receivedCRCBytes[0]);
    uint8_t low = static_cast<uint8_t>(receivedCRCBytes[1]);
    uint16_t receivedCRC = (high << 8) | low;

    // Compare expected vs received
    return calculatedCRC == receivedCRC;
}

// Lists all available serial ports (debugging utility)
void SerialComms::listAvailablePorts(){
    const auto serialPortInfos = QSerialPortInfo::availablePorts();

    qDebug() << "Total ports found:" << serialPortInfos.count();

    for (const QSerialPortInfo &portInfo : serialPortInfos) {
        qDebug() << "---------------------------------------------";
        qDebug() << "Port Name: " << portInfo.portName();
        qDebug() << "Descripton: " << portInfo.description();
        qDebug() << "Manufacturer: " << portInfo.manufacturer();
        qDebug() << "Vendor ID: " << portInfo.vendorIdentifier();
        qDebug() << "Product ID: " << portInfo.productIdentifier();
    }
}

// Sends a test string and returns MCU response (debug helper)
QString SerialComms::sendTestStream(QString stream){
    serialPort->write((stream + "\r\n").toUtf8());

    qDebug() << "Message written: " << stream;

    // Reuse readMCU to fetch response
    QString result = readMCU(false, false);
    return result;
}

// Checks whether MCU connection has been initialized
bool SerialComms::isMCUConnected() {
    if (serialPort == nullptr)
        return false;

    return true;
}