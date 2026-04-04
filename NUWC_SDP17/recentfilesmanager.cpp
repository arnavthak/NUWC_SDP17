// recentfilesmanager.cpp
#include "recentfilesmanager.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QStandardPaths>
#include <QDir>

RecentFilesManager::RecentFilesManager(QObject *parent)
    : QObject(parent) {}

QString RecentFilesManager::getFilePath() {
    QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dir); // ensure directory exists
    return dir + "/recent_files.json";
}

QVariantList RecentFilesManager::loadRecentFiles() {
    qDebug() << "Loading recent files...";
    qDebug() << "Recent files path:" << getFilePath();

    QFile file(getFilePath());

    if (!file.exists())
        return {};

    if (!file.open(QIODevice::ReadOnly))
        return {};

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    return doc.array().toVariantList();
}

void RecentFilesManager::saveRecentFiles(const QVariantList &files) {
    qDebug() << "Saving recent files...";
    qDebug() << "Recent files path:" << getFilePath();

    QFile file(getFilePath());

    if (!file.open(QIODevice::WriteOnly))
        return;

    QJsonArray array = QJsonArray::fromVariantList(files);
    QJsonDocument doc(array);

    file.write(doc.toJson());
    file.close();
}
