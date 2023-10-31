#include <Servo.h>

const int analogPin = A1;
const int emergencyPin = 6;
const int startPin = 10;
Servo esc;
unsigned long previousMillis = 0;
const long interval = 50;

enum MotorState {
  STOPPED,
  RUNNING,
  EMERGENCY_STOP
};

MotorState currentMotorState = STOPPED;

void changeMotorState(MotorState newState) {
  if (newState != currentMotorState) {
    switch (newState) {
      case STOPPED:
        esc.writeMicroseconds(1000);
        Serial.println("Motor stopped.");
        break;
      case RUNNING:
        Serial.println("Motor running.");
        break;
      case EMERGENCY_STOP:
        esc.writeMicroseconds(1000);
        Serial.println("Emergency Stop Activated!");
        break;
    }
    currentMotorState = newState;
  }
}

void setup() {
  Serial.begin(9600);
  esc.attach(9);
  esc.writeMicroseconds(1000);

  pinMode(emergencyPin, INPUT_PULLUP);
  pinMode(startPin, INPUT_PULLUP);
}

void loop() {
  unsigned long currentMillis = millis();

  if (digitalRead(emergencyPin) == LOW) {
    changeMotorState(EMERGENCY_STOP);
  } else if (digitalRead(startPin) == LOW && currentMotorState != RUNNING) {
    changeMotorState(RUNNING);
  }

  if (currentMotorState == EMERGENCY_STOP) {
    return;
  }

  if (currentMillis - previousMillis >= interval && currentMotorState == RUNNING) {
    previousMillis = currentMillis;

    int analogValue = analogRead(analogPin);
    float voltage = analogValue * (5.0 / 1023.0);

    Serial.print("Analog reading = ");
    Serial.print(analogValue);
    Serial.print("\t Voltage = ");
    Serial.print(voltage);
    Serial.println("V");

    int throttlePosition = map(analogValue, 0, 1023, 1000, 2000);
    esc.writeMicroseconds(throttlePosition);
  }
}
