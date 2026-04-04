#ifndef RECENTFILESMANAGER_H
#define RECENTFILESMANAGER_H

#include <QObject>
#include <QVariantList>

class RecentFilesManager : public QObject {
    Q_OBJECT

public:
    explicit RecentFilesManager(QObject *parent = nullptr);

    Q_INVOKABLE QVariantList loadRecentFiles();
    Q_INVOKABLE void saveRecentFiles(const QVariantList &files);

private:
    QString getFilePath();
};

#endif // RECENTFILESMANAGER_H
