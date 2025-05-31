# Integrated Smart Energy Meter with Home Automation

A **real-time, IoT-enabled prepaid smart energy meter** with live cloud monitoring and home automation capabilities. Measure and control your home's power usage from anywhere via LCD and Adafruit IO app!

---

## Features

- **Real-time monitoring:** Display voltage, current, power, and energy on local LCD.
- **Cloud dashboard:** Sends live updates to [Adafruit IO](https://io.adafruit.com/) for remote monitoring on mobile/PC.
- **Prepaid metering:** Deducts energy balance automatically as you use power.
- **Automatic supply cut-off:** Relay disconnects load when balance reaches zero.
- **Home automation:** Remotely turn appliances ON/OFF via Adafruit IO or serial commands.
- **Low balance alerts:** Visual warnings and relay control for safe operation.
- **DIY-friendly:** Open hardware, full schematic, and code provided.

---

## Hardware Overview

![System Hardware Schematic](images/system_schematic.png)
> *Replace this path with your actual schematic/circuit image in your repo!*

**Core Components:**
- ESP8266 or ESP32 MCU (WiFi-enabled)
- ACS712 Current Sensor
- Voltage divider (for AC voltage sensing)
- 16x2 LCD (parallel interface)
- Relay module
- Optocoupler (for pulse counting from meter)
- (Optional) Proteus simulation files for testing

---

## Cloud Dashboard

View live energy consumption, prepaid balance, and control appliances via **Adafruit IO**:

![Adafruit IO Dashboard Screenshot](images/adafruit_dashboard.png)
> *Replace with a screenshot of your actual Adafruit IO dashboard!*

---

##  How It Works

1. **Energy Monitoring:**  
   The microcontroller continuously reads current and voltage, computes power/energy, and displays them locally.

2. **Prepaid System:**  
   User’s prepaid balance (in ₹) is stored in EEPROM. Each kWh used deducts from balance; supply is cut off when depleted.

3. **Cloud Sync:**  
   ESP8266/ESP32 pushes live data (power, units, balance) to Adafruit IO using MQTT/HTTP.

4. **Remote Automation:**  
   Control relays (fan, light, etc.) using Adafruit IO dashboard buttons, or locally via serial commands (`A`, `B`, `C`, `D`).

---

## Circuit Diagram

![image](https://github.com/user-attachments/assets/d58625c7-f9ef-4ea7-8f5a-054fff0587d0)


---

## Quantity of Materials

| Component         | Specification             | Quantity |
|-------------------|--------------------------|----------|
| ESP8266/ESP32     | NodeMCU/Wemos D1 Mini    | 1        |
| ACS712            | 5A/20A/30A Module        | 1        |
| Relay Module      | 5V, single-channel       | 1        |
| 16x2 LCD          | HD44780-compatible       | 1        |
| Voltage Divider   | (per schematic)          | 1        |
| Optocoupler       | e.g., PC817              | 1        |
| Misc              | Resistors, wires, PCB    | as reqd  |

---

## Setup & Installation

### 1. **Clone this Repository**
```sh
git clone https://github.com/PRAYAG2000n/Smart-energy-meter-with-home-automation.git
cd Smart-energy-meter-with-home-automation
```
### 2. Install Arduino Libraries
Adafruit IO Arduino
LiquidCrystal
Install via Arduino IDE Library Manager (Sketch → Include Library → Manage Libraries).

### 3. Flash the Code
Select your board (ESP8266/ESP32) and upload the code.

### 4. Connect Hardware
Wire up as per schematic, including sensors, LCD, relay, and opto input.

### 5. Set up Adafruit IO
Sign up and create feeds named power, units, and balance.

## Usage
- Live data: Shown on LCD and Adafruit IO dashboard.
- Control appliances via Adafruit IO dashboard switches or serial: Send A (LED ON), B (LED OFF), C (Fan ON), D (Fan OFF)
- Automatic cut-off: Power supply cuts off at zero balance (prepaid model).
- Recharge: Update EEPROM balance as needed for demo/testing.

## Images of smart energy meter with home automation (Both software and hardware circuits)
![image](https://github.com/user-attachments/assets/249d0f12-0d18-4a77-b0df-c6e34adb5b17)
![image](https://github.com/user-attachments/assets/ed8cded9-e101-4d37-82dc-6653429426f2)
![image](https://github.com/user-attachments/assets/d5ec1c08-9b09-4f39-a707-79da8cf8b88a)
![image](https://github.com/user-attachments/assets/9cc151f2-4a00-44ab-8099-54b0b6542504)


## Documentation
See Smart energy meter with home automation report.docx for theory, calibration, results, and design notes.



