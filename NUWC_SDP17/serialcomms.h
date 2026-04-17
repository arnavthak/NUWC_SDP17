#ifndef SERIALCOMMS_H
#define SERIALCOMMS_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
#include <QByteArray>

class SerialComms : public QObject
{
    Q_OBJECT

public:
    explicit SerialComms(QObject *parent = nullptr); // constructor
    ~SerialComms(); // destructor
    void sendByteStream(QByteArray byteStream, bool useCRC); // write to packets
    QString readMCU(bool useCRC = false, bool testArduino = true); // read MCU
    uint16_t calculateCRC(const QByteArray &data); // calculate checksum value for padding
    bool verifyCRC(const QByteArray &data); // verify checksum
    void listAvailablePorts(); // debug for port connections
    Q_INVOKABLE QString sendTestStream(QString stream); // debug method for testing
    // Q_INVOKABLE void executeTestSequence(const QVariantList &testSteps); potentially obsolete
    Q_INVOKABLE bool isMCUConnected(); // check MCU

signals:
    void dataReceived(QString message);

private:
    QSerialPort *serialPort = nullptr;
    QByteArray msgBuffer;
};

#endif // SERIALCOMMS_H
