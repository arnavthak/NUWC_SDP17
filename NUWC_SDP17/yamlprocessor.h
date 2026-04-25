/**
 * @file yamlprocessor.h
 * @brief Parses YAML configuration files into structured data for testing.
 */

// Code Author: Arnav Thakrar

#ifndef YAMLPROCESSOR_H
#define YAMLPROCESSOR_H

#include <QObject>
#include <QUrl>
#include <QString>
#include <QMap>
#include <QList>
#include <QVariantMap>
#include <QSet>

/**
 * @struct ChipConfiguration
 * @brief Represents the configuration of a chip parsed from YAML.
 *
 * This structure contains metadata and pin configuration information
 * required to initialize and interact with the chip under test.
 */
struct ChipConfiguration {
    QMap<QString, QString> chipInfo;                 /**< General chip metadata (e.g., name, pin count) */
    QMap<QString, QList<QString>> pinNames;          /**< Mapping of categories to pin names */
    QMap<QString, QList<QString>> pinConfigs;        /**< Processed pin configuration instructions */
    QMap<QString, QList<QString>> rawPinConfigs;     /**< Raw pin configuration data from YAML */
    QSet<QString> outputPins;                        /**< Set of pins configured as outputs */
    QSet<QString> clockPins;                         /**< Set of pins used as clock signals */
};

/**
 * @struct Tests
 * @brief Represents a collection of test cases parsed from YAML.
 *
 * This structure stores test instructions, expected outputs, and
 * metadata required for executing and validating tests.
 */
struct Tests {
    QList<QList<QPair<QString, QList<QString>>>> tests; /**< Test instructions (command + pin values) */
    QList<QList<QString>> outputs;                      /**< Expected outputs for each test */
    QList<QString> descriptions;                        /**< Descriptions for each test case */
    QList<QMap<int, QString>> pinValueMaps;             /**< Mapping of pin indices to values */
};

/**
 * @class YamlProcessor
 * @brief Handles loading and parsing of YAML files for chip configuration and tests.
 *
 * This class provides functionality to read YAML files and convert them into
 * structured data used by the TestController. It exposes methods for QML usage
 * and emits signals when YAML files are successfully loaded.
 */
class YamlProcessor : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a YamlProcessor object.
     * @param parent Optional parent QObject.
     */
    explicit YamlProcessor(QObject *parent = nullptr);

    /**
     * @brief Loads a YAML file and returns its contents as a QVariantMap.
     *
     * This method is invokable from QML and provides a generic representation
     * of the YAML structure.
     *
     * @param filePath Path to the YAML file.
     * @return Parsed YAML data as a QVariantMap.
     */
    Q_INVOKABLE QVariantMap loadYaml(const QString &filePath);

public slots:
    /**
     * @brief Reads chip configuration from a YAML file.
     *
     * Parses metadata and pin configuration required to initialize the chip.
     *
     * @param filePath Path to the YAML file.
     * @return A ChipConfiguration struct containing parsed data.
     */
    ChipConfiguration readChipConfiguration(const QUrl& filePath);

    /**
     * @brief Reads test definitions from a YAML file.
     *
     * Uses the provided chip configuration to correctly interpret
     * test instructions and expected outputs.
     *
     * @param filePath Path to the YAML file.
     * @param cfg Parsed chip configuration.
     * @return A Tests struct containing parsed test data.
     */
    Tests readTests(const QUrl& filePath, const ChipConfiguration& cfg);

signals:
    /**
     * @brief Emitted when a YAML file has been successfully loaded.
     * @param filePath Path to the loaded YAML file.
     */
    void yamlLoaded(const QUrl& filePath);
};

#endif // YAMLPROCESSOR_H