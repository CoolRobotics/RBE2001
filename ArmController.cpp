#include "Arduino.h"
#include "ArmController.h"

ArmController::ArmController() {
}

void ArmController::setup() {
  lowerLimitSwitchPin = 5;
  upperLimitSwitchPin = 6;

  pinMode(lowerLimitSwitchPin, INPUT_PULLUP);
  pinMode(upperLimitSwitchPin, INPUT_PULLUP);
  armServo.attach(7);
}

void ArmController::liftUp() {
  armServo.write(70);
  delay(1000);
}

bool ArmController::isLiftedUp() {
  return digitalRead(upperLimitSwitchPin) == LOW;
}

void ArmController::dropDown() {
  armServo.write(98);
  delay(1000);
}

bool ArmController::isDropDownFinish() {
  return digitalRead(lowerLimitSwitchPin) == LOW;
}

void ArmController::dropDownAtReactor() {
  armServo.write(100);
  delay(1000);
}

bool ArmController::isDropDownAtReactorDone() {
  return  armServo.read() == 100;
}

void ArmController::dropDownMoreAtReactor() {
  armServo.write(100);
  delay(1000);
}

bool ArmController::isDropDownMoreAtReactorDone() {
  return  armServo.read() == 100
  ;
}
