# Stranger Things Light Wall
A Halloween project that recreates the iconic Stranger Things light wall using Arduino Nano and ESP32, allowing messages to be displayed by lighting up individual letters.
Did this for daughter who loves Stranger Things.

Attribution:
Zach from Byte Sized Engieneering LLC for the shell code and original idea.
Website: https://www.bytesizedengineering.com/
YouTube: https://www.youtube.com/@bytesizedengineering

## Overview

This project creates an interactive light wall where each letter of the alphabet is represented by a colored LED. Messages can be sent via WiFi through a web interface or Telnet, causing the corresponding letters to light up in sequence - just like Joyce Byers' Christmas lights in Stranger Things.

## Hardware Components

- **Arduino Nano** - Controls the LED strip and relay
- **ESP32 Node MCU Dev Board** - WiFi server and web interface
- **WS2811 LED Strip** - 28 LEDs for alphabet display
- **Relay Module** (optional) - For controlling mains voltage Christmas lights
- **C9 Replacement Christmas Light Bulb Covers** - Color-matched LED covers

## Wiring

### Arduino Nano to ESP32
- Arduino TX (Pin 1) → ESP32 RX (Pin 3)
- Arduino RX (Pin 3) → ESP32 TX (Pin 1)
- Common GND connection

### Arduino Nano to LEDs
- LED Data → Arduino Pin 2
- Relay Control → Arduino Pin 3

## Dependencies

### Arduino Libraries
- **FastLED** - LED strip control and color management

### ESP32 Libraries
- **WiFi.h** - WiFi connectivity
- **ESPTelnet.h** - Telnet server functionality
- **WebServer.h** - HTTP web server

## Features

### LED Control
- **Individual Letter Display** - Each ASCII letter (A-Z) maps to a specific LED
- **Color Coded LEDs** - LEDs alternate between Yellow, Orange, Red, Green, and Blue
- **Special Commands**:
  - `,` - Turn all lights on
  - `.` - Turn all lights off
  - `1` - Chase effect (lights sequence across the strip)
  - `2` - Triple flash
  - `3` - Seven flashes

### Communication Methods
1. **Web Interface** - Responsive HTML page with predefined messages and custom text input
2. **Telnet Server** - Direct command-line interface on port 23

### Predefined Messages
- Halloween greetings: "happy halloween", "trick or treat", "nice costume"
- Stranger Things themed: "have you seen eleven", "im here", "run", "hide", "its coming"
- Interactive responses: "yes", "no", "boo"

## Setup Instructions

1. **Arduino Configuration**:
   - Install FastLED library
   - Upload `ArduinoNano.ino` to Arduino Nano
   - Serial communication set to 2400 baud (reduced for breadboard stability)

2. **ESP32 Configuration**:
   - Install required libraries (WiFi, ESPTelnet, WebServer)
   - Update WiFi credentials in `ESP32.ino`:
     ```cpp
     const char* ssid = "YOUR_WIFI_SSID";
     const char* password = "YOUR_WIFI_PASSWORD";
     ```
   - Upload `ESP32.ino` to ESP32

3. **Physical Setup**:
   - Mount LEDs in alphabet order on wall (see demo video)
   - Connect hardware as per wiring diagram
   - Optionally connect relay for additional Christmas lights

## Usage

1. Power on both devices
2. Connect to the ESP32's IP address via web browser
3. Use the web interface to send messages or control lights
4. Alternatively, connect via Telnet to send commands directly

## Technical Details

- **LED Mapping**: Custom index mapping accounts for the physical layout of letters on the wall
- **Serial Communication**: 2400 baud rate prevents data corruption on breadboard setup  
- **Responsive Design**: Web interface optimized for mobile devices
- **Real-time Control**: Immediate response to web and Telnet commands

## Project Background

Built for Halloween as a Stranger Things tribute. Originally planned to use HC-05 Bluetooth module but switched to ESP32 for better WiFi connectivity and web interface capabilities. The project allows guests to interact with the light wall just like the characters in the show.

## Files

- `ArduinoNano.ino` - Arduino code for LED control
- `ESP32.ino` - ESP32 server and web interface code
- `StrangerThingsLightWall.MOV` - Demo video

