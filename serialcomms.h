#ifndef SERIALCOMMS_H
#define SERIALCOMMS_H

#include <QObject>
#include <QSerialPort>
#include <QByteArray>

class  : public QObject
{
    Q_OBJECT

public:
    explicit SerialComms(QObject *parent = nullptr);
    ~SerialComms();

private slots:
    void onReadyRead();
    void linkTest();

private:
    QSerialPort *m_serialPort;
    QByteArray m_dataSent;
    QByteArray m_dataReceived;
};

#endif // SERIALCOMMS_H
