#include "Arduino.h"
#include "GrabberController.h"
void GrabberController::setup() {
  grabberServo.attach(8);
  transPlaneServo.attach(9);
}

void GrabberController::grab() {
  grabberServo.write(180);
  delay(500);
}

bool GrabberController::isGrabFinished() {
  return grabberServo.read() == 180;
}

bool GrabberController::isReleaseFinished() {
  return grabberServo.read() == 20;
}

void GrabberController::release() {
  grabberServo.write(20);
  delay(1000);
}

void GrabberController::rotateDownGrabber() {
  transPlaneServo.write(91);
  delay(1000);
}

bool GrabberController::isRotateDownGrabberDone() {
  return transPlaneServo.read() == 91;
}

void GrabberController::rotateDownGrabberAtReactor() {
  transPlaneServo.write(96);
  delay(1000);
}

bool GrabberController::isRotateDownGrabberAtReactorDone() {
  return transPlaneServo.read() == 96;
}


void GrabberController::rotateUpGrabber() {
  transPlaneServo.write(164);
  delay(1000);
}

bool GrabberController::isRotateUpGrabberDone() {
  return transPlaneServo.read() == 164;
}

