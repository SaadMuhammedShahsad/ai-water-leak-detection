# AI Water Leak Detection System

A real-time water leak detection system built with an ESP32 and a water sensor. When water is detected, it sounds a buzzer, turns on an LED and sends an alert message to a phone within about 1 second.

## Components
- ESP32
- Resistive water level sensor module
- Buzzer
- LED
- USB cable

## How it works
1. The ESP32 continuously reads the signal from the water sensor.
2. The code checks the sensor reading to decide whether water is present, which means a leak.
3. When a leak is detected, the buzzer sounds and the LED turns on.
4. The ESP32 is connected to a laptop, and an alert message is sent to a phone to report that a leak was detected.

## Result
The alert triggers in about 1 second after water is detected.

## Files
- `WATER_LEAK_AND_DETECTION_SYSTEM_PROJECT_FINALLL.ino`: Arduino/ESP32 code
