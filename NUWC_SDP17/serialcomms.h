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
    explicit SerialComms(QObject *parent = nullptr);
    Q_INVOKABLE void linkTest(const QString &senderPortName, const QString &recvPortName);
    ~SerialComms();
    void sendByteStream(QByteArray byteStream, QSerialPort serialPort, bool useCRC);
    QString readMCU(QSerialPort serialPort, bool useCRC);
    uint16_t calculateCRC(const QByteArray &data);
    bool verifyCRC(const QByteArray &data);
    void sendSelectedFile();


private slots:
    void onReceiverReadyRead();

private:
    QSerialPort *serialPort;

    //linktest only
    QSerialPort *m_sendPort;
    QSerialPort *m_recvPort;
    QByteArray m_dataSent;
    QByteArray m_dataReceived;
};

#endif // SERIALCOMMS_H
