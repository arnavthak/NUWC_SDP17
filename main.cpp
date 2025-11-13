#include <QCoreApplication>
#include <QString>
#include <QDebug>
#include "serialcomms.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setApplicationName("Qt link test");

    const QStringList args = QCoreApplication::arguments();
    if (args.size() < 2) {
        qWarning << "Error: You must provide the serial port name as argument.";
        return -1;
    }

    QString portName = args.at(1);

    serialComms comms;

    return a.exec();
}
