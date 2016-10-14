#include <Servo.h>
#include "Arduino.h"
#include "MovementController.h"

Encoder leftEncoder(2, 3);
//Encoder rightEncoder(18, 19);

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

/**

   L1 R1
   L2 R2

   L1 = left back sensor


   00   drive straight
   00

   00      error or turn right
   01

   00      error or turn left
   10

   00      error
   11

   01      error or turn right
   00

   01    merge right slowly
   01

   01      error
   10

   01    turn left
   11

   10      error turn right
   00

   10      error or drive back and use front sensors
   01

   10    merge left slowly
   10

   10    turn right
   11

   11      error
   00

   11    turn left
   01

   11    turn right
   10

   11  drive straight
   11
*/
void MovementController::followLine() {

  int leftMostVal = digitalRead(leftMostLineTrackerPin);
  int leftMoreVal = digitalRead(leftMoreLineTrackerPin);
  int leftVal = digitalRead(leftLineTrackerPin);

  int rightVal = digitalRead(rightLineTrackerPin);
  int rightMoreVal = digitalRead(rightMoreLineTrackerPin);
  int rightMostVal = digitalRead(rightMostLineTrackerPin);

  if (leftMostVal == HIGH && rightMostVal == LOW) {
    leftMotor.write(60);
    rightMotor.write(90);
  } else if (leftMostVal == LOW && rightMostVal == HIGH) {
    leftMotor.write(90);
    rightMotor.write(60);
  } else if (leftMoreVal == HIGH && rightMoreVal == LOW) {
    leftMotor.write(70);
    rightMotor.write(90);
  } else if (leftMoreVal == LOW && rightMoreVal == HIGH) {
    leftMotor.write(90);
    rightMotor.write(70);
  } else if (leftVal == HIGH && rightVal == LOW) {
    leftMotor.write(75);
    rightMotor.write(90);
  } else if (leftVal == LOW && rightVal == HIGH) {
    leftMotor.write(90);
    rightMotor.write(75);
  } else {
    leftMotor.write(80);
    rightMotor.write(80);
  }

}

void MovementController::goStraightLine() {
  leftMotor.write(80);
  rightMotor.write(80);
}

void MovementController::initLineCounter() {
  oldColor = digitalRead(leftMostLineTrackerPin);
  numLinePassed = 0;
}


void MovementController::initRotation(Direction direction) {
  oldDegree = leftEncoder.read();
  oldColor =  direction == RIGHT_DIRECTION ?
              digitalRead(rightMostLineTrackerPin) :
              digitalRead(leftMostLineTrackerPin);
  numLinePassed = 0;
}

void MovementController::rotateCW() {
  leftMotor.write(110);
  rightMotor.write(80);
}

void MovementController::rotateCCW() {
  leftMotor.write(80);
  rightMotor.write(110);
}

void MovementController::stop() {
  leftMotor.write(90);
  rightMotor.write(90);
}

bool MovementController::isRotationDone(int dstDegree) {
  int degree = leftEncoder.read();
  int degreeDiff = degree - oldDegree;

  return abs(degreeDiff) <= dstDegree / 2 * 2.73 ? false : isLine();
}

bool MovementController::isLine() {
  int leftMostVal = digitalRead(leftMostLineTrackerPin);
  int leftMoreVal = digitalRead(leftMoreLineTrackerPin);
  int leftVal = digitalRead(leftLineTrackerPin);

  int rightVal = digitalRead(rightLineTrackerPin);
  int rightMoreVal = digitalRead(rightMoreLineTrackerPin);
  int rightMostVal = digitalRead(rightMostLineTrackerPin);

  //  Serial.print(leftMost);
  //  Serial.print(" ");
  //  Serial.print(leftMore);
  //  Serial.print(" ");
  //  Serial.print(left);
  //  Serial.print(" ");
  //  Serial.print(right);
  //  Serial.print(" ");
  //  Serial.print(rightMore);
  //  Serial.print(" ");
  //  Serial.println(rightMost);

  return  leftVal == HIGH || leftMoreVal == HIGH || leftMostVal == HIGH || rightVal == HIGH || rightMoreVal == HIGH || rightMostVal == HIGH;
}

void MovementController::initCrossDetection() {
  bool rightCrossLineVal = digitalRead(rightCrossLineTrackerPin);

  return rightCrossLineVal == HIGH;
}

bool MovementController::isCross() {
  bool rightCrossLineVal = digitalRead(rightCrossLineTrackerPin);

  return rightCrossLineVal == HIGH;
}

bool MovementController::isFrontCross() {
  bool leftMostLineVal = digitalRead(leftMostLineTrackerPin);
  bool rightMostLineVal = digitalRead(rightMostLineTrackerPin);

  return leftMostLineVal == HIGH && rightMostLineVal == HIGH;
}

void MovementController::goBackward() {
  leftMotor.write(105);
  rightMotor.write(105);
}

bool MovementController::isReachingContainer() {
  bool containerVal = digitalRead(frontLimitSwitchPin);
  return containerVal == LOW;
}

bool MovementController::isNintyDone(Direction direction) {
  int degree = leftEncoder.read();
  int degreeDiff = degree - oldDegree;

  bool mostLineVal = direction == RIGHT_DIRECTION ?
                     digitalRead(rightMostLineTrackerPin) :
                     digitalRead(leftMostLineTrackerPin);

  if (mostLineVal != oldColor) {
    oldColor = mostLineVal;
    if (mostLineVal == HIGH) numLinePassed++;
  }

  return abs(degreeDiff) <= 246 / 2 ? false : (numLinePassed > 1 && isLine());
}

