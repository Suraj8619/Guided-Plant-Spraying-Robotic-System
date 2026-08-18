#include <IRremote.h>

const int PIN_JOY_X = A0;
const int PIN_JOY_Y = A1;
const int PIN_IR_SEND = 3;

// Calibration & Thresholds
const int CENTER_MIN = 470;
const int CENTER_MAX = 550;

void setup() {
  IrSender.begin(PIN_IR_SEND);
}

void loop() {
  int xVal = analogRead(PIN_JOY_X);
  int yVal = analogRead(PIN_JOY_Y);

  unsigned long commandToSend = 0x00; // Stop default

  // Center deadband check
  if (xVal >= CENTER_MIN && xVal <= CENTER_MAX && yVal >= CENTER_MIN && yVal <= CENTER_MAX) {
    commandToSend = 0x00; // Stop
  }
  // Forward motion states (varying speeds based on deflection)
  else if (yVal > CENTER_MAX && abs(xVal - 512) < 150) {
    if (yVal < 700) {
      commandToSend = 0x10; // Slow
    } else if (yVal < 900) {
      commandToSend = 0x11; // Medium
    } else {
      commandToSend = 0x12; // Fast
    }
  }
  // Reverse motion state
  else if (yVal < CENTER_MIN && abs(xVal - 512) < 150) {
    commandToSend = 0x20;
  }
  // Left turn state
  else if (xVal < CENTER_MIN) {
    commandToSend = 0x30;
  }
  // Right turn state
  else if (xVal > CENTER_MAX) {
    commandToSend = 0x31;
  }

  // Send the IR command via standard protocol
  IrSender.sendNEC(0x00FF, commandToSend, 0);
  delay(100);
}