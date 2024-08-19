/**
 * @file XBeeArduino.cpp
 * @brief Implementation of the XBeeArduino class for Arduino-compatible XBee modules.
 * 
 * This file contains the implementation of the XBeeArduino class, which provides an interface 
 * for interacting with both standard and LoRaWAN XBee modules on an Arduino platform.
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

#include "XBeeArduino.h"
#include "port_arduino.h"

XBeeArduino::XBeeArduino(HardwareSerial* serialPort, uint32_t baudrate, XBeeModuleType moduleType, void (*onReceiveCallback)(XBeeLRPacket_t*))
    : serialPort_(serialPort), moduleType_(moduleType), xbee_(nullptr),baudRate_(baudrate) {

    if (moduleType_ == XBEE_STANDARD) {
    } else if (moduleType_ == XBEE_LORA) {
        ctable_ = {
            .OnReceiveCallback = onReceiveCallback, 
            .OnSendCallback = NULL, //can be left as all NULL if no callbacks needed
        };
        htable_ = {
            .PortUartRead = portUartRead,
            .PortUartWrite = portUartWrite,
            .PortMillis = portMillis,
            .PortFlushRx = portFlushRx,
            .PortUartInit = portUartInit,
            .PortDelay = portDelay,
        };
        xbee_ = XBeeLRCreate(&ctable_,&htable_);
    }
}

XBeeArduino::~XBeeArduino() {
    if (xbee_ != nullptr) {
        xbeeDisconnect(xbee_);
        delete xbee_;  // Free the memory allocated for the XBee structure
    }
}

bool XBeeArduino::begin() {
    return portUartInit(baudrate, serialPort_) == 0; 
}

bool XBeeArduino::connect() {
    return XbeeConnect(xbee_) == 0;
    if (xbee_ != nullptr) {
        return XbeeConnect(xbee_) == 0;
    }
    return false;
}

bool XBeeArduino::disconnect() {
    if (xbee_ != nullptr) {
        return xbeeDisconnect(xbee_) == 0;
    }
}

bool XBeeArduino::sendData(const uint8_t* data, size_t length) {
    if (moduleType_ == XBEE_STANDARD) {
        return xbeeSendData(xbee_, data, length) == 0;
    } else if (moduleType_ == XBEE_LORA) {
        return xbeeLRSendData(xbee_, data, length) == 0;
    }
    return false;
}

bool XBeeArduino::isConnected() {
    if (xbee_ != nullptr) {
        return XbeeConnected(xbee_) ;
    }
    return false;
}

void XBeeArduino::reset() {
    if (moduleType_ == XBEE_STANDARD) {
        xbeeHardReset(xbee_);
    } else if (moduleType_ == XBEE_LORA) {
        xbeeLRHardReset(xbee_);
    }
}

bool XBeeArduino::setLoRaApiOptions(uint8_t options) {
    if (moduleType_ == XBEE_LORA) {
        return xbeeLRSetApiOptions(xbee_, options) == 0;
    }
    return false;
}

bool XBeeArduino::getLoRaDevEUI(uint8_t* devEUI) {
    if (moduleType_ == XBEE_LORA) {
        return xbeeLRGetDevEUI(xbee_, devEUI) == 0;
    }
    return false;
}
