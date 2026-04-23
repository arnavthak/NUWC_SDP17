#include "exporter.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>
#include <QFileDialog>
#include <QDir>

Exporter::Exporter() {}

void Exporter::exportData(const QVariantMap &data, const QUrl &fileUrl, const QString &format)
{
    QString fileName = fileUrl.toLocalFile();
    QFile file(fileName);

    // If the file cannot be opened, silently abort export.
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    if (format == "json") {
        QJsonObject json = QJsonObject::fromVariantMap(data);
        QJsonDocument doc(json);
        file.write(doc.toJson());
    }
    else if (format == "csv") {
        // CSV export assumes the following structure in the input data:
        // - "results": map of test names to result objects
        // - "pinValues": list of input pin maps aligned with results
        //
        // Each row is formatted as:
        // Test, Inputs, Output, Expected, Status
        //
        // Pin inputs are sorted numerically by key before formatting.

        QTextStream out(&file);

        // Simple CSV structure
        out << "Test,Inputs,Output,Expected,Status\n";

        QVariantMap results = data["results"].toMap();
        QVariantList pinValues = data["pinValues"].toList();

        int i = 0;
        for (auto key : results.keys()) {
            // Assumes pinValues[i] corresponds to the current result entry.
            // This relies on consistent ordering between results and pinValues.
            QVariantMap r = results[key].toMap();
            QVariantMap pins = pinValues[i].toMap();

            QString inputStr;
            QStringList keys = pins.keys();

            // Sort pin keys numerically to ensure consistent input ordering (e.g., 1,2,10 instead of 1,10,2)
            std::sort(keys.begin(), keys.end(), [](const QString &a, const QString &b){
                return a.toInt() < b.toInt();
            });

            for (int j = 0; j < keys.size(); j++) {
                inputStr += keys[j] + ":" + pins[keys[j]].toString();
                if (j < keys.size() - 1)
                    inputStr += " ";
            }

            out << key << ","
                // Wrap input string in quotes to preserve spacing within CSV field
                << "\"" << inputStr << "\","
                << r["Response"].toString() << ","
                << r["Expected"].toString() << ","
                << r["Result"].toString()
                << "\n";

            i++;
        }
    }

    file.close();
}