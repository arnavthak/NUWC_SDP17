#include "serialcomms.h"
#include <QDebug>
#include <QCoreApplication>

SerialComms::SerialComms(QObject *parent) : QObject(parent), m_serialPort(new QSerialPort(this)) {
    // connect readyRead signal to onReadyRead slot
    connect(m_serialPort, &QSerialPort::readyRead, this, &SerialComms::onReadyRead);
}

SerialComms::~SerialComms()
{
    // if you see a door open, close it, because that's how doors work.
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
    }
}

void SerialComms::linkTest(const QString &portName)
{
    m_serialPort->setPortName(portName);

    // configure port
    m_serialPort->setBaudRate(QSerialPort::Baud115200);         // 115200 baud
    m_serialPort->setDataBits(QSerialPort::Data8);              // 8 data bits
    m_serialPort->setParity(QSerialPort::NoParity);             // no parity
    m_serialPort->setStopBits(QSerialPort::OneStop);            // 1 stop bit
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);   // no flow control

    if (m_serialPort->open(QIODevice::ReadWrite)) {
        qDebug() << "Serial port opened:" << portName;

        // send data
        m_dataSent = "CSE/ECESeniorDesign";
        qint64 bytesWritten = m_serialPort->write(m_dataSent);

        if (bytesWritten == -1) {
            qWarning() << "Failed to write data:" << m_serialPort->errorString();
        } else {
            qDebug() << "Data sent:" << m_dataSent;
        }

    } else {
        qWarning() << "Failed to open serial port:" << m_serialPort->errorString();
        QCoreApplication::quit(); // Quit if we can't open the port
    }

}

void SerialComms::onReadyRead()
{
    // receive data
    m_dataReceived.append(m_serialPort->readAll());

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

        m_serialPort->close();
        qDebug() << "Port closed.";
        QCoreApplication::quit();
    }
}
