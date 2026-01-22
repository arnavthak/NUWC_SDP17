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
};

class YamlProcessor : public QObject
{
    Q_OBJECT

public:
    explicit YamlProcessor(QObject *parent = nullptr);
    Q_INVOKABLE QVariantMap loadYaml(const QString &filePath);

public slots:
    ChipConfiguration readChipConfiguration(const QUrl& filePath);

signals:
    Q_INVOKABLE void yamlLoaded(const QUrl& filePath);
};

#endif // YAMLPROCESSOR_H
