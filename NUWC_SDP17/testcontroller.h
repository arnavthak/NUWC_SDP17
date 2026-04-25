/**
 * @file testcontroller.h
 * @brief Controls execution of test sequences and coordinates system components.
 */

#ifndef TESTCONTROLLER_H
#define TESTCONTROLLER_H

#include <QObject>
#include <yamlprocessor.h>
#include <serialcomms.h>
#include <QTimer>
#include <QElapsedTimer>

/**
 * @class TestController
 * @brief Manages execution of tests using configuration data and serial communication.
 *
 * This class coordinates the process of running tests defined in a YAML file.
 * It interacts with the YamlProcessor to parse test configurations and uses
 * SerialComms to communicate with the MCU. It supports stepwise execution,
 * pausing, resuming, and stopping of tests, and emits signals for UI updates.
 */
class TestController : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a TestController object.
     *
     * @param yamlProcessor Pointer to the YAML processing component.
     * @param serialComms Pointer to the serial communication handler.
     * @param parent Optional parent QObject.
     */
    explicit TestController(YamlProcessor* yamlProcessor,
                            SerialComms* serialComms,
                            QObject* parent = nullptr);

    /**
     * @brief Starts execution of tests from a YAML file.
     *
     * This method is invokable from QML and initializes the test run.
     *
     * @param filePath Path to the YAML test configuration file.
     * @param isSimulation Whether to run in simulation mode or hardware mode.
     */
    Q_INVOKABLE void startTests(const QUrl &filePath, bool isSimulation);

    /**
     * @brief Pauses the currently running test sequence.
     *
     * This method is invokable from QML.
     */
    Q_INVOKABLE void pauseTests();

    /**
     * @brief Resumes a paused test sequence.
     *
     * This method is invokable from QML.
     */
    Q_INVOKABLE void resumeTests();

    /**
     * @brief Stops the currently running test sequence.
     *
     * This method is invokable from QML.
     */
    Q_INVOKABLE void stopTests();

private:
    /**
     * @brief Sends chip configuration data to the MCU.
     *
     * @param config The chip configuration to send.
     */
    void sendChipConfiguration(const ChipConfiguration& config);

    /**
     * @brief Resolves expected outputs for sequential test cases.
     *
     * This method processes test steps to determine expected outputs
     * for each stage of a sequential test.
     *
     * @param test The test definition (input-output pairs).
     * @param expectedOutputs Output list to populate.
     * @param pinCount Number of pins in the test.
     */
    void resolveSequentialOutputs(
        const QList<QPair<QString, QList<QString>>>& test,
        QList<QString>& expectedOutputs,
        int pinCount);

    /**
     * @brief Sends a single test to the MCU.
     *
     * @param test The test definition.
     * @param pinCount Number of pins involved in the test.
     */
    void sendTest(const QList<QPair<QString, QList<QString>>>& test, int pinCount);

    /**
     * @brief Creates a byte stream representing expected outputs.
     *
     * @param expectedOutputs List of expected output values.
     * @return Byte array representing expected output sequence.
     */
    QByteArray createExpectedBytestream(QList<QString>& expectedOutputs);

    /**
     * @brief Executes tests step-by-step.
     *
     * This method drives the main execution loop, allowing for
     * pause/resume/stop control.
     *
     * @param filePath Path to the YAML test configuration file.
     * @param isSimulation Whether to run in simulation mode.
     */
    void runTestsStepwise(const QUrl &filePath, bool isSimulation);

    QVariantMap results;              /**< Stores aggregated test results */
    int currentTestIndex;             /**< Index of the current test being executed */
    Tests currentTests;               /**< Parsed list of tests */
    ChipConfiguration currentConfig;  /**< Active chip configuration */
    QUrl currentFile;                 /**< Currently loaded test file */
    bool currentSimulation;           /**< Indicates simulation or hardware mode */

    bool isRunning = false;           /**< Indicates if tests are actively running */
    bool isPaused = false;            /**< Indicates if execution is paused */
    bool isStopped = true;            /**< Indicates if execution is stopped */

    YamlProcessor* yamlProcessor;     /**< Handles YAML parsing */
    SerialComms* serialComms;         /**< Handles MCU communication */

    QElapsedTimer testTimer;          /**< Measures test execution timing */

signals:
    /**
     * @brief Emits a log message for UI display.
     *
     * @param text The message content.
     * @param color Display color (e.g., for status indication).
     */
    void logMessage(QString text, QString color);

    /**
     * @brief Emitted when test results are ready.
     *
     * @param results Aggregated test results.
     */
    void resultsReady(QVariantMap results);

    /**
     * @brief Clears the console/log output in the UI.
     */
    void clearConsole();

    /**
     * @brief Emitted when all tests have completed.
     */
    void testCompleted();
};

#endif // TESTCONTROLLER_H