/**
 * @file XBeeArduino.h
 * @brief Arduino wrapper for XBee modules, supporting both standard and LoRaWAN modules.
 * 
 * This header file defines the XBeeArduino class, which provides a simple interface for 
 * interacting with XBee modules in an Arduino environment. The class supports both 
 * standard XBee and LoRaWAN modules, allowing for easy initialization, configuration, 
 * and communication.
 * 
 * @version 1.0
 * @date 2024-08-17
 * @author Felix Galindo
 * 
 * @license MIT
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef XBEE_ARDUINO_H
#define XBEE_ARDUINO_H

#include <Arduino.h>
#include "xbee.h"
#include "xbee_lr.h"

/**
 * @enum XBeeModuleType
 * @brief Defines the type of XBee module being used.
 */
enum XBeeModuleType {
    XBEE_STANDARD,    /**< Standard XBee module */
    XBEE_LORA         /**< LoRaWAN XBee module */
};

/**
 * @class XBeeArduino
 * @brief Provides an interface to interact with XBee modules in an Arduino environment.
 * 
 * The XBeeArduino class supports both standard XBee and XBee LoRaWAN modules, offering methods to 
 * initialize, connect, disconnect, send data, and reset the module. It also includes specific 
 * methods for handling LoRaWAN features.
 */
class XBeeArduino {
public:
    /**
     * @brief Constructs a new XBeeArduino object with optional callbacks.
     * 
     * @param serialPort Pointer to the HardwareSerial instance for communication.
     * @param baudrate The baud rate for serial communication.
     * @param moduleType The type of XBee module (XBEE_STANDARD or XBEE_LORA).
     * @param onReceiveCallback Callback function for data reception.
     * @param onSendCallback Callback function for data transmission.
     */
    XBeeArduino(HardwareSerial* serialPort, long baudrate, XBeeModuleType moduleType, void (*onReceiveCallback)(XBeeLRPacket_t*), void (*onSendCallback)(XBeeLRPacket_t*));

    /**
     * @brief Destructor for XBeeArduino.
     * 
     * Cleans up resources associated with the XBee module.
     */
    ~XBeeArduino();

    /**
     * @brief Initializes the XBee module.
     * 
     * @return True if initialization is successful, false otherwise.
     */
    bool begin();

    /**
     * @brief Connects to the XBee network.
     * 
     * @return True if connection is successful, false otherwise.
     */
    bool connect();

    /**
     * @brief Disconnects from the XBee network.
     */
    void disconnect();

    /**
     * @brief Sends data through the XBee module.
     * 
     * @param data Pointer to the data to be sent.
     * @param length Length of the data in bytes.
     * @return True if data is sent successfully, false otherwise.
     */
    bool sendData(const uint8_t* data, size_t length);

    /**
     * @brief Checks if the XBee module is connected.
     * 
     * @return True if connected, false otherwise.
     */
    bool isConnected();

    /**
     * @brief Resets the XBee module.
     */
    void reset();

    /**
     * @brief Sets API options for LoRaWAN XBee modules.
     * 
     * @param options The API options to set.
     * @return True if the options are set successfully, false otherwise.
     */
    bool setLoRaApiOptions(uint8_t options);

    /**
     * @brief Retrieves the Device EUI for LoRaWAN XBee modules.
     * 
     * @param devEUI Pointer to a buffer where the Device EUI will be stored.
     * @return True if the Device EUI is retrieved successfully, false otherwise.
     */
    bool getLoRaDevEUI(uint8_t* devEUI);

private:
    HardwareSerial* serialPort_; /**< Pointer to the HardwareSerial instance used for communication */
    XBeeModuleType moduleType_;  /**< Type of the XBee module (standard or LoRaWAN) */
    uint32_t baudRate_;
    XBee* xbee_;  /**< Pointer to the XBee structure */
    XBeeCTable ctable_;  /**< Callback table for XBee */
    XBeeHTable htable_;  /**< HAL table for XBee */
 };

#endif // XBEE_ARDUINO_H