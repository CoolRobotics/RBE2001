#include <Servo.h>
#include <Encoder.h>
#include "Arduino.h"
#include "Messages.h"
#include "LineFollower.h"
#include "RBE2001.h"


Messages msg;
unsigned long timeForHeartbeat;
int frontLineTrackerPin = 29;

int rightMostLineTrackerPin = 7;
int rightMoreLineTrackerPin = 28;
int rightLineTrackerPin = 27;

int leftLineTrackerPin = 26;
int leftMoreLineTrackerPin = 25;
int leftMostLineTrackerPin = 6;

//int upperArmLimitSwitchPin = 26;
//int lowerArmLimitSwitchPin = 27;  // NEW !!!

int oldDegree;

// Interrupts
int leftEncoderPin1 = 2; // All four encoder wires (left and right) are in interrupts
int leftEncoderPin2 = 3;
int rightEncoderPin1 = 18; // NEW !!!
int rightEncoderPin2 = 19; // NEW !!!
int wristPotentiometerPin = 20; // NEW!!!

/**
   This "starter" program reads and debug-prints messages from the field. It also sends
   heartbeat messages every second letting the field know that the robot is still running.

   You can use this as the basis for your project, or copy the elements from here into your
   project. If you do that, be sure that you include the Messages and BTComms classes (.cpp
   and .h files) into your new project.
*/

Servo rightMotor;
Servo leftMotor;

// NEW !!!
Servo armMotor;
Servo wristMotor;
Servo clawMotor;


// Initialize encoder
Encoder leftWheelEncoder(leftEncoderPin1, leftEncoderPin2); // NEW !!! (changed)
Encoder rightWheelEncoder(rightEncoderPin1, rightEncoderPin2); // NEW !!!

// Keep track of current action
Action action;

/**
   Initialize the messages class and the debug serial port
*/
void setup() {
  Serial.begin(115200);
  Serial.println("Starting");
  msg.setup();
  timeForHeartbeat = millis() + 1000;
  pinMode(frontLineTrackerPin, INPUT);

  pinMode(leftMostLineTrackerPin, INPUT);
  pinMode(leftMoreLineTrackerPin, INPUT);
  pinMode(leftLineTrackerPin, INPUT);

  pinMode(rightLineTrackerPin, INPUT);
  pinMode(rightMoreLineTrackerPin, INPUT);
  pinMode(rightMostLineTrackerPin, INPUT);
  //
  //  // Initialize motors
  leftMotor.attach(11);
  rightMotor.attach(10);

  action = followLineBackwardAction;
}

/**
   The loop method runs continuously, so call the Messages read method to check for input
   on the bluetooth serial port and parse any complete messages. Then send the heartbeat if
   it's time to do that.

   For the final project, one good way of implementing it is to use a state machine with high
   level tasks as states. The state will tell what you should be doing each time the loop
   function is called.

   Refer to the state machine lecture or look at the BTComms class for an example on how to
   implement state machines.
*/
void loop() {
  if (msg.read()) {
    switch (msg.getMessageType()) {
      case Messages::kStopMovement:
        // Received "Stop" message
        break;
      case Messages::kResumeMovement:
        // Received "Resume" message
        break;
    }
  }

  if (millis() > timeForHeartbeat) {
    timeForHeartbeat = millis() + 1000;
    msg.sendHeartbeat();
  }
  switch (action) {
    case followLineBackwardAction:
      followLine(backwardDirection);
      //      if (isFollowLineFinished()) {
      //        Serial.println("Backward");
      //        delay(1000);
      //        oldDegree = leftWheelEncoder.read(); // new
      //        action = rotateNintyDegreesAction;
      //      }
      break;
    case followLineForwardAction:
      Serial.println("Forward");
      followLine(forwardDirection);
      break;
    case rotateNintyDegreesAction:
      Serial.println("Rotate");
      turnNintyDegrees();
      break;
  }
}

void followLine(Direction direction) {


  int front = digitalRead(frontLineTrackerPin);

  int leftMost = digitalRead(leftMostLineTrackerPin);
  int leftMore = digitalRead(leftMoreLineTrackerPin);
  int left = digitalRead(leftLineTrackerPin);

  int right = digitalRead(rightLineTrackerPin);
  int rightMore = digitalRead(rightMoreLineTrackerPin);
  int rightMost = digitalRead(rightMostLineTrackerPin);

  Serial.print(front);
  Serial.print(" ");

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

  Serial.print(rightMost);
  Serial.println(" ");




  if (left == HIGH && right == LOW) {
    leftMotor.write(90);
    rightMotor.write(75);
  } else if (left == LOW && right == HIGH) {
    leftMotor.write(75);
    rightMotor.write(90);
  } else if (left == LOW && right == LOW) {
    if (leftMore == HIGH && rightMore == LOW) {
      leftMotor.write(90);
      rightMotor.write(60);
    } else if (leftMore == LOW && rightMore == HIGH) {
      leftMotor.write(60);
      rightMotor.write(90);
    } else if (leftMore == LOW && rightMore == LOW) {
      if (leftMost == HIGH && rightMost == LOW) {
        leftMotor.write(90);
        rightMotor.write(43);
      } else if (leftMost == LOW && rightMost == HIGH) {
        leftMotor.write(43);
        rightMotor.write(90);
      } else {
        leftMotor.write(80);
        rightMotor.write(80);
      }
    }
  }
}

void turnNintyDegrees() {
  int newDegree = leftWheelEncoder.read();
  int differenceDegree = oldDegree - newDegree;
  if (abs(differenceDegree) < 200) { //246
    Serial.print(oldDegree);
    Serial.print(" ");
    Serial.print(leftWheelEncoder.read());
    Serial.print(" ");
    Serial.println(differenceDegree);
    leftMotor.write(80);
    rightMotor.write(110);
  } else {
    Serial.println("STOP!");
    leftMotor.write(90);
    rightMotor.write(90);
    delay(1000);
    action = followLineForwardAction;
  }
}

//bool isFollowLineFinished() {
//  return digitalRead(horizontalLineTrackerPin) == HIGH;
//}

//void raiseArm() {
//  armMotor.write
//}
//
//void lowerArm() {
//
//}
//
//void stopArm() {
//  if (
//}

