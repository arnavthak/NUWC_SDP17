#include <QtTest/QtTest>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQuickItem>
#include <qqmlcontext.h>
#include "yamlprocessor.h"
#include "serialcomms.h"

class TestQmlUi : public QObject {
    Q_OBJECT

private slots:
    void initTestCase() {
        // Load the QML file

        QQmlApplicationEngine engine;

        YamlProcessor yamlProcessor;
        engine.rootContext()->setContextProperty("yamlProcessor", &yamlProcessor);

        SerialComms comms;
        engine.rootContext()->setContextProperty("serialComms", &comms);

        engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));
        QVERIFY2(!engine.rootObjects().isEmpty(), "QML root object not loaded");

        window = qobject_cast<QQuickWindow*>(engine.rootObjects().first());
        QVERIFY2(window, "Root object is not a QQuickWindow");
    }

    void testFileSelectorButtonClick() {
        // verify the header and then the button
        QObject *header = window->property("header").value<QObject*>();
        QVERIFY2(header, "Header not found");

        QList<QObject*> children = header->children();

        // Optional: print them for debugging
        for (auto *child : children) {
            qDebug() << "Child:" << child->objectName()
            << "Type:" << child->metaObject()->className();
        }

        QQuickItem *button = header->findChild<QQuickItem*>("fileSelector");
        QVERIFY2(button, "Button not found inside header");

        // simulate a mouse click on the button
        QPoint clickPoint = button->mapToScene(QPointF(button->width()/2, button->height()/2)).toPoint();
        QTest::mouseClick(window, Qt::LeftButton, Qt::NoModifier, clickPoint);

        // berify that the filedialog opened
        QQuickItem *fileDialog = window->findChild<QQuickItem*>("fileDialog");
        QVERIFY2(fileDialog, "FileDialog not found");

        bool visible = fileDialog->property("visible").toBool();
        QVERIFY2(visible, "FileDialog did not open");

        // test readChipConfiguration:
        YamlProcessor processor;

        ChipConfiguration config = processor.readChipConfiguration(QUrl(QStringLiteral("qrc:/74ls00.yaml")));

        // check basic information
        QCOMPARE(config.chipInfo["Chip Number"], QString("00"));
        QCOMPARE(config.chipInfo["Description"], QString("Quad 2-input NAND gate"));
        QCOMPARE(config.chipInfo["Logic Type"], QString("NAND"));
        QCOMPARE(config.chipInfo["Number of Inputs"], QString("2"));
        QCOMPARE(config.chipInfo["Pin Count"], QString("14"));

        // check pin names
        QCOMPARE(config.pinNames["A"], QList<QString>({"1","4","9","12"}));
        QCOMPARE(config.pinNames["B"], QList<QString>({"2","5","10","13"}));
        QCOMPARE(config.pinNames["Y"], QList<QString>({"3","6","8","11"}));

        // check pin configs
        QCOMPARE(config.pinConfigs["G"], QList<QString>({"7"}));
        QCOMPARE(config.pinConfigs["I"], QList<QString>({"1","2","4","5","9","10","12","13"}));
        QCOMPARE(config.pinConfigs["O"], QList<QString>({"3","6","8","11"}));
        QCOMPARE(config.pinConfigs["V"], QList<QString>({"14"}));

        //test serialcomms.cpp
        SerialComms serialComms;

        serialComms.linkTest("COM10", "COM11"); // set your own port names here

        // allow some time for onReceiverReadyRead to fire
        QTest::qWait(500);

        QVERIFY(serialComms.verificationSucceeded());
    }

private:
    QQmlApplicationEngine engine;
    QQuickWindow *window = nullptr;
};

QTEST_MAIN(TestQmlUi)
#include "test_qmlui.moc"
