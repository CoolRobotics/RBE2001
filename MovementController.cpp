#include <Servo.h>
#include "MovementController.h"

Encoder leftEncoder(2, 3);

MovementController::MovementController() {
}

void MovementController::setup() {

  oldDegree = 0;

  // left side
  leftMostLineTrackerPin = 29;
  leftMoreLineTrackerPin = 28;
  leftLineTrackerPin = 27;
  leftCrossLineTrackerPin = 23;

  // right side
  rightMostLineTrackerPin = 24;
  rightMoreLineTrackerPin = 25;
  rightLineTrackerPin = 26;
  rightCrossLineTrackerPin = 22;

  // limit switch
  frontLimitSwitchPin = 4;

  // left Side
  pinMode(leftMostLineTrackerPin, INPUT_PULLUP);
  pinMode(leftMoreLineTrackerPin, INPUT_PULLUP);
  pinMode(leftLineTrackerPin, INPUT_PULLUP);
  pinMode(leftCrossLineTrackerPin, INPUT_PULLUP);

  // right side
  pinMode(rightMostLineTrackerPin, INPUT_PULLUP);
  pinMode(rightMoreLineTrackerPin, INPUT_PULLUP);
  pinMode(rightLineTrackerPin, INPUT_PULLUP);
  pinMode(rightCrossLineTrackerPin, INPUT_PULLUP);

  // limit switch
  pinMode(frontLimitSwitchPin, INPUT_PULLUP);

  // Initialize motors
  leftMotor.attach(11);
  rightMotor.attach(10);
}

void MovementController::followLine() {

  int leftMostVal = digitalRead(leftMostLineTrackerPin);
  int leftMoreVal = digitalRead(leftMoreLineTrackerPin);
  int leftVal = digitalRead(leftLineTrackerPin);

  int rightVal = digitalRead(rightLineTrackerPin);
  int rightMoreVal = digitalRead(rightMoreLineTrackerPin);
  int rightMostVal = digitalRead(rightMostLineTrackerPin);

  if (leftMostVal == HIGH && rightMostVal == LOW) {
    leftMotor.write(50);
    rightMotor.write(90);
  } else if (leftMostVal == LOW && rightMostVal == HIGH) {
    leftMotor.write(90);
    rightMotor.write(50);
  } else if (leftMoreVal == HIGH && rightMoreVal == LOW) {
    leftMotor.write(65);
    rightMotor.write(90);
  } else if (leftMoreVal == LOW && rightMoreVal == HIGH) {
    leftMotor.write(90);
    rightMotor.write(65);
  } else if (leftVal == HIGH && rightVal == LOW) {
    leftMotor.write(78);
    rightMotor.write(90);
  } else if (leftVal == LOW && rightVal == HIGH) {
    leftMotor.write(90);
    rightMotor.write(78);
  } else {
    leftMotor.write(80);
    rightMotor.write(80);
  }

}

void MovementController::initLineCounter() {
  oldColor = digitalRead(rightCrossLineTrackerPin);
  numLinePassed = 0;
}

void MovementController::initFrontLineCounter() {
  frontLeftOldColor = digitalRead(leftMostLineTrackerPin);
  frontRightOldColor = digitalRead(rightMostLineTrackerPin);
  numFrontLinePassed = 0;
}


void MovementController::initRotation(Direction direction) {
  oldDegree = leftEncoder.read();
  oldColor =  direction == RIGHT_DIRECTION ?
              digitalRead(rightMostLineTrackerPin) :
              digitalRead(leftMostLineTrackerPin);
  numLinePassed = 0;
}

void MovementController::rotateCW() {
  leftMotor.write(115);
  rightMotor.write(76);
}

void MovementController::rotateCCW() {
  leftMotor.write(76);
  rightMotor.write(115);
}

bool MovementController::isRotationDone(int dstDegree) {
  int degree = leftEncoder.read();
  int degreeDiff = degree - oldDegree;

  return abs(degreeDiff) <= dstDegree / 2 * 2.73 ? false : isLine();
}

bool MovementController::isLine() {
  int leftMoreVal = digitalRead(leftMoreLineTrackerPin);
  int leftVal = digitalRead(leftLineTrackerPin);

  int rightVal = digitalRead(rightLineTrackerPin);
  int rightMoreVal = digitalRead(rightMoreLineTrackerPin);

  return  leftVal == HIGH || leftMoreVal == HIGH || rightVal == HIGH || rightMoreVal == HIGH;
}

bool MovementController::isCross(int num) {
  bool rightCrossLineVal = digitalRead(rightCrossLineTrackerPin);

  if (rightCrossLineVal != oldColor) {
    oldColor = rightCrossLineVal;
    if (rightCrossLineVal == HIGH)
      numLinePassed++;
  }
  return numLinePassed == num;
}

bool MovementController::isFrontCross(int num) {
  int leftMostLineVal = digitalRead(leftMostLineTrackerPin);
  int rightMostLineVal = digitalRead(rightMostLineTrackerPin);

  if (leftMostLineVal != frontLeftOldColor || rightMostLineVal != frontRightOldColor) {

    if (leftMostLineVal != frontLeftOldColor) frontLeftOldColor = leftMostLineVal;
    if (rightMostLineVal != frontRightOldColor) frontRightOldColor = rightMostLineVal;

    if (leftMostLineVal == HIGH && rightMostLineVal == HIGH) {
      numFrontLinePassed++;
      Serial.print("numFrontLinePassed ");
      Serial.print(numFrontLinePassed);
    }
  }

  return numFrontLinePassed ==  num;
}

bool MovementController::hasFrontCross() {
  isFrontCross(0);
  return numFrontLinePassed >  1;
}

void MovementController::goBackward() {
  leftMotor.write(110);
  rightMotor.write(110);
}

bool MovementController::isReachingContainer() {
  bool containerVal = digitalRead(frontLimitSwitchPin);
  return containerVal == LOW;
}


int MovementController::isNintyDoneShare(Direction direction) {

  bool mostLineVal = direction == RIGHT_DIRECTION ?
                     digitalRead(rightMostLineTrackerPin) :
                     digitalRead(leftMostLineTrackerPin);

  if (mostLineVal != oldColor) {
    oldColor = mostLineVal;
    if (mostLineVal == HIGH) numLinePassed++;
  }

  int degree = leftEncoder.read();
  return degree - oldDegree;
}

bool MovementController::isNintyDone(Direction direction) {
  int degreeDiff = isNintyDoneShare(direction);
  return abs(degreeDiff) <= 246 / 2 ? false : (numLinePassed > 1 && isLine());
}

bool MovementController::isNintyDoneEdge(Direction direction) {
  int degreeDiff = isNintyDoneShare(direction);
  return abs(degreeDiff) <= 246 / 2 ? false : isLine();
}

void MovementController::stop() {
  leftMotor.write(90);
  rightMotor.write(90);
}

bool MovementController::isStopped() {
  return leftMotor.read() == 90 && rightMotor.read() == 90;
}
