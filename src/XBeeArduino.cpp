#include "XBeeArduino.h"
#include "xbee.h"
#include "port.h"

/**
 * @file XBeeArduino.cpp
 * @brief Implementation of the XBeeArduino class for Arduino-compatible XBee modules.
 * 
 * This file contains the implementation of the XBeeArduino class, which provides an interface 
 * for interacting with XBee modules on an Arduino platform.
 * 
 * @version 1.0
 * @date 2024-08-17
 * author Felix Galindo
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

XBeeArduino* XBeeArduino::instance_ = nullptr;

/**
 * @brief Constructor for XBeeArduino class.
 * 
 * Initializes the XBee module based on the type and sets up callbacks and UART communication.
 * 
 * @param serialPort A pointer to a Stream object (HardwareSerial or SoftwareSerial).
 * @param baudrate The baud rate for UART communication.
 * @param moduleType The type of XBee module (standard or LoRa).
 * @param onReceiveCallback A callback function to handle received data.
 * @param onSendCallback A callback function to handle post-send events.
 */
XBeeArduino::XBeeArduino(Stream* serialPort, uint32_t baudrate, XBeeModuleType moduleType,
                         void (*onReceiveCallback)(void*),
                         void (*onSendCallback)(void*))
    : serialPort_(serialPort), moduleType_(moduleType), xbee_(nullptr), baudRate_(baudrate),
      onReceiveCallback_(onReceiveCallback), onSendCallback_(onSendCallback) {

    if (moduleType_ == XBEE_STANDARD) {
        // Initialization for standard XBee modules
    } else if (moduleType_ == XBEE_LORA) {
        instance_ = this;
        ctable_.OnReceiveCallback = onReceiveWrapper, 
        ctable_.OnSendCallback = onSendWrapper, 
        htable_.PortUartRead = portUartRead,
        htable_.PortUartWrite = portUartWrite,
        htable_.PortMillis = portMillis,
        htable_.PortFlushRx = portFlushRx,
        htable_.PortUartInit = portUartInit,
        htable_.PortDelay = portDelay,

        xbee_ = (XBee*)XBeeLRCreate(&ctable_, &htable_);
    }
}

/**
 * @brief Destructor for XBeeArduino class.
 * 
 * Ensures proper disconnection and cleanup of the XBee module.
 */
XBeeArduino::~XBeeArduino() {
    if (xbee_ != nullptr) {
        XBeeDisconnect(xbee_);
        delete xbee_;  // Free the memory allocated for the XBee structure
    }
}

/**
 * @brief Initializes the XBee module.
 * @return True if initialization is successful, otherwise false.
 */
bool XBeeArduino::begin() {
    return portUartInit(baudRate_, serialPort_) == 0;  // Corrected baudRate_ reference
}

/**
 * @brief Connects the XBee module to the network.
 * @return True if the connection is successful, otherwise false.
 */
bool XBeeArduino::connect() {
    if (xbee_ != nullptr) {
        return XBeeConnect(xbee_);
    }
    return false;
}

/**
 * @brief Lets the XBee class process 
 * @return void
 */
void XBeeArduino::process() {
    if (xbee_ != nullptr) {
        XBeeProcess(xbee_);
    }
}

/**
 * @brief Disconnects the XBee module from the network.
 * @return True if disconnection is successful, otherwise false.
 */
bool XBeeArduino::disconnect() {
    if (xbee_ != nullptr) {
        return XBeeDisconnect(xbee_);
    }
    return false;  // Added a return statement here
}

/**
 * @brief Sends data through the XBee module.
 * @param data The data to be sent.
 * @return True if the data is sent successfully, otherwise false.
 */
template <typename T>
bool XBeeArduino::sendData(const T& data) {
    // if (moduleType_ == XBEE_STANDARD) {
    //return XBeeSendData(xbee_, data, length) == 0;
    // } else 
    if (moduleType_ == XBEE_LORA) {
        return XBeeSendData(xbee_, &data) == 0;
    }
    //return false;
}

// Explicit template instantiation for XBeeLRPacket_s
template bool XBeeArduino::sendData<XBeeLRPacket_s>(const XBeeLRPacket_s&);

/**
 * @brief Checks if the XBee module is connected to the network.
 * @return True if the module is connected, otherwise false.
 */
bool XBeeArduino::isConnected() {
    if (xbee_ != nullptr) {
        return XBeeConnected(xbee_);
    }
    return false;
}

/**
 * @brief Resets the XBee module.
 */
void XBeeArduino::reset() {
    if (xbee_ != nullptr) {
        return XBeeHardReset(xbee_);
    }
}

/**
 * @brief Sets the API options for LoRaWAN communication.
 * @param options The API options to set.
 * @return True if the options are set successfully, otherwise false.
 */
bool XBeeArduino::setLoRaApiOptions(const uint8_t options) {
    if (moduleType_ == XBEE_LORA) {
        return XBeeSetAPIOptions(xbee_, options);
    }
    return false;
}

/**
 * @brief Retrieves the DevEUI of the LoRaWAN XBee module.
 * @param devEUI A pointer to a buffer where the DevEUI will be stored.
 * @return True if the DevEUI is retrieved successfully, otherwise false.
 */
bool XBeeArduino::getLoRaDevEUI(uint8_t* devEUI, uint8_t length) {
    if (moduleType_ == XBEE_LORA) {
        return XBeeLRGetDevEUI(xbee_, devEUI, length);
    }
    return false;
}

void XBeeArduino::onReceiveWrapper(XBee* xbee, void* data) {
    if (instance_ && instance_->onReceiveCallback_) {
        instance_->onReceiveCallback_(data);  // Forward the call to the original callback
    }
}

void XBeeArduino::onSendWrapper(XBee* xbee, void* data) {
    if (instance_ && instance_->onSendCallback_) {
        instance_->onSendCallback_(data);  // Forward the call to the original callback
    }
}