// Code Author: Arnav Thakrar

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "yamlprocessor.h"
#include "serialcomms.h"
#include "testcontroller.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    YamlProcessor yamlProcessor;
    engine.rootContext()->setContextProperty("yamlProcessor", &yamlProcessor);

    SerialComms comms;
    comms.listAvailablePorts();
    engine.rootContext()->setContextProperty("serialComms", &comms);

    TestController testController(&yamlProcessor, &comms);
    engine.rootContext()->setContextProperty("testController", &testController);

    QObject::connect(
        &yamlProcessor,
        &YamlProcessor::yamlLoaded,
        &yamlProcessor,
        &YamlProcessor::readChipConfiguration
    );


    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("NUWC_SDP17", "Main");

    return app.exec();
}
