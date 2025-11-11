#ifndef YAMLPROCESSOR_H
#define YAMLPROCESSOR_H

#include <QObject>
#include <QUrl>
#include <QString>

class YamlProcessor : public QObject
{
    Q_OBJECT
public:
    explicit YamlProcessor(QObject *parent = nullptr);
    Q_INVOKABLE QString readChipInfo(const QUrl& filePath, const QString fieldName);
};

#endif // YAMLPROCESSOR_H
