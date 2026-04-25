/**
 * @file serialcomms.h
 * @brief Handles serial communication with an external MCU.
 */

#ifndef SERIALCOMMS_H
#define SERIALCOMMS_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
#include <QByteArray>

/**
 * @class SerialComms
 * @brief Manages serial communication and data exchange with a microcontroller.
 *
 * This class provides functionality to send and receive data over a serial
 * connection. It supports optional CRC-based integrity checking and exposes
 * certain methods to QML for interaction with the GUI.
 */
class SerialComms : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a SerialComms object.
     * @param parent Optional parent QObject.
     */
    explicit SerialComms(QObject *parent = nullptr);

    /**
     * @brief Destroys the SerialComms object.
     */
    ~SerialComms();

    /**
     * @brief Sends a byte stream to the MCU.
     *
     * Optionally appends a CRC checksum before transmission.
     *
     * @param byteStream The raw data to send.
     * @param useCRC Whether to append a CRC checksum.
     */
    void sendByteStream(QByteArray byteStream, bool useCRC);

    /**
     * @brief Reads data from the MCU.
     *
     * Optionally verifies CRC and supports a test mode for debugging.
     *
     * @param useCRC Whether to validate CRC on received data.
     * @param testArduino Whether to use test mode behavior.
     * @return The received message as a QString.
     */
    QString readMCU(bool useCRC = false, bool testArduino = true);

    /**
     * @brief Calculates a CRC checksum for the given data.
     *
     * @param data Input byte array.
     * @return The computed CRC value.
     */
    uint16_t calculateCRC(const QByteArray &data);

    /**
     * @brief Verifies the CRC of received data.
     *
     * @param data Input byte array including checksum.
     * @return True if CRC is valid, false otherwise.
     */
    bool verifyCRC(const QByteArray &data);

    /**
     * @brief Lists available serial ports.
     *
     * Primarily used for debugging connection issues.
     */
    void listAvailablePorts();

    /**
     * @brief Sends a test stream to the MCU.
     *
     * This method is invokable from QML and is intended for debugging.
     *
     * @param stream The test data to send.
     * @return The MCU response.
     */
    Q_INVOKABLE QString sendTestStream(QString stream);

    /**
     * @brief Checks whether the MCU is currently connected.
     *
     * This method is invokable from QML.
     *
     * @return True if the MCU is connected, false otherwise.
     */
    Q_INVOKABLE bool isMCUConnected();

signals:
    /**
     * @brief Emitted when data is received from the MCU.
     * @param message The received message.
     */
    void dataReceived(QString message);

private:
    QSerialPort *serialPort = nullptr; /**< Serial port used for communication */
    QByteArray msgBuffer;              /**< Buffer for accumulating incoming data */
};

#endif // SERIALCOMMS_H