#include <IRremote.h>

// Motor Driver Pins (L298N / Dual H-Bridge)
const int PIN_ENA = 5;  // PWM Left
const int PIN_IN1 = 3;  // Direction Left 1
const int PIN_IN2 = 4;  // Direction Left 2
const int PIN_ENB = 6;  // PWM Right
const int PIN_IN3 = 7;  // Direction Right 1
const int PIN_IN4 = 8;  // Direction Right 2

const int PIN_IR_RECV = 2;

// Calibrated PWM speeds to counteract chassis imbalance and drive straight
const int PWM_SLOW_L = 100;
const int PWM_SLOW_R = 108;

const int PWM_MED_L  = 175;
const int PWM_MED_R  = 190;

const int PWM_FAST_L = 240;
const int PWM_FAST_R = 255;

void setup() {
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);

  IrReceiver.begin(PIN_IR_RECV, ENABLE_LED_FEEDBACK);
  stopMotors();
}

void loop() {
  if (IrReceiver.decode()) {
    unsigned long cmd = IrReceiver.decodedIRData.command;

    switch (cmd) {
      case 0x10: // Forward Slow
        driveForward(PWM_SLOW_L, PWM_SLOW_R);
        break;
      case 0x11: // Forward Medium
        driveForward(PWM_MED_L, PWM_MED_R);
        break;
      case 0x12: // Forward Fast
        driveForward(PWM_FAST_L, PWM_FAST_R);
        break;
      case 0x20: // Backward
        driveBackward(PWM_MED_L, PWM_MED_R);
        break;
      case 0x30: // Turn Left
        turnLeft(PWM_MED_L, PWM_MED_R);
        break;
      case 0x31: // Turn Right
        turnRight(PWM_MED_L, PWM_MED_R);
        break;
      case 0x00: // Stop
      default:
        stopMotors();
        break;
    }
    IrReceiver.resume();
  }
}

void driveForward(int pwmL, int pwmR) {
  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, LOW);
  analogWrite(PIN_ENA, pwmL);
  analogWrite(PIN_ENB, pwmR);
}

void driveBackward(int pwmL, int pwmR) {
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, HIGH);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, HIGH);
  analogWrite(PIN_ENA, pwmL);
  analogWrite(PIN_ENB, pwmR);
}

void turnLeft(int pwmL, int pwmR) {
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, HIGH);
  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, LOW);
  analogWrite(PIN_ENA, pwmL);
  analogWrite(PIN_ENB, pwmR);
}

void turnRight(int pwmL, int pwmR) {
  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, HIGH);
  analogWrite(PIN_ENA, pwmL);
  analogWrite(PIN_ENB, pwmR);
}

void stopMotors() {
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, LOW);
  analogWrite(PIN_ENA, 0);
  analogWrite(PIN_ENB, 0);
}