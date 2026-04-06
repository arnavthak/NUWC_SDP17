// written by Ben

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>
#include <QDebug>

#include "pdfreportgenerator.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    PdfReportGenerator pdfGenerator;
    engine.rootContext()->setContextProperty("pdfGenerator", &pdfGenerator);

    // Go two levels up from the build folder
    QString qmlPath = QDir::cleanPath(
        QCoreApplication::applicationDirPath() + "/../../main.qml"
        );

    qDebug() << "Loading QML from:" << qmlPath;

    engine.load(QUrl::fromLocalFile(qmlPath));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
