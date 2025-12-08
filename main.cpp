//written by Abhinav
//reviewed by Ben

#include <QCoreApplication>
#include <QSerialPort>
#include <QString>
#include <QDebug>
#include "serialcomms.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setApplicationName("Qt link test");

    /* Removed for testing/debug purposes. Upon return, please adjust for specific argument quantities.
    const QStringList args = QCoreApplication::arguments();
    if (args.size() < 2) {
        qWarning() << "Error: You must provide the serial port name as argument.";
        return -1;
    }*/

    QString sender = "/dev/ttys004";    // please adjust names for arguments in the future.. these ones are placeholders.
    QString recv = "/dev/ttys005";      // ^^^

    SerialComms comms;
    comms.linkTest(sender,recv);

    return a.exec();
}
