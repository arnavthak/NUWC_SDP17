#include <QtTest/QtTest>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQuickItem>

class TestQmlUi : public QObject {
    Q_OBJECT

private slots:
    void initTestCase() {
        // Load the QML file
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
    }

private:
    QQmlApplicationEngine engine;
    QQuickWindow *window = nullptr;
};

QTEST_MAIN(TestQmlUi)
#include "test_qmlui.moc"
