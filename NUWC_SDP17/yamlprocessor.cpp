// Code Author: Arnav Thakrar

#include "yamlprocessor.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QMap>
#include <QString>
#include <map>
#include <QList>
#include <QVariantMap>
#include <QVariantList>
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

Tests YamlProcessor::readTests(const QUrl& filePath, const ChipConfiguration& cfg)
{
    Tests result;

    YAML::Node root = YAML::LoadFile(filePath.toLocalFile().toStdString());

    QSet<QString> outputPins;
    for (const QString& pin : cfg.pinConfigs["O"]) {
        outputPins.insert(pin);
    }


    static const QMap<QString, QString> instructionMap = {
        {"0", "0x0F"},
        {"1", "0xFF"},
        {"READ", "0x6F"},
        {"C", "0x9F"},
        {"Z", "0x8F"}
    };

    for (auto it = root.begin(); it != root.end(); ++it) {
        QString testID = QString::fromStdString(it->first.as<std::string>());
        if (!testID.contains("Test"))
            continue;

        YAML::Node testNode = it->second;

        QMap<QString, QList<QString>> currentTest;
        QList<QString> currentExpected;

        for (auto jt = testNode.begin(); jt != testNode.end(); ++jt) {

            QString key = QString::fromStdString(jt->first.as<std::string>());
            YAML::Node valNode = jt->second;

            bool isSequential = false;
            bool isToggle = false;

            // --- SPLIT PIN KEYS ---
            QStringList pinNames = key.contains(",") ? key.split(",") : QStringList{key};

            // --- VALUE → BINARY STRING ---
            QString value;

            if (valNode.IsScalar()) {
                QString raw = QString::fromStdString(valNode.as<std::string>());

                if (raw.contains("S")) { isSequential = true; value = "Q"; }
                else if (raw.contains("T")) { isSequential = true; isToggle = true; value = "Q"; }
                else if (raw.startsWith("0b")) value = raw.mid(2);
                else if (instructionMap.contains(raw)) value = raw;
                else if (raw[0].isDigit()) {
                    bool ok;
                    int num = raw.toInt(&ok);
                    if (ok) value = QString::number(num, 2);
                }
                else throw std::runtime_error("Invalid value string");
            }

            // pad binary
            while (value.length() < pinNames.size())
                value.prepend("0");

            if (value.length() > pinNames.size())
                throw std::runtime_error("More values than pins");

            // --- EXPAND PIN NAMES ---
            QList<QString> pins;
            QString valuesExpanded;
            for (int i = 0; i < pinNames.size(); ++i) {
                QString name = pinNames[i].trimmed();

                if (cfg.pinNames.contains(name)) {
                    for (const QString& p : cfg.pinNames[name]) {
                        pins.append(p);
                        valuesExpanded.append(value[i]);
                    }
                }
                else if (name.toInt() > 0) {
                    pins.append(name);
                    valuesExpanded.append(value[i]);
                }
                else {
                    throw std::runtime_error("Invalid pin name");
                }
            }

            // --- CONVERT VALUES → INSTRUCTIONS ---
            QList<QString> instructions;
            for (QChar v : valuesExpanded) {
                QString s(v);
                if (!isSequential && instructionMap.contains(s))
                    instructions.append(instructionMap[s]);
                else
                    instructions.append(QString());  // placeholder
            }

            // --- ASSIGN TO TEST STRUCTURE ---
            for (int i = 0; i < pins.size(); ++i) {
                int pinInt = pins[i].toInt();
                if (pinInt == 0 || pinInt > 24) continue;

                QString pinHex = QString("0x%1").arg(pinInt, 2, 16, QChar('0')).toUpper();
                QString hexInstr = instructions[i];

                if (outputPins.contains(pins[i])) {
                    if (isSequential) {
                        currentExpected.append(isToggle ? "toggle" : "same");
                        hexInstr = instructionMap["READ"] + "_prev";
                    } else {
                        currentExpected.append(hexInstr);
                        hexInstr = instructionMap["READ"];
                    }
                }

                if (!currentTest.contains(hexInstr))
                    currentTest[hexInstr] = QList<QString>();

                currentTest[hexInstr].append(pinHex);
            }
        }

        result.tests.append(currentTest);
        result.outputs.append(currentExpected);
    }

    return result;
}

QVariantMap YamlProcessor::loadYaml(const QString &filePath)
{
    ChipConfiguration cfg = this->readChipConfiguration(filePath);
    Tests tests = this->readTests(filePath, cfg);

    QVariantMap result;

    // ---------------- CHIP INFO ----------------
    QVariantMap chipInfoMap;
    for (auto it = cfg.chipInfo.constBegin(); it != cfg.chipInfo.constEnd(); ++it) {
        chipInfoMap.insert(it.key(), it.value());
    }
    result.insert("chipInfo", chipInfoMap);

    // ---------------- PIN NAMES ----------------
    QVariantMap pinNamesMap;
    for (auto it = cfg.pinNames.constBegin(); it != cfg.pinNames.constEnd(); ++it) {
        QVariantList list;
        for (const QString &pin : it.value())
            list.append(pin);
        pinNamesMap.insert(it.key(), list);
    }
    result.insert("pinNames", pinNamesMap);

    // ---------------- PIN CONFIGS ----------------
    QVariantMap pinConfigsMap;
    for (auto it = cfg.pinConfigs.constBegin(); it != cfg.pinConfigs.constEnd(); ++it) {
        QVariantList list;
        for (const QString &cfgValue : it.value())
            list.append(cfgValue);
        pinConfigsMap.insert(it.key(), list);
    }
    result.insert("pinConfigs", pinConfigsMap);

    // =====================================================
    // TEST INSTRUCTIONS  (QList<QMap<QString,QList<QString>>>)
    // =====================================================
    QVariantList testsList;

    for (const auto& testCase : tests.tests) {
        QVariantMap testMap;

        for (auto it = testCase.constBegin(); it != testCase.constEnd(); ++it) {
            QVariantList pinList;
            for (const QString& pin : it.value())
                pinList.append(pin);

            testMap.insert(it.key(), pinList);  // instruction → [pins]
        }

        testsList.append(testMap);
    }

    result.insert("tests", testsList);

    // =====================================================
    // EXPECTED OUTPUTS  (QList<QList<QString>>)
    // =====================================================
    QVariantList outputsList;

    for (const auto& outputCase : tests.outputs) {
        QVariantList outputValues;
        for (const QString& val : outputCase)
            outputValues.append(val);

        outputsList.append(outputValues);
    }

    result.insert("outputs", outputsList);

    return result;
}


