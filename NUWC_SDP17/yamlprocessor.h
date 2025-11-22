#ifndef YAMLPROCESSOR_H
#define YAMLPROCESSOR_H

#include <QObject>
#include <QUrl>
#include <QString>
#include <QSet>
#include <QMap>
#include <QList>

struct ChipConfiguration {
    QMap<QString, QString> chipInfo;
    QSet<int> outputPins;
    QSet<int> clockPins;
    QMap<QString, QList<int>> pinNames;
    QMap<QString, QList<int>> pinConfigs;
};

class YamlProcessor : public QObject
{
    Q_OBJECT

public:
    explicit YamlProcessor(QObject *parent = nullptr);
    Q_INVOKABLE QString readChipInfo(const QUrl& filePath, const QString fieldName);

public slots:
    ChipConfiguration readChipConfiguration(const QUrl& filePath);

signals:
    Q_INVOKABLE void yamlLoaded(const QUrl& filePath);
};

#endif // YAMLPROCESSOR_H
