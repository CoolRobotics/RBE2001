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

bool armStatus = true;
void ArmController::dropDown() {
  armServo.write(98);
  delay(2000);
  armStatus = false;
}

void ArmController::liftUp() {
  armServo.write(70);
  delay(2000);
  armStatus = true;
}

bool ArmController::isLiftedUp() {
  if (digitalRead(upperLimitSwitchPin) == LOW) {
    armStatus = true;
    return true;
  }
  return false;
}

bool ArmController::isDropDownFinish() {
  if (digitalRead(lowerLimitSwitchPin) == LOW) {
    armStatus = false;
    return true;
  }
  return false;
}
