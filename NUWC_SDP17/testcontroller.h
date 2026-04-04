#ifndef TESTCONTROLLER_H
#define TESTCONTROLLER_H

#include <QObject>
#include <yamlprocessor.h>
#include <serialcomms.h>
#include <QTimer>
#include <QElapsedTimer>

class TestController : public QObject
{
    Q_OBJECT

public:
    explicit TestController(YamlProcessor* yamlProcessor,
                            SerialComms* serialComms,
                            QObject* parent = nullptr);
    //Q_INVOKABLE QVariantMap runTests(const QUrl& filePath, bool isSimulation);
    Q_INVOKABLE void startTests(const QUrl &filePath, bool isSimulation);

    Q_INVOKABLE void pauseTests();
    Q_INVOKABLE void resumeTests();
    Q_INVOKABLE void stopTests();

private:
    void sendChipConfiguration(const ChipConfiguration& config);
    void resolveSequentialOutputs(
        const QList<QPair<QString, QList<QString>>>& test,
        QList<QString>& expectedOutputs,
        int pinCount);
    void sendTest(const QList<QPair<QString, QList<QString>>>& test, int pinCount);
    QByteArray createExpectedBytestream(QList<QString>& expectedOutputs);
    void runTestsStepwise(const QUrl &filePath, bool isSimulation);

    QVariantMap results;
    int currentTestIndex;
    Tests currentTests;
    ChipConfiguration currentConfig;
    QUrl currentFile;
    bool currentSimulation;

    bool isRunning = false;
    bool isPaused = false;
    bool isStopped = true;

    YamlProcessor* yamlProcessor;
    SerialComms* serialComms;

    QElapsedTimer testTimer;

signals:
    void logMessage(QString text, QString color);
    void resultsReady(QVariantMap results);
    void clearConsole();
};

#endif // TESTCONTROLLER_H
