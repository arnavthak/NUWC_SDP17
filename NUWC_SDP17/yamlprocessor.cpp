#include "yamlprocessor.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <yaml-cpp/yaml.h>

YamlProcessor::YamlProcessor(QObject *parent) : QObject(parent) {}

QString YamlProcessor::readChipInfo(const QUrl& filePath, const QString fieldName) {
    QString localPath = filePath.toLocalFile();

    if (localPath.isEmpty()) {
        qWarning() << "Invalid file path provided.";
        return "Error: Invalid path.";
    }

    QFileInfo fileInfo(localPath);

    if (!fileInfo.exists() || !fileInfo.isReadable()) {
        qWarning() << "File not found or not readable:" << localPath;
        return QString("Error: File not found or not readable.");
    }

    try {
        YAML::Node config = YAML::LoadFile(localPath.toStdString());
        if (config["Chip Info"]) {
            YAML::Node chipInfo = config["Chip Info"];
            if (chipInfo[fieldName.toStdString()]) {
                std::string chipInfoStd = chipInfo[fieldName.toStdString()].as<std::string>();
                return QString::fromStdString(chipInfoStd);
            } else {
                YAML::Emitter out;
                out << config;
                qDebug().noquote() << out.c_str();
                qWarning() << "Key fieldName not found in YAML file.";
                return "Error: Field name key not found.";
            }
        } else {
            YAML::Emitter out;
            out << config;
            qDebug().noquote() << out.c_str();
            qWarning() << "Key 'Chip Info' not found in YAML file.";
            return "Error: Chip info key not found.";
        }
    } catch (const YAML::BadFile& e) {
        // Handles errors if YAML::LoadFile fails to open the file
        qWarning() << "YAML-CPP BadFile Error:" << e.what();
        return QString("Error: Could not open/read file (YAML-CPP).");
    } catch (const YAML::ParserException& e) {
        // Handles errors if the file content is not valid YAML
        qWarning() << "YAML-CPP Parsing Error:" << e.what();
        return QString("Error: Failed to parse YAML content.");
    } catch (const YAML::Exception& e) {
        // Catches other potential YAML-CPP errors (e.g., incorrect type conversion)
        qWarning() << "YAML-CPP General Error:" << e.what();
        return QString("Error: YAML processing failed.");
    }

}

ChipConfiguration YamlProcessor::readChipConfiguration(const QUrl& filePath) {
    qWarning() << "YamlProcessor::readChipConfiguration(const QUrl& filePath) called!";
    return ChipConfiguration{};
}
