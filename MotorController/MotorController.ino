#include <Servo.h>

const int analogPin = A1;
const int stopPin = 6;
const int startPin = 10;
Servo esc;
unsigned long previousMillis = 0;
const long interval = 50;

enum MotorState {
  STOPPED,
  RUNNING
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
    }
    currentMotorState = newState;
  }
}

void setup() {
  Serial.begin(9600);
  esc.attach(9);
  esc.writeMicroseconds(1000);

  pinMode(stopPin, INPUT_PULLUP);
  pinMode(startPin, INPUT_PULLUP);
}

void loop() {
  unsigned long currentMillis = millis();

  if (digitalRead(stopPin) == LOW) {
    changeMotorState(STOPPED);
  } else if (digitalRead(startPin) == LOW && currentMotorState != RUNNING) {
    changeMotorState(RUNNING);
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
