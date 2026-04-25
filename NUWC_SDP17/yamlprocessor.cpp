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

/**
 * Parses chip configuration data from a YAML file.
 * Extracts chip metadata, pin mappings, and configuration groupings.
 */
ChipConfiguration YamlProcessor::readChipConfiguration(const QUrl& filePath)
{
    ChipConfiguration chipConfig;

    QString localPath = filePath.toLocalFile();

    try {
        // Load YAML file into memory
        YAML::Node fullFile = YAML::LoadFile(localPath.toStdString());

        // Mapping from shorthand config labels to instruction hex values
        static const QMap<QString, QString> configMap = {
            {"I",   "0x1F"},
            {"O",   "0x2F"},
            {"C_R", "0x3F"},
            {"G",   "0x4F"},
            {"V",   "0x5F"},
            {"C_F", "0x7F"}
        };

        // ============================
        // CHIP INFO SECTION
        // ============================
        if (fullFile["Chip Info"] && fullFile["Chip Info"].IsMap()) {
            YAML::Node chipInfoField = fullFile["Chip Info"];

            // Copy all key-value metadata into chipInfo map
            for (auto it = chipInfoField.begin(); it != chipInfoField.end(); ++it) {
                QString key = QString::fromStdString(it->first.as<std::string>());
                QString value = QString::fromStdString(it->second.as<std::string>());
                chipConfig.chipInfo.insert(key, value);
            }
        }

        // ============================
        // PIN CONFIGURATION SECTION
        // ============================
        if (fullFile["Pin Config"] && fullFile["Pin Config"].IsMap()) {

            YAML::Node pinConfigField = fullFile["Pin Config"];

            for (auto it = pinConfigField.begin(); it != pinConfigField.end(); ++it) {

                QString keyStr = QString::fromStdString(it->first.as<std::string>());
                QString configStr;
                QString name = "";
                QString description = "";

                // Case 1: Pin defined as a structured object (name/config/description)
                if (it->second.IsMap()) {
                    YAML::Node node = it->second;

                    name = node["name"] ? QString::fromStdString(node["name"].as<std::string>()) : "";
                    configStr = node["config"] ? QString::fromStdString(node["config"].as<std::string>()) : "";
                    description = node["description"] ? QString::fromStdString(node["description"].as<std::string>()) : "";

                    // Map logical pin names to physical pins
                    chipConfig.pinNames[name].append(keyStr);
                }
                // Case 2: Pin defined as a simple scalar value
                else if (it->second.IsScalar()) {
                    configStr = QString::fromStdString(it->second.as<std::string>());
                }
                else {
                    throw std::runtime_error("Incorrect Pin Assignment Data Type");
                }

                // Store raw configuration for UI/debugging purposes
                QList<QString> raw_data;
                raw_data.append(configStr);
                raw_data.append(name);
                raw_data.append(description);

                chipConfig.rawPinConfigs[keyStr] = raw_data;

                // Convert shorthand config (e.g., "I") to instruction hex
                if (configMap.contains(configStr)) {
                    configStr = configMap[configStr];
                }

                // Validate pin number and convert to hex format
                bool ok;
                int keyInt = keyStr.toInt(&ok);

                if (!ok || keyInt <= 0 || keyInt > 24)
                    continue;

                QString keyHex =
                    QString("0x%1")
                        .arg(keyInt, 2, 16, QChar('0'))
                        .toUpper();

                // Group pins by configuration instruction
                chipConfig.pinConfigs[configStr].append(keyHex);

                // Track output pins separately
                if (configStr == "0x2F") {
                    chipConfig.outputPins.insert(keyHex);
                }

                // Track clock-related pins
                if (configStr == "0x3F" || configStr == "0x6F") {
                    chipConfig.clockPins.insert(keyHex);
                }
            }
        }

    } catch (const YAML::Exception& e) {
        // Handle YAML parsing errors gracefully
        qWarning() << "YAML Error:" << e.what();
    }

    return chipConfig;
}


/**
 * Parses test cases from YAML and converts them into structured instructions.
 * Handles sequential logic, pin expansion, and expected output generation.
 */
