#ifndef TESTCONTROLLER_H
#define TESTCONTROLLER_H

#include <QObject>
#include <yamlprocessor.h>
#include <serialcomms.h>

class TestController : public QObject
{
    Q_OBJECT

public:
    explicit TestController(YamlProcessor* yamlProcessor,
                            SerialComms* serialComms,
                            QObject* parent = nullptr);

private:
    void sendChipConfiguration(const ChipConfiguration& config);
    void resolveSequentialOutputs(
        const QMap<QString, QList<QString>>& test,
        QList<QString>& expectedOutputs,
        int pinCount);
    void sendTest(const QMap<QString, QList<QString>>& test, int pinCount);
    QByteArray createExpectedBytestream(QList<QString>& expectedOutputs);
    QVariantMap runTests(ChipConfiguration& config, Tests& tests);

    YamlProcessor* yamlProcessor;
    SerialComms* serialComms;
};

#endif // TESTCONTROLLER_H
