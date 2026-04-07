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

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    if (format == "json") {
        QJsonObject json = QJsonObject::fromVariantMap(data);
        QJsonDocument doc(json);
        file.write(doc.toJson());
    }
    else if (format == "csv") {
        QTextStream out(&file);

        // Simple CSV structure
        out << "Test,Inputs,Output,Expected,Status\n";

        QVariantMap results = data["results"].toMap();
        QVariantList pinValues = data["pinValues"].toList();

        int i = 0;
        for (auto key : results.keys()) {
            QVariantMap r = results[key].toMap();
            QVariantMap pins = pinValues[i].toMap();

            QString inputStr;
            QStringList keys = pins.keys();
            std::sort(keys.begin(), keys.end(), [](const QString &a, const QString &b){
                return a.toInt() < b.toInt();
            });

            for (int j = 0; j < keys.size(); j++) {
                inputStr += keys[j] + ":" + pins[keys[j]].toString();
                if (j < keys.size() - 1)
                    inputStr += " ";
            }

            out << key << ","
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