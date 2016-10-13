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

  // right side
  rightMostLineTrackerPin = 24;
  rightMoreLineTrackerPin = 25;
  rightLineTrackerPin = 26;
  rightCrossLineTrackerPin = 22;

  // Left Side
  pinMode(leftMostLineTrackerPin, INPUT_PULLUP);
  pinMode(leftMoreLineTrackerPin, INPUT_PULLUP);
  pinMode(leftLineTrackerPin, INPUT_PULLUP);

  // Right side
  pinMode(rightMostLineTrackerPin, INPUT_PULLUP);
  pinMode(rightMoreLineTrackerPin, INPUT_PULLUP);
  pinMode(rightLineTrackerPin, INPUT_PULLUP);
  pinMode(rightCrossLineTrackerPin, INPUT_PULLUP);

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

  int leftMost = digitalRead(leftMostLineTrackerPin);
  int leftMore = digitalRead(leftMoreLineTrackerPin);
  int left = digitalRead(leftLineTrackerPin);

  int right = digitalRead(rightLineTrackerPin);
  int rightMore = digitalRead(rightMoreLineTrackerPin);
  int rightMost = digitalRead(rightMostLineTrackerPin);

  if (leftMost == HIGH && rightMost == LOW) {
    leftMotor.write(60);
    rightMotor.write(90);
  } else if (leftMost == LOW && rightMost == HIGH) {
    leftMotor.write(90);
    rightMotor.write(60);
  } else if (leftMore == HIGH && rightMore == LOW) {
    leftMotor.write(70);
    rightMotor.write(90);
  } else if (leftMore == LOW && rightMore == HIGH) {
    leftMotor.write(90);
    rightMotor.write(70);
  } else if (left == HIGH && right == LOW) {
    leftMotor.write(75);
    rightMotor.write(90);
  } else if (left == LOW && right == HIGH) {
    leftMotor.write(90);
    rightMotor.write(75);
  } else {
    leftMotor.write(80);
    rightMotor.write(80);
  }

}

void MovementController::goStraightLine() {
  leftMotor.write(83);
  rightMotor.write(83);
}

void MovementController::initRotation() {
  oldDegree = leftEncoder.read();
  stop();
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
  Serial.print(abs(degreeDiff));
  Serial.print(",");
  Serial.println(dstDegree / 2 * 2.73);

  return abs(degreeDiff) <= dstDegree 2.73 ? false : isLine();
}

bool MovementController::isLine() {
  int leftMost = digitalRead(leftMostLineTrackerPin);
  int leftMore = digitalRead(leftMoreLineTrackerPin);
  int left = digitalRead(leftLineTrackerPin);

  int right = digitalRead(rightLineTrackerPin);
  int rightMore = digitalRead(rightMoreLineTrackerPin);
  int rightMost = digitalRead(rightMostLineTrackerPin);

  Serial.print(leftMost);
  Serial.print(" ");
  Serial.print(leftMore);
  Serial.print(" ");
  Serial.print(left);
  Serial.print(" ");
  Serial.print(right);
  Serial.print(" ");
  Serial.print(rightMore);
  Serial.print(" ");
  Serial.println(rightMost);

  return  left == HIGH || leftMore == HIGH || right == HIGH || rightMore == HIGH;
}

bool MovementController::isCross() {
  bool rightCrossLine = digitalRead(rightCrossLineTrackerPin);

  return rightCrossLine == HIGH;
}

bool MovementController::isFrontCross() {
  bool leftMostLine = digitalRead(leftMostLineTrackerPin);
  bool rightMostLine = digitalRead(rightMostLineTrackerPin);

  return leftMostLine == HIGH && rightMostLine == HIGH;
}

void MovementController::goBackward() {
  leftMotor.write(105);
  rightMotor.write(105);
}

