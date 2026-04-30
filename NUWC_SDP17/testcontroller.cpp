#include "testcontroller.h"

TestController::TestController(YamlProcessor* yamlProcessor,
                               SerialComms* serialComms,
                               QObject* parent)
    : QObject(parent),
    yamlProcessor(yamlProcessor),
    serialComms(serialComms)
{
    // Validate dependencies (controller relies on both components)
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

    // Validate configuration before proceeding
    if (!ok) {
        qWarning() << "Invalid Pin Count in chip configuration.";
        return;
    }

    int halfPins = pinCount / 2;

    // Iterate through instruction → pin mappings
    for (auto it = config.pinConfigs.begin(); it != config.pinConfigs.end(); ++it)
    {
        QString instructionKey = it.key();
        QList<QString> pins = it.value();

        // Convert instruction (hex string) to byte
        bool keyOk;
        int instruction = instructionKey.toInt(&keyOk, 16);

        if (!keyOk)
            continue;

        byteStream.append(static_cast<char>(instruction));

        // Convert each pin value into protocol-specific byte
        for (const QString& pinHex : pins)
        {
            bool pinOk;
            int pin = pinHex.toInt(&pinOk, 16);

            if (!pinOk)
                continue;

            // Remap pins in upper half (hardware-specific addressing quirk)
            if (pin > halfPins)
                pin = pin + 24 - pinCount;

            byteStream.append(static_cast<char>(pin));
        }

        // Append termination byte for this instruction packet
        byteStream.append(static_cast<char>(0x00));

        // Send instruction packet to MCU
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

    // Look for special "previous state" instruction
    for (auto it = test.begin(); it != test.end(); ++it)
    {
        if (it->first == "0x6F_prev") {
            values = it->second;
            found = true;
            break;
        }
    }

    // If sequential dependency exists, query MCU for previous state
    if (found) {
        QString key = "0x6F";

        bool keyOk;
        int key_int = key.toInt(&keyOk, 16);

        if (!keyOk) {
            return;
        }

        byteStream.append(static_cast<char>(key_int));

        // Encode pin values
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

        // Send query packet
        byteStream.append(static_cast<char>(0x00));
        serialComms->sendByteStream(byteStream, false);
        byteStream.clear();

        qDebug() << "Sequential queries transmission finished.";

        // Read MCU response (previous output state)
        QString responseBytes = serialComms->readMCU(false, false);
        responseBytes.replace("\n", "");

        qDebug() << "Sequential Response Bytes: " << responseBytes;

        // Resolve placeholders like "same" and "toggle"
        for (int i = 0; i < responseBytes.length(); i++) {

            // "same" → invert logic based on previous state
            if (expectedOutputs[i] == "same") {
                expectedOutputs[i] = (responseBytes[i] == '1') ? "0xFF" : "0x0F";
            }

            // "toggle" → flip logic
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

        // Normalize sequential instruction
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

            // Encode pin values
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

            // Terminate and send instruction
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

    // Convert logical outputs into protocol-specific byte format
    for (const QString& hex : expectedOutputs) {
        if (hex.startsWith("0x")) {
            if (hex == "0x0F") {
                byteStream.append(0x30); // ASCII '0'
            }
            if (hex == "0xFF") {
                byteStream.append(0x31); // ASCII '1'
            }

            // newline delimiter between outputs
            byteStream.append(0x0a);
        }
    }

    return byteStream;
}

void TestController::startTests(const QUrl &filePath, bool isSimulation)
{
    // Resume if paused
    if (isPaused) {
        isPaused = false;
        emit logMessage("[System] Resuming...", "#22c55e");
        QTimer::singleShot(0, this, [=]() {
            runTestsStepwise(currentFile, currentSimulation);
        });
        return;
    }

    emit clearConsole();

    // Initialize execution state
    isRunning = true;
    isStopped = false;
    isPaused = false;

    results.clear();
    currentTestIndex = 0;
    currentFile = filePath;
    currentSimulation = isSimulation;

    emit logMessage("[System] Starting test...", "#9ca3af");

    // Load configuration + tests (safe on main thread)
    emit logMessage("[System] Extracting chip configuration...", "#9ca3af");
    currentConfig = yamlProcessor->readChipConfiguration(filePath);

    emit logMessage("[System] Extracting tests...", "#9ca3af");
    currentTests = yamlProcessor->readTests(filePath, currentConfig);

    // Validate pin count
    bool ok;
    int pinCount = currentConfig.chipInfo.value("Pin Count").toInt(&ok);
    if (!ok) {
        emit logMessage("[Error] Invalid Pin Count in chip configuration", "#ef4444");
        emit resultsReady(QVariantMap());
        isRunning = false;
        isStopped = true;
        return;
    }

    // MCU connection check
    if (!currentSimulation && !serialComms->isMCUConnected()) {
        emit logMessage("[Error] MCU not connected", "#ef4444");
        emit resultsReady(QVariantMap());
        isRunning = false;
        isStopped = true;
        return;
    }

    // Delay → configure → delay → start tests
    QTimer::singleShot(2000, this, [=]() {
        if (!currentSimulation) {
            emit logMessage("[System] Sending chip configuration...", "#9ca3af");
            sendChipConfiguration(currentConfig);
        }

        QTimer::singleShot(3000, this, [=]() {
            runTestsStepwise(currentFile, currentSimulation);
        });
    });
}

void TestController::runTestsStepwise(const QUrl &filePath, bool isSimulation)
{
    QString colorSystem = "#9ca3af";

    // Respect control flags
    if (isStopped || isPaused)
        return;

    // All tests completed
    if (currentTestIndex >= currentTests.tests.length()) {
        emit logMessage("[System] Finished all tests", colorSystem);
        isRunning = false;
        isStopped = true;
        emit resultsReady(results);
        return;
    }

    int i = currentTestIndex;

    // Start timing current test
    testTimer.restart();

    // Execute test (hardware vs simulation)
    if (!isSimulation) {
        emit logMessage(QString("[System] Resolving sequential outputs for Test %1...").arg(i), colorSystem);

        resolveSequentialOutputs(currentTests.tests[i],
                                 currentTests.outputs[i],
                                 currentConfig.chipInfo.value("Pin Count").toInt());

        emit logMessage(QString("[System] Sending Test %1 to MCU...").arg(i), colorSystem);

        sendTest(currentTests.tests[i],
                 currentConfig.chipInfo.value("Pin Count").toInt());
    } else {
        emit logMessage(QString("[System] Simulating Test %1...").arg(i), colorSystem);
    }

    // Generate expected output
    emit logMessage(QString("[System] Creating expected output bytestream for Test %1...").arg(i), colorSystem);

    QByteArray expectedBytestream = createExpectedBytestream(currentTests.outputs[i]);
    QString expected = QString::fromUtf8(expectedBytestream).trimmed().replace("\n", "");

    QString responseBytes;

    if (isSimulation) {
        // In simulation, expected = response
        responseBytes = expected;
    } else {
        // Trigger MCU response read
        serialComms->sendByteStream(QByteArray(), false);
        responseBytes = serialComms->readMCU(false, false).replace("\n", "");
    }

    // Determine pass/fail
    QString result_status = (responseBytes == expected) ? "PASS" : "FAIL";
    QString description = currentTests.descriptions[i];

    qint64 elapsedMs = testTimer.elapsed();

    // Store structured result
    QVariantMap test_stats;
    test_stats.insert("Result", result_status);
    test_stats.insert("Expected", expected);
    test_stats.insert("Response", responseBytes);
    test_stats.insert("Description", description);
    test_stats.insert("Duration_ms", elapsedMs);

    results.insert(QString("Test %1").arg(i+1), test_stats);

    emit logMessage(QString("[System] Stored results for Test %1").arg(i), colorSystem);
    emit logMessage(QString("[System] Test %1 took %2 ms").arg(i).arg(elapsedMs), colorSystem);

    // Update UI
    emit resultsReady(results);
    emit testCompleted();

    // Move to next test after delay (non-blocking)
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

    // Resume execution loop
    runTestsStepwise(currentFile, currentSimulation);
}

void TestController::stopTests()
{
    if (!isRunning)
        return;

    // Reset state and halt execution
    isStopped = true;
    isPaused = false;
    isRunning = false;

    emit logMessage("[System] Stopped", "#ef4444");

    // Reset for next run
    currentTestIndex = 0;
    results.clear();
}