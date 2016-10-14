#include "Arduino.h"
#include "ArmController.h"

ArmController::ArmController() {

}

ArmController::setup() {
  lowerLimitSwitchPin = 5;
  upperLimitSwitchPin = 6;

  pinMode(lowerLimitSwitchPin, INPUT_PULLUP);
  pinMode(upperLimitSwitchPin, INPUT_PULLUP);
  armServo.attach(7);
  graberServo.attach(8);
  transPlaneServo.attach(9);
}


void ArmController::dropDown() {
  armServo.write(100);
}

void ArmController::liftUp() {
  armServo.write(70);
}

bool ArmController::isLiftedUp() {
  return armServo.read() == 70;
}

bool ArmController::isDropDownFinish() {
  return digitalRead(lowerLimitSwitchPin) == LOW;
}

void ArmController::stablize() {
  armServo.write(82);
}

void ArmController::grab() {
  graberServo.write(180);
}

bool ArmController::isGrabFinished() {
  return graberServo.read() == 180;
}

bool ArmController::isReleaseFinished() {
  return graberServo.read() == 20;
}

void ArmController::release() {
  graberServo.write(56);
}

void ArmController::rotateDownGraber() {
  graberServo.write(0);
}

