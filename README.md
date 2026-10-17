# ESP32-Based Indoor Environmental Monitoring System

A proposed embedded-systems design for monitoring indoor temperature, relative humidity, and an indicative air-quality sensor signal using ESP32, sensors, Wi-Fi communication, and local alerts.

> **Project status:** Requirements and architecture design. Physical implementation and testing are planned and will be documented when completed.

## Objectives
- Measure temperature and relative humidity using BME280.
- Read an indicative air-quality signal using an MQ-135 module.
- Validate sensor readings and detect faults.
- Provide configurable local alerts using an LED and buzzer.
- Transmit telemetry using Wi-Fi through HTTP or MQTT.
- Continue local monitoring during network outages.

## Proposed Hardware
- ESP32 development board
- BME280 temperature and humidity sensor
- MQ-135 air-quality sensor module
- LED and current-limiting resistor
- Buzzer and suitable driver if required
- USB power supply, wiring, and enclosure

## System Architecture

![ESP32 Indoor Environmental Monitoring Block Diagram](Indoor_Environmental_Monitor_Block_Diagram.png)

## Week 1 Requirements and Design Report

[Open the Week 1 Technical Report](Week1_Embedded_System_Requirements_Report.docx)

## Working Principle
1. Sensors collect environmental readings.
2. ESP32 firmware validates and processes the readings.
3. Threshold logic determines the device status.
4. LED and buzzer provide local alerts.
5. Wi-Fi transmits readings to a dashboard or server.
6. Local monitoring continues if Wi-Fi is unavailable.

## Safety and Measurement Limitations
- MQ-135 readings are indicative unless properly calibrated.
- This project is not a certified smoke or carbon-monoxide alarm.
- Verify sensor voltage compatibility before connecting modules to the ESP32.
- Record actual measurements and test results only after performing the tests.

## Verification Plan
- Verify sensor initialization and readings.
- Compare temperature and humidity with a reference instrument.
- Test threshold alerts and sensor-fault handling.
- Disconnect and restore Wi-Fi to test reconnection.
- Document actual results, limitations, and test conditions.

## Future Development
- Implement ESP32 firmware.
- Integrate the selected sensors and local alerts.
- Develop a dashboard for live readings and historical data.
- Add test evidence and wiring documentation.

## Author
Prajakta Jadhav

Embedded Systems Engineer Intern

## License
A license can be added when the project’s distribution terms are decided.
