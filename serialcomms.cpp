//written by Abhinav
//reviewed by Ben

#include "serialcomms.h"
#include <QDebug>
#include <QSerialPort>
#include <QCoreApplication>



SerialComms::SerialComms(QObject *parent) : QObject(parent)
{
    // create send and receive
    m_sendPort = new QSerialPort(this);
    m_recvPort = new QSerialPort(this);

    // connect readyRead signal to onReadyRead slot
    connect(m_recvPort, &QSerialPort::readyRead, this, &SerialComms::onReceiverReadyRead);
}

SerialComms::~SerialComms()
{
    // if you see a door open, close it, because that's how doors work.
    if (m_sendPort->isOpen()) {
        m_sendPort->close();
    }
    if (m_recvPort->isOpen()) {
        m_recvPort->close();
    }
}

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

}

void SerialComms::onReceiverReadyRead()
{
    // receive data
    m_dataReceived.append(m_recvPort->readAll());

    if (m_dataReceived.size() >= m_dataSent.size()) {
        qDebug() << "Data Received:" << m_dataReceived;

        // verify data
        if (m_dataReceived == m_dataSent) {
            qInfo() << "Verification success: data matches.";
        } else {
            qWarning() << "Verification failed: unexpected result.";
            qWarning() << "Expected:" << m_dataSent;
            qWarning() << "Received:" << m_dataReceived;
        }

        m_recvPort->close();
        qDebug() << "Port closed.";
        QCoreApplication::quit();
    }
}
