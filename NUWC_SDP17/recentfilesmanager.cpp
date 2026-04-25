// recentfilesmanager.cpp
#include "recentfilesmanager.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QStandardPaths>
#include <QDir>

RecentFilesManager::RecentFilesManager(QObject *parent)
    : QObject(parent) {}

// Returns the full file path used to store recent files data.
// Uses the platform-specific AppDataLocation and ensures the directory exists.
QString RecentFilesManager::getFilePath() {
    QString dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    // Ensure the application data directory exists before writing
    QDir().mkpath(dir);

    return dir + "/recent_files.json";
}

QVariantList RecentFilesManager::loadRecentFiles() {
    qDebug() << "Loading recent files...";
    qDebug() << "Recent files path:" << getFilePath();

    QFile file(getFilePath());

    // If no file exists yet, return an empty list (first run case)
    if (!file.exists())
        return {};

    // If file cannot be opened, fail silently and return empty list
    if (!file.open(QIODevice::ReadOnly))
        return {};

    // Read entire JSON file contents
    QByteArray data = file.readAll();
    file.close();

    // Parse JSON and convert to QVariantList for QML compatibility
    QJsonDocument doc = QJsonDocument::fromJson(data);
    return doc.array().toVariantList();
}

void RecentFilesManager::saveRecentFiles(const QVariantList &files) {
    qDebug() << "Saving recent files...";
    qDebug() << "Recent files path:" << getFilePath();

    QFile file(getFilePath());

    // If file cannot be opened for writing, abort save
    if (!file.open(QIODevice::WriteOnly))
        return;

    // Convert QVariantList -> JSON array for storage
    QJsonArray array = QJsonArray::fromVariantList(files);
    QJsonDocument doc(array);

    // Write JSON data to file (overwrites existing content)
    file.write(doc.toJson());
    file.close();
}