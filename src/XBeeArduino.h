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
    bool getLoRaWANDevEUI(uint8_t* devEUI, uint8_t length);

    /**
     * @brief Sets the App EUI of the LoRaWAN XBee module.
     * @param value A pointer to a buffer where the App EUI will be stored.
     * @return True if the AppEUI is set successfully, otherwise false.
     */
    bool setLoRaWANAppEUI(const char* value);

    /**
     * @brief Sets the App Key of the LoRaWAN XBee module.
     * @param value A pointer to a buffer where the App Key will be stored.
     * @return True if the App Key is set successfully, otherwise false.
     */
    bool setLoRaWANAppKey(const char* value);

    /**
     * @brief Sets the App Key of the LoRaWAN XBee module.
     * @param value A pointer to a buffer where the App Key will be stored.
     * @return True if the App Key is set successfully, otherwise false.
     */
    bool setLoRaWANNwkKey(const char* value);

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

    /**
     * @brief Sets the LoRaWAN Class on the XBee LR module.
     * 
     * This function sends the AT_LC command to set the LoRaWAN Class 
     * (e.g., 'A', 'B', 'C') on the XBee LR module.
     * 
     * @param[in] value The LoRaWAN Class to be set ('A', 'B', or 'C').
     * 
     * @return bool Returns true if the command was successfully sent and the response was positive, otherwise false.
     */
    bool setLoRaWANClass(const char value);

    /**
     * @brief Sets the LoRaWAN Activation Mode on the XBee LR module.
     * 
     * This function sends the AT_AM command to set the Activation Mode 
     * (e.g., OTAA or ABP) on the XBee LR module.
     * 
     * @param[in] value The Activation Mode to be set.
     * 
     * @return bool Returns true if the command was successfully sent and the response was positive, otherwise false.
     */
    bool setLoRaWANActivationMode(const uint8_t value);

    /**
     * @brief Sets the LoRaWAN Adaptive Data Rate (ADR) on the XBee LR module.
     * 
     * This function sends the AT_AD command to enable or disable the 
     * Adaptive Data Rate (ADR) feature on the XBee LR module.
     * 
     * @param[in] value The ADR setting to be set (e.g., 0 for disable, 1 for enable).
     * 
     * @return bool Returns true if the command was successfully sent and the response was positive, otherwise false.
     */
    bool setLoRaWANADR(const uint8_t value);

    /**
     * @brief Sets the LoRaWAN Data Rate on the XBee LR module.
     * 
     * This function sends the AT_DR command to set the Data Rate 
     * on the XBee LR module.
     * 
     * @param[in] value The Data Rate to be set.
     * 
     * @return bool Returns true if the command was successfully sent and the response was positive, otherwise false.
     */
    bool setLoRaWANDataRate(const uint8_t value);

    /**
     * @brief Sets the LoRaWAN Region on the XBee LR module.
     * 
     * This function sends the AT_LR command to set the operational region 
     * (e.g., US915, EU868) on the XBee LR module.
     * 
     * @param[in] value The Region to be set.
     * 
     * @return bool Returns true if the command was successfully sent and the response was positive, otherwise false.
     */
    bool setLoRaWANRegion(const uint8_t value);

    /**
     * @brief Sets the LoRaWAN Duty Cycle on the XBee LR module.
     * 
     * This function sends the AT_DC command to configure the duty cycle 
     * settings on the XBee LR module.
     * 
     * @param[in] value The Duty Cycle to be set.
     * 
     * @return bool Returns true if the command was successfully sent and the response was positive, otherwise false.
     */
    bool setLoRaWANDutyCycle(const uint8_t value);

    /**
     * @brief Gets the LoRaWAN Specification Version from the XBee LR module.
     * 
     * This function sends the AT_LV command to retrieve the LoRaWAN 
     * Specification Version currently in use by the XBee LR module.
     * 
     * @param[out] responseBuffer Buffer to store the retrieved version string.
     * @param[in] buffer_size The size of the response buffer.
     * 
     * @return bool Returns true if the command was successfully sent and a valid response was received, otherwise false.
     */
    bool getLoRaWANSpecVersion(char* responseBuffer, uint8_t buffer_size);

    /**
     * @brief Sets the LoRaWAN Join RX1 Delay on the XBee LR module.
     * 
     * This function sends the AT_J1 command to set the RX1 delay 
     * (in milliseconds) for the Join Accept message on the XBee LR module.
     * 
     * @param[in] value The RX1 Delay value to be set (in milliseconds).
     * 
     * @return bool Returns true if the command was successfully sent and the response was positive, otherwise false.
     */
    bool setLoRaWANJoinRX1Delay(const uint32_t value);

    /**
     * @brief Sets the LoRaWAN Join RX2 Delay on the XBee LR module.
     * 
     * This function sends the AT_J2 command to set the RX2 delay 
     * (in milliseconds) for the Join Accept message on the XBee LR module.
     * 
     * @param[in] value The RX2 Delay value to be set (in milliseconds).
     * 
     * @return bool Returns true if the command was successfully sent and the response was positive, otherwise false.
     */
    bool setLoRaWANJoinRX2Delay(const uint32_t value);

    /**
     * @brief Sets the LoRaWAN RX1 Delay on the XBee LR module.
     * 
     * This function sends the AT_D1 command to set the RX1 delay 
     * (in milliseconds) on the XBee LR module.
     * 
     * @param[in] value The RX1 Delay value to be set (in milliseconds).
     * 
     * @return bool Returns true if the command was successfully sent and the response was positive, otherwise false.
     */
    bool setLoRaWANRX1Delay(const uint32_t value);

    /**
     * @brief Sets the LoRaWAN RX2 Delay on the XBee LR module.
     * 
     * This function sends the AT_D2 command to set the RX2 delay 
     * (in milliseconds) on the XBee LR module.
     * 
     * @param[in] value The RX2 Delay value to be set (in milliseconds).
     * 
     * @return bool Returns true if the command was successfully sent and the response was positive, otherwise false.
     */
    bool setLoRaWANRX2Delay(const uint32_t value);

    /**
     * @brief Sets the LoRaWAN RX2 Data Rate on the XBee LR module.
     * 
     * This function sends the AT_XD command to set the RX2 Data Rate 
     * on the XBee LR module.
     * 
     * @param[in] value The RX2 Data Rate to be set.
     * 
     * @return bool Returns true if the command was successfully sent and the response was positive, otherwise false.
     */
    bool setLoRaWANRX2DataRate(const uint8_t value);

    /**
     * @brief Sets the LoRaWAN RX2 Frequency on the XBee LR module.
     * 
     * This function sends the AT_XF command to set the RX2 Frequency 
     * on the XBee LR module.
     * 
     * @param[in] value The RX2 Frequency to be set (in Hz).
     * 
     * @return bool Returns true if the command was successfully sent and the response was positive, otherwise false.
     */
    bool setLoRaWANRX2Frequency(const uint32_t value);

    /**
     * @brief Sets the LoRaWAN Transmit Power on the XBee LR module.
     * 
     * This function sends the AT_PO command to set the transmit power 
     * level on the XBee LR module.
     * 
     * @param[in] value The Transmit Power value to be set (in dBm).
     * 
     * @return bool Returns true if the command was successfully sent and the response was positive, otherwise false.
     */
    bool setLoRaWANTransmitPower(const uint8_t value);

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