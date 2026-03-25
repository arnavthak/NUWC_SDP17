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

ChipConfiguration YamlProcessor::readChipConfiguration(const QUrl& filePath)
{
    ChipConfiguration chipConfig;

    QString localPath = filePath.toLocalFile();

    try {
        YAML::Node fullFile = YAML::LoadFile(localPath.toStdString());

        // =====================================================
        // CONFIG MAP (Python equivalent)
        // =====================================================
        static const QMap<QString, QString> configMap = {
            {"I",   "0x1F"},
            {"O",   "0x2F"},
            {"C_R", "0x3F"},
            {"G",   "0x4F"},
            {"V",   "0x5F"},
            {"C_F", "0x7F"}
        };

        // =====================================================
        // CHIP INFO
        // =====================================================
        if (fullFile["Chip Info"] && fullFile["Chip Info"].IsMap()) {
            YAML::Node chipInfoField = fullFile["Chip Info"];

            for (auto it = chipInfoField.begin(); it != chipInfoField.end(); ++it) {
                QString key = QString::fromStdString(it->first.as<std::string>());
                QString value = QString::fromStdString(it->second.as<std::string>());
                chipConfig.chipInfo.insert(key, value);
            }
        }

        // =====================================================
        // PIN CONFIG
        // =====================================================
        if (fullFile["Pin Config"] && fullFile["Pin Config"].IsMap()) {

            YAML::Node pinConfigField = fullFile["Pin Config"];

            for (auto it = pinConfigField.begin(); it != pinConfigField.end(); ++it) {

                QString keyStr = QString::fromStdString(it->first.as<std::string>());
                QString configStr;

                // ---------------------------------------------
                // Handle dictionary case (named pin)
                // ---------------------------------------------
                if (it->second.IsMap()) {
                    YAML::Node node = it->second;

                    QString name =
                        QString::fromStdString(node["name"].as<std::string>());
                    configStr =
                        QString::fromStdString(node["config"].as<std::string>());

                    chipConfig.pinNames[name].append(keyStr);
                }
                // ---------------------------------------------
                // Handle string case
                // ---------------------------------------------
                else if (it->second.IsScalar()) {
                    configStr =
                        QString::fromStdString(it->second.as<std::string>());
                }
                else {
                    throw std::runtime_error("Incorrect Pin Assignment Data Type");
                }

                // ---------------------------------------------
                // Convert config using config_map
                // ---------------------------------------------
                if (configMap.contains(configStr)) {
                    configStr = configMap[configStr];
                }

                // ---------------------------------------------
                // Convert key to integer and validate
                // ---------------------------------------------
                bool ok;
                int keyInt = keyStr.toInt(&ok);

                if (!ok || keyInt <= 0 || keyInt > 24)
                    continue;

                QString keyHex =
                    QString("0x%1")
                        .arg(keyInt, 2, 16, QChar('0'))
                        .toUpper();

                // ---------------------------------------------
                // Store config grouping (like Python)
                // ---------------------------------------------
                chipConfig.pinConfigs[configStr].append(keyHex);

                // ---------------------------------------------
                // Track output pins
                // ---------------------------------------------
                if (configStr == "0x2F") {
                    chipConfig.outputPins.insert(keyHex);
                }

                // ---------------------------------------------
                // Track clock pins
                // Python: if config in ["0x3F","0x6F"]
                // ---------------------------------------------
                if (configStr == "0x3F" || configStr == "0x6F") {
                    chipConfig.clockPins.insert(keyHex);
                }
            }
        }

    } catch (const YAML::Exception& e) {
        qWarning() << "YAML Error:" << e.what();
    }

    return chipConfig;
}


Tests YamlProcessor::readTests(const QUrl& filePath, const ChipConfiguration& cfg)
{
    Tests result;

    YAML::Node root = YAML::LoadFile(filePath.toLocalFile().toStdString());

    QSet<QString> outputPins = cfg.outputPins;

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

        QList<QPair<QString, QList<QString>>> currentTest;
        QList<QString> currentExpected;
        QString currentDescription;

        for (auto jt = testNode.begin(); jt != testNode.end(); ++jt) {

            QString key = QString::fromStdString(jt->first.as<std::string>());
            YAML::Node valNode = jt->second;

            if (key == "description") {
                currentDescription = QString::fromStdString(valNode.as<std::string>());
                continue;
            }

            bool isSequential = false;
            bool isToggle = false;

            QStringList pinNames = key.contains(",") ? key.split(",") : QStringList{key};

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

            while (value.length() < pinNames.size())
                value.prepend("0");

            if (value.length() > pinNames.size())
                throw std::runtime_error("More values than pins");

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

            QList<QString> instructions;
            for (QChar v : valuesExpanded) {
                QString s(v);
                if (!isSequential && instructionMap.contains(s))
                    instructions.append(instructionMap[s]);
                else
                    instructions.append(QString());
            }

            for (int i = 0; i < pins.size(); ++i) {
                int pinInt = pins[i].toInt();
                if (pinInt == 0 || pinInt > 24) continue;

                QString pinHex = QString("0x%1").arg(pinInt, 2, 16, QChar('0')).toUpper();
                QString hexInstr = instructions[i];

                if (outputPins.contains(pinHex)) {
                    if (isSequential) {
                        currentExpected.append(isToggle ? "toggle" : "same");
                        hexInstr = instructionMap["READ"] + "_prev";
                    } else {
                        currentExpected.append(hexInstr);
                        hexInstr = instructionMap["READ"];
                    }
                }

                // --- REPLACED MAP LOGIC ---
                bool found = false;
                for (auto& pair : currentTest) {
                    if (pair.first == hexInstr) {
                        pair.second.append(pinHex);
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    QList<QString> newList;
                    newList.append(pinHex);
                    currentTest.append(qMakePair(hexInstr, newList));
                }
            }
        }

        result.tests.append(currentTest);
        result.outputs.append(currentExpected);
        result.descriptions.append(currentDescription);
    }

    qDebug() << result.outputs;
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
    // OUTPUT PINS (QSet<QString>)
    // =====================================================
    QVariantList outputPinsList;
    for (const QString &pin : cfg.outputPins) {
        outputPinsList.append(pin);
    }
    result.insert("outputPins", outputPinsList);

    // =====================================================
    // CLOCK PINS (QSet<QString>)
    // =====================================================
    QVariantList clockPinsList;
    for (const QString &pin : cfg.clockPins) {
        clockPinsList.append(pin);
    }
    result.insert("clockPins", clockPinsList);

    // =====================================================
    // TEST INSTRUCTIONS  (QList<QMap<QString,QList<QString>>>)
    // =====================================================
    QVariantList testsList;

    for (const auto& testCase : tests.tests) {
        QVariantMap testMap;

        for (auto it = testCase.constBegin(); it != testCase.constEnd(); ++it) {
            QVariantList pinList;
            for (const QString& pin : it->second)
                pinList.append(pin);

            testMap.insert(it->first, pinList);  // instruction → [pins]
        }

        testsList.append(testMap);
    }

    result.insert("tests", testsList);

    // =====================================================
    // EXPECTED OUTPUTS  (QList<QList<QString>>)
    // =====================================================
    QVariantList outputsList;

    for (const auto& outputCase : tests.outputs) {
        QList<QString> outputValues;
        for (const QString& val : outputCase)
            outputValues.append(val);

        outputsList.append(outputValues);
    }

    result.insert("outputs", outputsList);
    qDebug() << outputsList;

    return result;
}


