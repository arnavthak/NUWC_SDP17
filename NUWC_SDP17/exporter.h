#ifndef EXPORTER_H
#define EXPORTER_H

#include <QObject>

class Exporter : public QObject
{
    Q_OBJECT
public:
    Exporter();
    Q_INVOKABLE void exportData(const QVariantMap &data, const QUrl &fileUrl, const QString &format);
};

#endif // EXPORTER_H
