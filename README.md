# Vehicle Monitoring & Accident Detection System
An IoT-based accident detection and emergency alerting system using **Arduino**, **MPU6050**, **Vibration Sensors**, **GPS**, **GSM**, and **NodeMCU** for cloud integration.  
The system detects vehicle crashes, determines location, alerts emergency contacts via SMS, and sends the crash data to a cloud server.

---

# Overview
Road accidents are a leading cause of injury and death globally. Timely identification and reporting of accidents is crucial in reducing response times and saving lives.

This project provides an automated system that:
- Detects vehicle accidents with high accuracy  
- Sends the GPS location via SMS using a GSM module  
- Uploads the accident information to a cloud server  
- Allows the driver to cancel false triggers  
- Offers a real-time orientation visualizer for the MPU6050 sensor  

The goal is to create a low-cost, real-time, scalable accident-detection solution.

---

# Features
- Real-time accident detection using **MPU6050** (gyroscope + accelerometer)
- Additional vibration sensor support (801S)
- Live GPS tracking using SIM28ML
- Automated SMS alerts via GSM SIM900
- Cloud logging through NodeMCU (ESP8266)
- User override to cancel false alarms
- Buzzer alert for crash confirmation
- MPU6050 live orientation visualizer in Processing
- Modular and easy-to-extend architecture

---

#  Architecture

### **High-Level Architecture**
     +-------------------+
     |   MPU6050 Sensor  |
     +-------------------+
              \
               \
     +-------------------+
     |  801S Vibration   |
     |      Sensor       |
     +-------------------+
              \
               →  (Motion + Shock Data)
                 ┌──────────────────────────┐
                 │       Arduino UNO        │
                 │  Accident Classification │
                 └──────────────────────────┘
                  /            |             \
                 /             |              \
    (Override Button)     (GSM SMS Alerts)    (Cloud Data)
                |              |                |
         +------------+   +----------+   +------------------+
         |  Buzzer    |   | GSM900   |   |   NodeMCU ESP8266 |
         +------------+   +----------+   +------------------+


### **Cloud Workflow**
Arduino → NodeMCU → HTTP/HTTPS → Cloud Server → Dashboard / Stakeholders


---

# Hardware Components

| Component | Purpose |
|----------|---------|
| **Arduino Uno** | Central controller for sensors & logic |
| **MPU6050** | 3-axis accelerometer + 3-axis gyroscope |
| **801S Vibration Sensor** | Shock detection during collision |
| **GPS Module (SIM28ML)** | Provides latitude & longitude |
| **GSM Module (SIM900)** | Sends SMS alerts to emergency contacts |
| **NodeMCU ESP8266** | Sends crash data to cloud |
| **Buzzer** | Audible alarm after potential crash |
| **Push Button** | Allows user confirmation (“I’m safe”) |

---

# System Workflow

## **1. Continuous Monitoring**
The Arduino continuously reads:
- X, Y, Z accelerometer/gyroscope data from MPU6050  
- Shock data from vibration sensor  
- GPS coordinates  
- User override button state  

---

## **2. Threshold-Based Accident Detection**
An accident is detected if:
x >= 700 AND
y >= 700 AND
z >= 700


These values can be calibrated depending on vehicle type and sensor positioning.

---

## **3. Safety Confirmation Window**
After a potential crash, the buzzer sounds for **3 seconds**, and the user is given:
≈ 2.5 seconds (5 cycles × 500ms)
to press the safety button.

### -> If the button IS pressed:
- Accident dismissed  
- Cloud updated with `condition = 0`  

### -> If NOT pressed:
- Accident confirmed  
- GSM sends SMS to emergency contacts  
- NodeMCU uploads to cloud with `condition = 1`

---

## **4. Cloud & SMS Notification**
### SMS Format:

#accident occurred at lat=<lat> lon=<lng>*

### Cloud Format:
$&field3=<status>&field1=<lat>&field2=<lng>*

---

# Accident Detection Logic

### Logic Summary
if (high acceleration + vibration) → potential accident
if (override switch HIGH during countdown) → false alarm
else → confirmed accident

### Why use both MPU6050 + Vibration Sensor?
- MPU6050 detects motion/orientation changes  
- Vibration sensor detects sudden impacts  
- Combined → highly reliable crash detection  

---

#  Data Formats

### **Cloud Update Packet**
$&field3=1&field1=12.971600&field2=77.594600*

### **Emergency SMS**
#accident occurred at lat=12.971600 and lon=77.594600*

---

#  Circuit Description

### **1. MPU6050 → Arduino**
VCC → 5V
GND → GND
SCL → A5
SDA → A4
INT → D2

### **2. Vibration Sensor → Arduino**
OUT → A0 / A1 / A2 (depends on configuration)

### **3. GPS SIM28ML → Arduino**

| GPS Pin | Arduino Pin |
|--------|--------------|
| TX | D9 (SoftwareSerial) |
| RX | D10 |

### **4. GSM SIM900 → Arduino**

SIM900 TX → D7
SIM900 RX → D8

### **5. Button & Buzzer**
Button → D3
Buzzer → D11

---

#  Software Requirements

### **Arduino**
- Arduino IDE  
- TinyGPS++  
- SoftwareSerial  
- Wire.h  
- MPU6050 DMP library (optional)

### **Processing**
- Processing 3.x or 4.x  
- Toxiclibs library  
- processing.serial library  

### **Cloud**
- NodeMCU flashed with ESP8266 firmware  
- WiFi network  
- Backend or Thingspeak-style endpoint  

---

