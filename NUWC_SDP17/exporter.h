/**
 * @file exporter.h
 * @brief Class for exporting data in JSON & CSV formats
 */

#ifndef EXPORTER_H
#define EXPORTER_H

#include <QObject>

/**
 * @class Exporter
 *
 * @brief Exports application data to JSON or CSV formats.
 *
 * This class formats input data and writes it to a specified file location.
 */
class Exporter : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructs an Exporter object
     */
    Exporter();

    /**
     * @brief Exports input data in specified format and stores in specified location
     *
     * This method is invokable from QML.
     *
     * @param data The input data
     * @param fileUrl The location that the formatted data should be stored
     * @param format Output format. Supported values: "json", "csv".
     *
     * @return None
     */
    Q_INVOKABLE void exportData(const QVariantMap &data, const QUrl &fileUrl, const QString &format);
};

#endif // EXPORTER_H
