#include "yamlprocessor.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QMap>
#include <QString>
#include <map>
#include <QList>
#include <yaml-cpp/yaml.h>

YamlProcessor::YamlProcessor(QObject *parent) : QObject(parent) {}

ChipConfiguration YamlProcessor::readChipConfiguration(const QUrl& filePath) {

    ChipConfiguration chipConfig;
    chipConfig.chipInfo = QMap<QString, QString>();
    chipConfig.pinConfigs = QMap<QString, QList<QString>>();
    chipConfig.pinNames = QMap<QString, QList<QString>>();

    QString localPath = filePath.toLocalFile();

    if (localPath.isEmpty()) {
        qWarning() << "Invalid file path provided.";
    }

    QFileInfo fileInfo(localPath);

    if (!fileInfo.exists() || !fileInfo.isReadable()) {
        qWarning() << "File not found or not readable:" << localPath;
    }

    try {
        YAML::Node fullFile = YAML::LoadFile(localPath.toStdString());

        if (fullFile["Chip Info"]) {
            YAML::Node chipInfoField = fullFile["Chip Info"];
            if (chipInfoField.IsMap()) {
                for (YAML::const_iterator it = chipInfoField.begin(); it != chipInfoField.end(); ++it) {
                    chipConfig.chipInfo[QString::fromStdString(it->first.as<std::string>())] = QString::fromStdString(it->second.as<std::string>());
                }
            }
        } else {
            YAML::Emitter out;
            out << fullFile;
            qDebug().noquote() << out.c_str();
            qWarning() << "Key 'Chip Info' not found in YAML file.";
        }

        if (fullFile["Pin Config"]) {
            YAML::Node pinConfigField = fullFile["Pin Config"];
            if (pinConfigField.IsMap()) {
                qDebug()<< "Pin Config";
                for (YAML::const_iterator it = pinConfigField.begin(); it != pinConfigField.end(); ++it) {
                    QString key = QString::fromStdString(it->first.as<std::string>());
                    QString config;
                    if (it->second.IsMap()) {
                        auto value = it->second.as<std::map<std::string, std::string>>();
                        config = QString::fromStdString(value["config"]);
                        QString name = QString::fromStdString(value["name"]);

                        if (!chipConfig.pinNames.contains(name)) {
                            chipConfig.pinNames[name] = QList<QString>();
                        }
                        chipConfig.pinNames[name].append(key);

                    } else {
                        config = QString::fromStdString(it->second.as<std::string>());
                    }

                    if (!chipConfig.pinConfigs.contains(config)) {
                        chipConfig.pinConfigs[config] = QList<QString>();
                    }
                    chipConfig.pinConfigs[config].append(key);
                }
            }
        } else {
            YAML::Emitter out;
            out << fullFile;
            qDebug().noquote() << out.c_str();
            qWarning() << "Key 'Pin Config' not found in YAML file.";
        }

    } catch (const YAML::BadFile& e) {
        // Handles errors if YAML::LoadFile fails to open the file
        qWarning() << "YAML-CPP BadFile Error:" << e.what();
    } catch (const YAML::ParserException& e) {
        // Handles errors if the file content is not valid YAML
        qWarning() << "YAML-CPP Parsing Error:" << e.what();
    } catch (const YAML::Exception& e) {
        // Catches other potential YAML-CPP errors (e.g., incorrect type conversion)
        qWarning() << "YAML-CPP General Error:" << e.what();
    }

    qDebug() << "Contents of chipInfo:";
    for (auto i = chipConfig.chipInfo.constBegin(); i != chipConfig.chipInfo.constEnd(); ++i) {
        qDebug() << i.key() << ":" << i.value();
    }

    qDebug() << "Contents of pinNames:";
    for (auto i = chipConfig.pinNames.constBegin(); i != chipConfig.pinNames.constEnd(); ++i) {
        qDebug() << i.key() << ":" << i.value();
    }

    qDebug() << "Contents of pinConfigs:";
    for (auto i = chipConfig.pinConfigs.constBegin(); i != chipConfig.pinConfigs.constEnd(); ++i) {
        qDebug() << i.key() << ":" << i.value();
    }

    return chipConfig;
}
