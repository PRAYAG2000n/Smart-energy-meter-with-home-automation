# Integrated Smart Energy Meter with Home Automation

A dual-purpose IoT project that measures household energy consumption in real time **and** allows remote control of appliances. Energy readings are displayed locally on a 16×2 LCD and published to an Adafruit IO dashboard. Relay drivers provide basic home automation control.

---

## Project Features

- Real-time energy consumption measurement
- Local LCD display for instant readings
- Cloud connectivity via Adafruit IO
- Prepaid energy balance management and automatic relay cut-off
- Remote appliance control through serial commands and Adafruit IO app

## Components Used

- ESP8266 / ESP32 (Wi-Fi-enabled microcontroller)
- ACS712 Current sensor
- Voltage divider network
- 16×2 LCD display
- Relay module
- Optocoupler (for pulse counting)
- Adafruit IO for cloud data visualisation

## Usage

- Appliance control is possible through serial commands ('A', 'B', 'C', 'D') or via the Adafruit IO dashboard.
- Energy usage is monitored in real-time, and power supply is automatically cut off when the prepaid balance reaches zero.
- The current balance and units are displayed both locally and on the mobile app.

## Libraries
- Adafruit IO Arduino for Adafruit IO cloud integration (https://github.com/adafruit/Adafruit_IO_Arduino)
- Liquid crystal for LCD display (https://github.com/arduino-libraries/LiquidCrystal)
- Adafruit MQTT Library for MQTT protocol where the energy meter publishes live power/unit/balance readings to Adafruit IO using MQTT, which is more efficient and reliable for IoT data streams than regular HTTP. (https://github.com/adafruit/Adafruit_MQTT_Library/blob/master/examples/mqtt_esp8266/mqtt_esp8266.ino)
- Arduino JSON for sending (or receiving) data to/from Adafruit IO or other cloud services, values are packaged into JSON strings. This lets the cloud service and your device “understand” each other—regardless of programming language or platform.

## Software and hardware model of smart energy meter with home automation
![image](https://github.com/user-attachments/assets/bfa4ac20-e0a8-48fe-9258-c365f394d2fb)

![image](https://github.com/user-attachments/assets/4841a30d-0ed5-4031-971e-855b41fb078e)

![image](https://github.com/user-attachments/assets/5d6cafdb-9de7-40a8-96b2-de697e258f4c)

![image](https://github.com/user-attachments/assets/bb55e4e9-d294-4c65-84c3-155a1f6503f9)


## Project Report and Documentation

The information regarding this project is included in the word document.