Tests YamlProcessor::readTests(const QUrl& filePath, const ChipConfiguration& cfg)
{
    Tests result;

    YAML::Node root = YAML::LoadFile(filePath.toLocalFile().toStdString());

    QSet<QString> outputPins = cfg.outputPins;

    // Mapping of logical values to instruction bytes
    static const QMap<QString, QString> instructionMap = {
        {"0", "0x0F"},
        {"1", "0xFF"},
        {"READ", "0x6F"},
        {"C", "0x9F"},
        {"Z", "0x8F"}
    };

    // Iterate through all YAML entries
    for (auto it = root.begin(); it != root.end(); ++it) {
        QString testID = QString::fromStdString(it->first.as<std::string>());
        if (!testID.contains("Test"))
            continue;

        YAML::Node testNode = it->second;

        QList<QPair<QString, QList<QString>>> currentTest;
        QList<QString> currentExpected;
        QString currentDescription;
        QMap<int, QString> currentPinValues;

        // Process each instruction inside a test
        for (auto jt = testNode.begin(); jt != testNode.end(); ++jt) {

            QString key = QString::fromStdString(jt->first.as<std::string>());
            YAML::Node valNode = jt->second;

            // Extract test description
            if (key == "description") {
                currentDescription = QString::fromStdString(valNode.as<std::string>());
                continue;
            }

            bool isSequential = false;
            bool isToggle = false;

            // Support multi-pin syntax like "A,B,C"
            QStringList pinNames = key.contains(",") ? key.split(",") : QStringList{key};

            QString value;

            // Parse value encoding
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

            // Normalize bit string length
            while (value.length() < pinNames.size())
                value.prepend("0");

            if (value.length() > pinNames.size())
                throw std::runtime_error("More values than pins");

            QList<QString> pins;
            QString valuesExpanded;

            // Expand named pins into actual pin numbers
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

            // Convert bit values into instruction bytes
            QList<QString> instructions;
            for (QChar v : valuesExpanded) {
                QString s(v);
                if (!isSequential && instructionMap.contains(s))
                    instructions.append(instructionMap[s]);
                else
                    instructions.append(QString());
            }

            // Build test instruction + expected output mapping
            for (int i = 0; i < pins.size(); ++i) {
                int pinInt = pins[i].toInt();
                if (pinInt == 0 || pinInt > 24) continue;

                QString pinHex = QString("0x%1").arg(pinInt, 2, 16, QChar('0')).toUpper();
                QString bitVal = QString(valuesExpanded[i]);

                // Track input pin values
                if (!outputPins.contains(pinHex)) {
                    currentPinValues[pinInt] = bitVal;
                }

                QString hexInstr = instructions[i];

                // Handle output pins differently
                if (outputPins.contains(pinHex)) {
                    if (isSequential) {
                        currentExpected.append(isToggle ? "toggle" : "same");
                        hexInstr = instructionMap["READ"] + "_prev";
                    } else {
                        currentExpected.append(hexInstr);
                        hexInstr = instructionMap["READ"];
                    }
                }

                // Group pins by instruction (merge duplicates)
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

        // Store parsed test
        result.tests.append(currentTest);
        result.outputs.append(currentExpected);
        result.descriptions.append(currentDescription);
        result.pinValueMaps.append(currentPinValues);
    }

    qDebug() << result.outputs;
    return result;
}


/**
 * High-level wrapper that loads YAML and converts it into a QVariantMap
 * for easy use in QML/UI layers.
 */
QVariantMap YamlProcessor::loadYaml(const QString &filePath)
{
    ChipConfiguration cfg = this->readChipConfiguration(filePath);
    Tests tests = this->readTests(filePath, cfg);

    QVariantMap result;

    // Convert chip info to QVariantMap
    QVariantMap chipInfoMap;
    for (auto it = cfg.chipInfo.constBegin(); it != cfg.chipInfo.constEnd(); ++it) {
        chipInfoMap.insert(it.key(), it.value());
    }
    result.insert("chipInfo", chipInfoMap);

    // Convert pin names
    QVariantMap pinNamesMap;
    for (auto it = cfg.pinNames.constBegin(); it != cfg.pinNames.constEnd(); ++it) {
        QVariantList list;
        for (const QString &pin : it.value())
            list.append(pin);
        pinNamesMap.insert(it.key(), list);
    }
    result.insert("pinNames", pinNamesMap);

    // Convert pin configs
    QVariantMap pinConfigsMap;
    for (auto it = cfg.pinConfigs.constBegin(); it != cfg.pinConfigs.constEnd(); ++it) {
        QVariantList list;
        for (const QString &cfgValue : it.value())
            list.append(cfgValue);
        pinConfigsMap.insert(it.key(), list);
    }
    result.insert("pinConfigs", pinConfigsMap);

    // Raw configs (for UI/debugging)
    QVariantMap rawPinConfigMap;
    for (auto it = cfg.rawPinConfigs.constBegin(); it != cfg.rawPinConfigs.constEnd(); ++it) {
        QVariantList list;
        for (const QString &cfgValue : it.value())
            list.append(cfgValue);
        rawPinConfigMap.insert(it.key(), list);
    }
    result.insert("rawPinConfigs", rawPinConfigMap);

    // Convert output pins set
    QVariantList outputPinsList;
    for (const QString &pin : cfg.outputPins) {
        outputPinsList.append(pin);
    }
    result.insert("outputPins", outputPinsList);

    // Convert clock pins set
    QVariantList clockPinsList;
    for (const QString &pin : cfg.clockPins) {
        clockPinsList.append(pin);
    }
    result.insert("clockPins", clockPinsList);

    // Convert tests
    QVariantList testsList;
    for (const auto& testCase : tests.tests) {
        QVariantMap testMap;

        for (auto it = testCase.constBegin(); it != testCase.constEnd(); ++it) {
            QVariantList pinList;
            for (const QString& pin : it->second)
                pinList.append(pin);

            testMap.insert(it->first, pinList);
        }

        testsList.append(testMap);
    }
    result.insert("tests", testsList);

    // Convert expected outputs
    QVariantList outputsList;
    for (const auto& outputCase : tests.outputs) {
        QList<QString> outputValues;
        for (const QString& val : outputCase)
            outputValues.append(val);

        outputsList.append(outputValues);
    }
    result.insert("outputs", outputsList);

    // Convert pin value maps
    QVariantList pinValuesList;
    for (const auto& testMap : tests.pinValueMaps) {
        QVariantMap map;
        for (auto it = testMap.begin(); it != testMap.end(); ++it) {
            map.insert(QString::number(it.key()), it.value());
        }
        pinValuesList.append(map);
    }
    result.insert("pinValues", pinValuesList);

    // Notify listeners that YAML has been loaded
    emit yamlLoaded(filePath);

    return result;
}