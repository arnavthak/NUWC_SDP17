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
    //Q_INVOKABLE void linkTest(const QString &senderPortName, const QString &recvPortName); // can delete later.
    ~SerialComms();
    void sendByteStream(QByteArray byteStream, bool useCRC);
    QString readMCU(bool useCRC = false, bool testArduino = true);
    uint16_t calculateCRC(const QByteArray &data);
    bool verifyCRC(const QByteArray &data);
    void sendSelectedFile();
    void listAvailablePorts();
    Q_INVOKABLE QString sendTestStream(QString stream);
    Q_INVOKABLE void executeTestSequence(const QVariantList &testSteps);
    bool isMCUConnected();


private slots:
    //void onReceiverReadyRead(); // can delete later.

signals:
    void dataReceived(QString message);

private:
    QSerialPort *serialPort = nullptr;
    QByteArray msgBuffer;
};

#endif // SERIALCOMMS_H
