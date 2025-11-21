#ifndef SERIALCOMMS_H
#define SERIALCOMMS_H

#include <QObject>
#include <QSerialPort>
#include <QByteArray>

class SerialComms : public QObject
{
    Q_OBJECT

public:
    explicit SerialComms(QObject *parent = nullptr);
    Q_INVOKABLE void linkTest(const QString &senderPortName, const QString &recvPortName);
    ~SerialComms();

private slots:
    void onReceiverReadyRead();

private:
    QSerialPort *m_sendPort;
    QSerialPort *m_recvPort;
    QByteArray m_dataSent;
    QByteArray m_dataReceived;
};

#endif // SERIALCOMMS_H
