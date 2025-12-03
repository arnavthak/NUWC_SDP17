//written by Ben

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDir>
#include <QDebug>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

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
