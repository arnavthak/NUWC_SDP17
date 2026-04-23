/**
 * @file pdfreportgenerator.h
 * @brief Provides functionality for generating PDF test reports.
 */

#ifndef PDFREPORTGENERATOR_H
#define PDFREPORTGENERATOR_H

#include <QObject>
#include <QStringList>
#include <QMap>

/**
 * @struct PinConfig
 * @brief Represents configuration information for a single pin.
 *
 * This structure stores metadata about a pin, including its name,
 * direction, and default value.
 */
struct PinConfig {
    QString pinName;       /**< Name of the pin */
    QString direction;     /**< Direction of the pin ("Input" or "Output") */
    QString defaultValue;  /**< Default value assigned to the pin */
};

/**
 * @struct TestResult
 * @brief Represents the result of a single test case.
 *
 * This structure contains all relevant data for a test execution,
 * including input values, expected and actual outputs, and status.
 */
struct TestResult {
    QString testName;  /**< Name of the test */

    QMap<QString, QString> inputPins; /**< Mapping of pin names to input values */

    QString responseBytes;  /**< Actual response from the test */
    QString expectedBytes;  /**< Expected response */

    bool passed;        /**< Indicates whether the test passed */
    double durationMs;  /**< Execution time in milliseconds */
};

/**
 * @class PdfReportGenerator
 * @brief Generates PDF reports summarizing test results.
 *
 * This class formats test results data and exports it as a PDF report.
 * It supports integration with QML via Q_INVOKABLE methods and emits
 * signals to indicate success or failure of report generation.
 */
class PdfReportGenerator : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a PdfReportGenerator object.
     * @param parent Optional parent QObject.
     */
    explicit PdfReportGenerator(QObject *parent = nullptr);

    /**
     * @brief Generates a basic PDF report from test data.
     *
     * This method is invokable from QML via Q_INVOKABLE.
     * It compiles test results, pin configurations, and summary data
     * into a formatted PDF report saved to the specified file path.
     *
     * @param filePath The destination file path for the generated PDF.
     * @param chipName The name of the chip under test.
     * @param testMode The mode in which testing was performed. Must be "Simulation" or "Hardware".
     * @param totalTests Total number of tests executed.
     * @param passedTests Number of tests that passed.
     * @param failedTests Number of tests that failed.
     * @param pinConfigsVar List of pin configuration objects (QVariantList).
     * @param testsVar List of test result objects (QVariantList).
     * @param messages Additional messages or notes to include in the report.
     *
     * @return True if the report was generated successfully, false otherwise.
     */
    Q_INVOKABLE bool generateBasicReport(const QString &filePath,
                                        const QString &chipName,
                                        const QString &testMode,
                                        int totalTests,
                                        int passedTests,
                                        int failedTests,
                                        const QVariantList &pinConfigsVar,
                                        const QVariantList &testsVar,
                                        const QStringList &messages);

signals:
    /**
     * @brief Emitted when a report is successfully generated.
     * @param filePath The file path of the generated report.
     */
    void reportGenerated(const QString &filePath);

    /**
     * @brief Emitted when report generation fails.
     * @param reason Description of the failure.
     */
    void reportFailed(const QString &reason);
};

#endif // PDFREPORTGENERATOR_H