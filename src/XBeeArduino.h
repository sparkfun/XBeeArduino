#ifndef XBEE_ARDUINO_H
#define XBEE_ARDUINO_H

#include <Arduino.h>
#include "port.h"
#include "xbee.h"
#include "xbee_lr.h"  // Assuming this is where XBeeLRPacket_t and other XBee-related types are defined

/**
 * @file XBeeArduino.h
 * @brief Definition of the XBeeArduino class for Arduino-compatible XBee modules.
 * 
 * This file contains the definition of the XBeeArduino class, which provides an interface 
 * for interacting with XBee modules on an Arduino platform.
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

/**
 * @enum XBeeModuleType
 * @brief Enum to represent the type of XBee module.
 */
enum XBeeModuleType {
    XBEE_STANDARD, ///< Standard XBee module
    XBEE_LORA      ///< LoRa XBee module
};

/**
 * @class XBeeArduino
 * @brief A class to interface with XBee modules on Arduino platforms.
 * 
 * This class provides methods for interacting with XBee modules.
 * It includes functionality for initializing the module, sending/receiving data, and configuring
 * the module.
 */
class XBeeArduino {
public:
    /**
     * @brief Constructor for XBeeArduino class.
     * 
     * @param serialPort A pointer to a Stream object (HardwareSerial or SoftwareSerial).
     * @param baudrate The baud rate for UART communication.
     * @param moduleType The type of XBee module (standard or LoRa).
     * @param onReceiveCallback A callback function to handle received data.
     * @param onSendCallback A callback function to handle post-send events.
     */
    XBeeArduino(Stream* serialPort, uint32_t baudrate, XBeeModuleType moduleType,
                void (*onReceiveCallback)(void*),
                void (*onSendCallback)(void*));

    /**
     * @brief Destructor for XBeeArduino class.
     */
    ~XBeeArduino();

    /**
     * @brief Initializes the XBee module.
     * @return True if initialization is successful, otherwise false.
     */
    bool begin();

    /**
     * @brief Connects the XBee module to the network.
     * @return True if the connection is successful, otherwise false.
     */
    bool connect();

    /**
     * @brief Lets the XBee class process 
     * @return void
     */
    void process();

    /**
     * @brief Disconnects the XBee module from the network.
     * @return True if disconnection is successful, otherwise false.
     */
    bool disconnect();

    /**
     * @brief Sends data through the XBee module.
     * @param data The data to be sent.
     * @return True if the data is sent successfully, otherwise false.
     */
    template <typename T>
    bool sendData(const T& data);

    /**
     * @brief Checks if the XBee module is connected to the network.
     * @return True if the module is connected, otherwise false.
     */
    bool isConnected();

    /**
     * @brief Resets the XBee module.
     */
    void reset();

    /**
     * @brief Sets the API options for XBee.
     * @param options The API options to set.
     * @return True if the options are set successfully, otherwise false.
     */
    bool setApiOptions(const uint8_t options);

    /**
     * @brief Retrieves the DevEUI of the LoRaWAN XBee module.
     * @param devEUI A pointer to a buffer where the DevEUI will be stored.
     * @return True if the DevEUI is retrieved successfully, otherwise false.
     */
    bool getLoRaDevEUI(uint8_t* devEUI, uint8_t length);

    /**
     * @brief Sets the App EUI of the LoRaWAN XBee module.
     * @param value A pointer to a buffer where the App EUI will be stored.
     * @return True if the AppEUI is set successfully, otherwise false.
     */
    bool setLoRaAppEUI(const char* value);

    /**
     * @brief Sets the App Key of the LoRaWAN XBee module.
     * @param value A pointer to a buffer where the App Key will be stored.
     * @return True if the App Key is set successfully, otherwise false.
     */
    bool setLoRaAppKey(const char* value);

    /**
     * @brief Sets the App Key of the LoRaWAN XBee module.
     * @param value A pointer to a buffer where the App Key will be stored.
     * @return True if the App Key is set successfully, otherwise false.
     */
    bool setLoRaNwkKey(const char* value);

    /**
     * @brief Applys config changes on XBee
     * @return True if the changes are applied successfully, otherwise false.
     */
    bool applyChanges(void);
    /**
     * @brief Write config on XBee
     * @return True if the write config is successfull, otherwise false.
     */
    bool writeConfig(void);

private:
    Stream* serialPort_; ///< Pointer to the serial port (HardwareSerial or SoftwareSerial)
    XBeeModuleType moduleType_; ///< Type of XBee module (standard or LoRa)
    XBee* xbee_; ///< Pointer to the XBee object created by the library
    uint32_t baudRate_; ///< Baud rate for UART communication
    void (*onReceiveCallback_)(void*); ///< Callback for received data
    void (*onSendCallback_)(void*); ///< Callback for post-send events
    XBeeCTable ctable_;  ///< Callback table for the XBee library
    XBeeHTable htable_;  ///< Hardware table for the XBee library
    static XBeeArduino* instance_;
    static void onReceiveWrapper(XBee* xbee, void* data);
    static void onSendWrapper(XBee* xbee, void* data);
};

#endif  // XBEE_ARDUINO_H