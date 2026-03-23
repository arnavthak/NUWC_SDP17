#include "testcontroller.h"

TestController::TestController(YamlProcessor* yamlProcessor,
                               SerialComms* serialComms,
                               QObject* parent)
    : QObject(parent),
    yamlProcessor(yamlProcessor),
    serialComms(serialComms)
{
    if (!yamlProcessor) {
        qWarning() << "TestController initialized with null YamlProcessor.";
    }

    if (!serialComms) {
        qWarning() << "TestController initialized with null SerialComms.";
    }
}

void TestController::sendChipConfiguration(const ChipConfiguration& config)
{
    QByteArray byteStream;

    bool ok;
    int pinCount = config.chipInfo.value("Pin Count").toInt(&ok);

    if (!ok) {
        qWarning() << "Invalid Pin Count in chip configuration.";
        return;
    }

    int halfPins = pinCount / 2;

    for (auto it = config.pinConfigs.begin(); it != config.pinConfigs.end(); ++it)
    {
        QString instructionKey = it.key();
        QList<QString> pins = it.value();

        // convert instruction byte (e.g. "0x1F")
        bool keyOk;
        int instruction = instructionKey.toInt(&keyOk, 16);

        if (!keyOk)
            continue;

        byteStream.append(static_cast<char>(instruction));

        for (const QString& pinHex : pins)
        {
            bool pinOk;
            int pin = pinHex.toInt(&pinOk, 16);

            if (!pinOk)
                continue;

            if (pin > halfPins)
                pin = pin + 24 - pinCount;

            byteStream.append(static_cast<char>(pin));
        }

        byteStream.append(static_cast<char>(0x00)); // termination byte

        serialComms->sendByteStream(byteStream, false);

        byteStream.clear();
    }

    qDebug() << "Chip configuration transmission finished.";
}

void TestController::resolveSequentialOutputs(
    const QList<QPair<QString, QList<QString>>>& test,
    QList<QString>& expectedOutputs,
    int pinCount)
{
    QByteArray byteStream;
    int halfPins = pinCount / 2;

    bool found = false;
    QList<QString> values;

    for (auto it = test.begin(); it != test.end(); ++it)
    {
        if (it->first == "0x6F_prev") {
            values = it->second;
            found = true;
            break;
        }
    }

    if (found) {
        QString key = "0x6F";

        bool keyOk;
        int key_int = key.toInt(&keyOk, 16);

        if (!keyOk) {
            return;
        }

        byteStream.append(static_cast<char>(key_int));

        for (const QString& value : values) {
            bool ok;
            int valueInt = value.toInt(&ok, 16);

            if (!ok) {
                break;
            }

            if (valueInt > halfPins) {
                byteStream.append(static_cast<char>(valueInt + 24 - pinCount));
            } else {
                byteStream.append(static_cast<char>(valueInt));
            }
        }

        byteStream.append(static_cast<char>(0x00));
        serialComms->sendByteStream(byteStream, false);
        byteStream.clear();
        qDebug() << "Sequential queries transmission finished.";

        QString responseBytes = serialComms->readMCU(false, false);
        responseBytes.replace("\n", "");

        qDebug() << "Sequential Response Bytes: " << responseBytes;

        for (int i = 0; i < responseBytes.length(); i++) {
            // no toggle
            if (expectedOutputs[i] == "same") {
                expectedOutputs[i] = (responseBytes[i] == '1') ? "0xFF" : "0x0F";
            }

            // toggle
            if (expectedOutputs[i] == "toggle") {
                expectedOutputs[i] = (responseBytes[i] == '1') ? "0x0F" : "0xFF";
            }
        }

        qDebug() << "Sequential outputs resolved.";
    }
}

void TestController::sendTest(const QList<QPair<QString, QList<QString>>>& test, int pinCount)
{
    QByteArray byteStream;
    int halfPins = pinCount / 2;

    for (auto it = test.begin(); it != test.end(); ++it)
    {
        QString key = it->first;
        QList<QString> values = it->second;

        if (key == "0x6F_prev") {
            key = "0x6F";
        }

        if (key.startsWith("0x"))
        {
            bool keyOk;
            int keyInt = key.toInt(&keyOk, 16);

            if (!keyOk) {
                continue;
            }

            byteStream.append(static_cast<char>(keyInt));

            for (const QString& value : values)
            {
                bool valOk;
                int valueInt = value.toInt(&valOk, 16);

                if (!valOk) {
                    continue;
                }

                if (valueInt > halfPins) {
                    byteStream.append(static_cast<char>(valueInt + 24 - pinCount));
                } else {
                    byteStream.append(static_cast<char>(valueInt));
                }
            }

            byteStream.append(static_cast<char>(0x00));

            serialComms->sendByteStream(byteStream, false);

            byteStream.clear();
        }
    }

    qDebug() << "Test transmission finished.";
}

QByteArray TestController::createExpectedBytestream(QList<QString>& expectedOutputs)
{
    QByteArray byteStream;

    for (const QString& hex : expectedOutputs) {
        if (hex.startsWith("0x")) {
            if (hex == "0x0F") {
                byteStream.append(0x30);
            }
            if (hex == "0xFF") {
                byteStream.append(0x31);
            }
            byteStream.append(0x0a);
        }
    }

    return byteStream;
}

QVariantMap TestController::runTests(const QUrl& filePath)
{
    QVariantMap results;

    ChipConfiguration config = yamlProcessor->readChipConfiguration(filePath);
    Tests tests = yamlProcessor->readTests(filePath, config);

    bool ok;
    int pinCount = config.chipInfo.value("Pin Count").toInt(&ok);

    if (!ok) {
        qWarning() << "Invalid Pin Count in chip configuration.";
        return QVariantMap();
    }

    QThread::msleep(2000);

    sendChipConfiguration(config);

    QThread::msleep(3000);

    for (int i = 0; i < tests.tests.length(); i++) {
        resolveSequentialOutputs(tests.tests[i], tests.outputs[i], pinCount);
        sendTest(tests.tests[i], pinCount);

        QByteArray expectedBytestream = createExpectedBytestream(tests.outputs[i]);
        QString expected = QString::fromUtf8(expectedBytestream);
        expected = expected.trimmed();
        expected.replace("\n", "");

        qDebug() << "Expected Response Bytes: " << expected;

        serialComms->sendByteStream(QByteArray(), false);
        QString responseBytes = serialComms->readMCU(false, false).replace("\n", ""); // Unsure about the .replace()

        qDebug() << "Test Response Bytes: " << responseBytes;

        QString result_status = (responseBytes == expected) ? "PASS" : "FAIL";

        results.insert(QString("Test %1").arg(i), result_status);

        QThread::msleep(3000);
    }

    return results;

}
