#ifndef PDFREPORTGENERATOR_H
#define PDFREPORTGENERATOR_H

#include <QObject>
#include <QStringList>

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
                                         const QStringList &messages);

signals:
    void reportGenerated(const QString &filePath);
    void reportFailed(const QString &reason);
};

#endif // PDFREPORTGENERATOR_H
