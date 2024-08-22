
# XBeeArduino

`XBeeArduino` is an Arduino-friendly C++ wrapper for the [xbee_c_library](https://github.com/felixgalindo/xbee_c_library), providing a simplified interface for integrating XBee modules into Arduino projects.

## Features
- Easy-to-use C++ interface for XBee modules
- Compatible with Arduino platforms
- Uses the `xbee_c_library` as a submodule

## Installation
1. Clone the repository with its submodule:

   ```sh
   git clone --recurse-submodules https://github.com/your-username/XBeeArduino.git
   ```

2. Include the `XBeeArduino` header and source files in your Arduino project.

## Example
Check out the `examples/xbee_lr/xbee_lr.ino` for a basic usage example with the XBee LR.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.


## XBeeArduino Class Methods

The `XBeeArduino` class provides several methods to interact with XBee modules:

### Public Methods

1. **`bool begin()`**: Initializes the XBee module. Returns `true` if initialization is successful.
2. **`bool connect()`**: Establishes a connection with the XBee module. Returns `true` if the connection is successful.
3. **`void process()`**: Continuously processes incoming data from the XBee module.
4. **`bool disconnect()`**: Disconnects from the XBee module. Returns `true` if the disconnection is successful.
5. **`bool sendData(const T& data)`**: Sends data through the XBee module. The data type `T` can vary depending on the use case.
6. **`bool isConnected()`**: Checks if the XBee module is currently connected. Returns `true` if connected.
7. **`void reset()`**: Resets the XBee module to its default state.
8. **`bool setLoRaApiOptions(const uint8_t options)`**: Configures specific options for LoRa communication using the provided `options`.
9. **`bool getLoRaDevEUI(uint8_t* devEUI, uint8_t length)`**: Retrieves the LoRa Device EUI into the provided `devEUI` array.

### Private Methods and Attributes

1. **Serial Port**: Manages the serial communication port, which could be either HardwareSerial or SoftwareSerial.
2. **XBee Module**: Manages the instance of the XBee module, whether it is standard or LoRa.
3. **Callbacks**:
   - **`void (*onReceiveCallback_)`**: Function pointer for a callback when data is received.
   - **`void (*onSendCallback_)`**: Function pointer for a callback when data is sent.
   - **`void onReceiveWrapper(XBee* xbee, void* data)`**: Internal method to wrap the receive callback.
   - **`void onSendWrapper(XBee* xbee, void* data)`**: Internal method to wrap the send callback.

## Expanded Example

```cpp
#include <XBeeArduino.h>

XBeeArduino xbee;

void setup() {
    Serial.begin(9600);
    if (xbee.begin()) {
        Serial.println("XBee initialized successfully.");
    } else {
        Serial.println("Failed to initialize XBee.");
    }
  
    if (xbee.connect()) {
        Serial.println("XBee connected.");
    } else {
        Serial.println("Failed to connect XBee.");
    }
}

void loop() {
    xbee.process();
  
    if (xbee.isConnected()) {
        String data = "Hello, XBee!";
        if (xbee.sendData(data)) {
            Serial.println("Data sent successfully.");
        } else {
            Serial.println("Failed to send data.");
        }
    } else {
        Serial.println("XBee not connected.");
    }
  
    delay(1000);
}
```

This example demonstrates how to initialize, connect, send data, and process incoming data with the XBeeArduino library.

## XBeeArduino Constructor

The `XBeeArduino` class constructor allows you to initialize the XBee module with either `HardwareSerial` or `SoftwareSerial`.

### Constructor Signatures

```cpp
XBeeArduino(HardwareSerial* serial, int baudRate);
XBeeArduino(SoftwareSerial* serial, int baudRate);
```

### Parameters
- **`serial`**: Pointer to either `HardwareSerial` or `SoftwareSerial`. This determines the serial interface used for communication.
- **`baudRate`**: The baud rate for serial communication, typically `9600` or `115200`.

### Example Usage

#### Using `HardwareSerial`

```cpp
#include <XBeeArduino.h>

XBeeArduino xbee(&Serial, 9600);

void setup() {
    Serial.begin(9600);
    if (xbee.begin()) {
        Serial.println("XBee initialized with HardwareSerial.");
    }
}

void loop() {
    xbee.process();
}
```

#### Using `SoftwareSerial`

```cpp
#include <XBeeArduino.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX
XBeeArduino xbee(&mySerial, 9600);

void setup() {
    mySerial.begin(9600);
    if (xbee.begin()) {
        Serial.println("XBee initialized with SoftwareSerial.");
    }
}

void loop() {
    xbee.process();
}
```

## Using XBeeArduino Library in Arduino IDE

To use the `XBeeArduino` library in the Arduino IDE:

1. **Install the Library**:
   - Download or clone the `XBeeArduino` library repository.
   - Move the library folder into your Arduino libraries directory (e.g., `Documents/Arduino/libraries/`).

2. **Include the Library in Your Sketch**:
   - Start the Arduino IDE.
   - In your sketch, include the library at the top with `#include <XBeeArduino.h>`.

3. **Compile and Upload**:
   - Connect your Arduino board.
   - Write your sketch using the `XBeeArduino` library.
   - Compile and upload your sketch to the Arduino board using the Arduino IDE.

### Example Sketch

Here’s an example of how to use the `XBeeArduino` library in an Arduino sketch:

```cpp
#include <XBeeArduino.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX
XBeeArduino xbee(&mySerial, 9600);

void setup() {
    Serial.begin(9600);
    mySerial.begin(9600);
    
    if (xbee.begin()) {
        Serial.println("XBee initialized.");
    }
}

void loop() {
    xbee.process();
    
    if (xbee.isConnected()) {
        String data = "Hello, XBee!";
        if (xbee.sendData(data)) {
            Serial.println("Data sent successfully.");
        } else {
            Serial.println("Failed to send data.");
        }
    } else {
        Serial.println("XBee not connected.");
    }

    delay(1000);
}
```

This example demonstrates setting up the XBee module, processing incoming data, and sending data using the `XBeeArduino` library in the Arduino IDE.
