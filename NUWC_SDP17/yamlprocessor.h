// Code Author: Arnav Thakrar

#ifndef YAMLPROCESSOR_H
#define YAMLPROCESSOR_H

#include <QObject>
#include <QUrl>
#include <QString>
#include <QMap>
#include <QList>
#include <QVariantMap>

struct ChipConfiguration {
    QMap<QString, QString> chipInfo;
    QMap<QString, QList<QString>> pinNames;
    QMap<QString, QList<QString>> pinConfigs;
    QMap<QString, QList<QString>> rawPinConfigs;
    QSet<QString> outputPins;
    QSet<QString> clockPins;
};

struct Tests {
    QList<QList<QPair<QString, QList<QString>>>> tests;
    QList<QList<QString>> outputs;
    QList<QString> descriptions;
    QList<QMap<int, QString>> pinValueMaps;
};

class YamlProcessor : public QObject
{
    Q_OBJECT

public:
    explicit YamlProcessor(QObject *parent = nullptr);
    Q_INVOKABLE QVariantMap loadYaml(const QString &filePath);

public slots:
    ChipConfiguration readChipConfiguration(const QUrl& filePath);
    Tests readTests(const QUrl& filePath, const ChipConfiguration& cfg);

signals:
    void yamlLoaded(const QUrl& filePath);
};

#endif // YAMLPROCESSOR_H
