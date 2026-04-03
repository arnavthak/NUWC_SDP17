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

void TestController::startTests(const QUrl &filePath, bool isSimulation)
{
    if (isPaused) {
        isPaused = false;
        emit logMessage("[System] Resuming...", "#22c55e");
        runTestsStepwise(currentFile, currentSimulation);
        return;
    }

    emit clearConsole();

    isRunning = true;
    isStopped = false;
    isPaused = false;

    // Initialize
    results.clear();
    currentTestIndex = 0;
    currentFile = filePath;
    currentSimulation = isSimulation;

    emit logMessage("[System] Starting test...", "#9ca3af");

    // Step 1: Extract chip config and tests in main thread (QObject-safe)
    emit logMessage("[System] Extracting chip configuration...", "#9ca3af");
    currentConfig = yamlProcessor->readChipConfiguration(filePath);

    emit logMessage("[System] Extracting tests...", "#9ca3af");
    currentTests = yamlProcessor->readTests(filePath, currentConfig);

    bool ok;
    int pinCount = currentConfig.chipInfo.value("Pin Count").toInt(&ok);
    if (!ok) {
        emit logMessage("[Error] Invalid Pin Count in chip configuration", "#ef4444");
        emit resultsReady(QVariantMap());
        isRunning = false;
        isStopped = true;
        return;
    }

    // Step 2: Start stepwise execution with a timer to avoid blocking GUI
    QTimer::singleShot(0, this, [=]() { runTestsStepwise(filePath, isSimulation); });
}

void TestController::runTestsStepwise(const QUrl &filePath, bool isSimulation)
{
    QString colorSystem = "#9ca3af";

    if (isStopped)
        return;

    if (isPaused)
        return;

    // Done with all tests
    if (currentTestIndex >= currentTests.tests.length()) {
        emit logMessage("[System] Finished all tests", colorSystem);
        isRunning = false;
        isStopped = true;
        emit resultsReady(results);
        return;
    }

    int i = currentTestIndex;

    // Execute current test
    if (!isSimulation) {
        emit logMessage(QString("[System] Resolving sequential outputs for Test %1...").arg(i), colorSystem);
        resolveSequentialOutputs(currentTests.tests[i], currentTests.outputs[i],
                                 currentConfig.chipInfo.value("Pin Count").toInt());
        emit logMessage(QString("[System] Sending Test %1 to MCU...").arg(i), colorSystem);
        sendTest(currentTests.tests[i],
                 currentConfig.chipInfo.value("Pin Count").toInt());
    } else {
        emit logMessage(QString("[System] Simulating Test %1...").arg(i), colorSystem);
    }

    emit logMessage(QString("[System] Creating expected output bytestream for Test %1...").arg(i), colorSystem);
    QByteArray expectedBytestream = createExpectedBytestream(currentTests.outputs[i]);
    QString expected = QString::fromUtf8(expectedBytestream).trimmed().replace("\n", "");

    QString responseBytes;
    if (isSimulation) {
        responseBytes = expected;
    } else {
        serialComms->sendByteStream(QByteArray(), false);
        responseBytes = serialComms->readMCU(false, false).replace("\n", "");
    }

    QString result_status = (responseBytes == expected) ? "PASS" : "FAIL";
    QString description = currentTests.descriptions[i];

    QVariantMap test_stats;
    test_stats.insert("Result", result_status);
    test_stats.insert("Expected", expected);
    test_stats.insert("Response", responseBytes);
    test_stats.insert("Description", description);

    results.insert(QString("Test %1").arg(i), test_stats);

    emit logMessage(QString("[System] Stored results for Test %1").arg(i), colorSystem);

    // Increment index and schedule next test after 3 seconds
    currentTestIndex++;
    QTimer::singleShot(3000, this, [=]() {
        if (!isPaused && !isStopped) {
            runTestsStepwise(filePath, isSimulation);
        }
    });
}

void TestController::pauseTests()
{
    if (!isRunning)
        return;

    isPaused = true;
    emit logMessage("[System] Paused", "#facc15");
}

void TestController::resumeTests()
{
    if (!isPaused) return;

    isPaused = false;
    emit logMessage("[System] Resuming...", "#22c55e");

    // Resume execution
    runTestsStepwise(currentFile, currentSimulation);
}

void TestController::stopTests()
{
    if (!isRunning)
        return;

    isStopped = true;
    isPaused = false;
    isRunning = false;

    emit logMessage("[System] Stopped", "#ef4444");

    // Reset state so next start begins fresh
    currentTestIndex = 0;
    results.clear();
}
