#include <Servo.h>
#include <Encoder.h>
#include "Arduino.h"
#include "Messages.h"
#include "LCD.h"
#include "LineFollower.h"
#include "RBE2001.h"

// initialize the LCD
LCD lcd;

Messages msg;
unsigned long timeForHeartbeat;
int leftLineTrackerPin = 22;
int centerLineTrackerPin = 23;
int rightLineTrackerPin = 24;
int horizontalLineTrackerPin = 25;
int upperArmLimitSwitchPin = 26;  // NEW !!!
int lowerArmLimitSwitchPin = 27;  // NEW !!!

int oldDegree; //new

// NEW !!!
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
  // Initialize LCD screen with 2 rows and 16 columns
  lcd = LCD();
  pinMode(leftLineTrackerPin, INPUT);
  pinMode(centerLineTrackerPin, INPUT);
  pinMode(rightLineTrackerPin, INPUT);
  pinMode(horizontalLineTrackerPin, INPUT);

  // Initialize motors
  leftMotor.attach(11);
  rightMotor.attach(10);
  armMotor.attach(9);    // new

  action = followLineBackwardAction; // first action
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
        lcd.print(1, "Stop");
        break;
      case Messages::kResumeMovement:
        // Received "Resume" message
        lcd.print(1, "Resume");
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
      if (isFollowLineFinished()) {
        Serial.println("Backward");
        delay(1000);
        oldDegree = leftWheelEncoder.read(); // new
        action = rotateNintyDegreesAction;
      }
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
  int left = digitalRead(leftLineTrackerPin);
  int center = digitalRead(centerLineTrackerPin);
  int right = digitalRead(rightLineTrackerPin);

  if (left == HIGH && right == LOW) {
    //    if (direction == backwardDirection) {
    leftMotor.write(105);
    rightMotor.write(85);
    //    } else {
    //      leftMotor.write(75);
    //      rightMotor.write(90);
    //    }

  } else if (left == LOW && right == HIGH) {
    //     if (direction == backwardDirection) {
    leftMotor.write(85);
    rightMotor.write(105);
    //     } else {
    //      leftMotor.write(90);
    //    rightMotor.write(75);
    //     }
  } if ((left == HIGH && right == HIGH) || center == HIGH) {
    if (direction == backwardDirection) {
      leftMotor.write(75);
      rightMotor.write(75);
    } else {
      leftMotor.write(105);
      rightMotor.write(105);
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

bool isFollowLineFinished() {
  return digitalRead(horizontalLineTrackerPin) == HIGH;
}

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


void testArm() {  // test arm motor with height limit switches (no fail safe)
  bool upperSwitch = digitalRead(upperArmLimitSwitchPin);
  bool lowerSwitch = digitalRead(lowerArmLimitSwitchPin);
  
  // set the arm down if arm is floating between limit switches
  if (lowerSwitch == HIGH && upperSwitch == HIGH) {
    while (lowerSwitch == HIGH && upperSwitch == HIGH) {
      armMotor.write(100); // lower arm slowly
      Serial.println("LOWER");
    }
  }
  
  delay(2000);
  
  // raise and lower arm once
  if (lowerSwitch == LOW) {
    armMotor.write(90); // stop motor
    Serial.println("STOP!");
    delay(2000);  // preference delay inbetween transisitions
    while (upperSwitch == HIGH) {
      armMotor.write(80); // raise arm slowly
      Serial.println("RAISE");
    }
    if (upperSwitch == LOW) {
      armMotor.write(90); 
      Serial.println("STOP!");
      delay(2000);
      while (lowerSwitch == HIGH) {
        armMotor.write(100);  // lower arm slowly
        Serial.println("LOWER");
      } //while
    } //if
  } //if
} //testArm()

