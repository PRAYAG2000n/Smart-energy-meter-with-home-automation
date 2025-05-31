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

## Project Report and Documentation

The detailed project report is included in this repository for reference.

