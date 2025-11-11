#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "yamlprocessor.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    YamlProcessor yamlProcessor;
    engine.rootContext()->setContextProperty("yamlProcessor", &yamlProcessor);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("NUWC_SDP17", "Main");

    return app.exec();
}
