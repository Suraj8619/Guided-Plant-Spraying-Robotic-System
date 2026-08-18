#include <Servo.h>

Servo panServo;
Servo tiltServo;

const int PIN_PAN_SERVO  = 9;
const int PIN_TILT_SERVO = 10;
const int PIN_PUMP       = 7;

// Frame boundaries matching the vision pipeline
const int FRAME_WIDTH  = 640;
const int FRAME_HEIGHT = 480;

// Servo limits (degrees)
const int PAN_MIN = 30;
const int PAN_MAX = 150;
const int TILT_MIN = 45;
const int TILT_MAX = 135;

void setup() {
  Serial.begin(9600);
  
  panServo.attach(PIN_PAN_SERVO);
  tiltServo.attach(PIN_TILT_SERVO);
  
  pinMode(PIN_PUMP, OUTPUT);
  digitalWrite(PIN_PUMP, LOW);

  // Home position
  panServo.write(90);
  tiltServo.write(90);
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.startsWith("X") && input.indexOf("Y") > 0) {
      int yIndex = input.indexOf("Y");
      int xVal = input.substring(1, yIndex).toInt();
      int yVal = input.substring(yIndex + 1).toInt();

      // Convert coordinates to servo angles
      int panAngle = map(xVal, 0, FRAME_WIDTH, PAN_MAX, PAN_MIN);
      int tiltAngle = map(yVal, 0, FRAME_HEIGHT, TILT_MIN, TILT_MAX);

      panAngle = constrain(panAngle, PAN_MIN, PAN_MAX);
      tiltAngle = constrain(tiltAngle, TILT_MIN, TILT_MAX);

      // Actuate servos
      panServo.write(panAngle);
      tiltServo.write(tiltAngle);

      // Actuate spraying function
      digitalWrite(PIN_PUMP, HIGH);
      delay(300);
      digitalWrite(PIN_PUMP, LOW);
    }
  }
}