# Joystick Controlled Plant Spraying Robot

Final project for ECE 314: Elements of Robotics at Miami University (Spring 2024).

## Project Overview
An interactive robotics system designed to detect green plant targets using computer vision and target them with a pan-tilt sprayer nozzle, while allowing full chassis mobility via a custom analog joystick IR remote.

## Hardware Components
- Raspberry Pi 5
- 2x Arduino Uno microcontrollers
- 2x Servo motors (Sprayer pan/tilt)
- Sprayer Assembly (12V/5V DC Pump, Tubing, Nozzle, Water reservoir)
- 2-Axis Analog Joystick module
- IR Transmitter and TSOP IR Receiver
- DC Motors & Motor Driver (differential drive base)
- External power supply & 9V battery
- USB Webcam

## System Architecture & Code Files
- **`sprayerG.py`**: Python script running on Raspberry Pi 5. Captures camera frames, detects green objects using OpenCV, filters bounding boxes, and sends X/Y coordinates over serial.
- **`Sprayer.ino`**: Arduino code receiving X/Y coordinates from the Raspberry Pi over serial to actuate pan/tilt servo motors and trigger spraying.
- **`MainRobotCode.ino`**: Arduino code running on the mobile chassis. Decodes IR signals from the remote and drives motors with calibrated PWM values.
- **`Remote.ino`**: Arduino code reading analog X/Y values from the joystick and transmitting IR signals across multiple speed/directional cases.