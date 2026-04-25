/**
 * @file recentfilesmanager.h
 * @brief Manages loading and saving of recently accessed files.
 */

#ifndef RECENTFILESMANAGER_H
#define RECENTFILESMANAGER_H

#include <QObject>
#include <QVariantList>

/**
 * @class RecentFilesManager
 * @brief Handles persistence of recently opened files.
 *
 * This class provides functionality to load and save a list of
 * recently accessed files. It is designed to be used from QML
 * via Q_INVOKABLE methods.
 */
class RecentFilesManager : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructs a RecentFilesManager object.
     * @param parent Optional parent QObject.
     */
    explicit RecentFilesManager(QObject *parent = nullptr);

    /**
     * @brief Loads the list of recent files.
     *
     * This method retrieves the stored list of recently accessed files.
     *
     * @return A QVariantList containing recent file entries.
     */
    Q_INVOKABLE QVariantList loadRecentFiles();

    /**
     * @brief Saves the list of recent files.
     *
     * This method persists the provided list of recent file entries.
     *
     * @param files A QVariantList containing file entries to store.
     */
    Q_INVOKABLE void saveRecentFiles(const QVariantList &files);

private:
    /**
     * @brief Returns the file path used for storing recent files.
     *
     * This method determines the location where the recent files
     * data is persisted (e.g., a config or application data file).
     *
     * @return The file path as a QString.
     */
    QString getFilePath();
};

#endif // RECENTFILESMANAGER_H