#ifndef PDFREPORTGENERATOR_H
#define PDFREPORTGENERATOR_H

#include <QObject>
#include <QStringList>
#include <QMap>

struct PinConfig {
    QString pinName;
    QString direction;   // Input / Output
    QString defaultValue;
};

struct TestResult {
    QString testName;

    QMap<QString, QString> inputPins; // pin -> value

    QString responseBytes;
    QString expectedBytes;

    bool passed;
    double durationMs;
};

class PdfReportGenerator : public QObject
{
    Q_OBJECT

public:
    explicit PdfReportGenerator(QObject *parent = nullptr);

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
    void reportGenerated(const QString &filePath);
    void reportFailed(const QString &reason);
};

#endif // PDFREPORTGENERATOR_H