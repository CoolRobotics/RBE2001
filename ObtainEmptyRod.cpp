//#include <Servo.h>
//
//Servo rightMotor;
//Servo leftMotor;
//
//Servo armMotor;
//Servo wristMotor;
//Servo clawMotor;
//
//// left side
//int leftFrontLineTrackerPin = 25;
//int leftHorizontalLineTrackerPin = 24;
//int leftBackLineTrackerPin = 23;
//int leftBackMostLineTrackerPin = 22;
//
//// right side
//int rightFrontLineTrackerPin = 27;
//int rightHorizontalLineTrackerPin = 5;
//int rightBackLineTrackerPin = 26;
//int rightBackMostLineTrackerPin = 4;
//
//int frontLimitSwitchPin = 29;
//
//int upperArmLimitSwitchPin = 6;
//int lowerArmLimitSwitchPin = 28;
//
////int wristPotentiometerPin = 20;
//
//
//void setup() {
//  leftMotor.attach(11);
//  rightMotor.attach(10);
//  armMotor.attach(9);
//  wristMotor.attach(8);
//  clawMotor.attach(7);
//
//  isAtReactor(); // drive straight to reactor
//  isArmUp(); // if arm is not up, please raise arm to up position
//  isClawOpen(); // open claw if closed
//  positionClawToReactor() // get claw in postion to grab rod
//  
//
//}
//
//void isAtReactor() {
//  // line follow to reactor
//  while (digitalRead(frontLimitSwitchPin) == HIGH) {
//    if (digitalRead(leftFrontLineTrackerPin) == LOW && digitalRead(rightFrontLineTrackerPin) == LOW) {
//      leftMotor.write(130);
//      rightMotor.write(130);
//    } else if (digitalRead(leftFrontLineTrackerPin) == HIGH && digitalRead(rightFrontLineTrackerPin) == LOW) {
//      leftMotor.write(90);
//      rightMotor.write(130);
//    } else if (digitalRead(leftFrontLineTrackerPin) == LOW && digitalRead(rightFrontLineTrackerPin) == HIGH) {
//      leftMotor.write(130);
//      rightMotor.write(90);
//    } else {
//      leftMotor.write(130);
//      rightMotor.write(130);
//    }
//  }
//
//  // stop motors
//  if (digitalRead(frontLimitSwitchPin) == LOW) {
//    leftMotor.write(90);
//    rightMotor.write(90);
//  }
//}
//
//void isArmUp() {
//  bool upperSwitch = digitalRead(upperArmLimitSwitchPin);
//  bool lowerSwitch = digitalRead(lowerArmLimitSwitchPin);
//  // set the arm up if arm is floating between limit switches
//  while (upperSwitch == HIGH) {
//    armMotor.write(75); // raise arm slowly
//    Serial.println("LOWER");
//  }
//  armMotor.write(90);
//}
//
//void isClawOpen {
//  bool upperSwitch = digitalRead(upperArmLimitSwitchPin);
//  bool lowerSwitch = digitalRead(lowerArmLimitSwitchPin);
//
//  // open claw
//  for (int i = 180; i > 0; i--) {
//    claw.write(i);
//  }
//}
//
//void positionClawToReactor() {
//  
//}
//
//
//
//
////void testArm() {  // test arm motor with height limit switches (no fail safe)
////  bool upperSwitch = digitalRead(upperArmLimitSwitchPin);
////  bool lowerSwitch = digitalRead(lowerArmLimitSwitchPin);
////
////  // set the arm down if arm is floating between limit switches
////  if (lowerSwitch == HIGH && upperSwitch == HIGH) {
////    while (lowerSwitch == HIGH && upperSwitch == HIGH) {
////      armMotor.write(100); // lower arm slowly
////      Serial.println("LOWER");
////    }
////  }
////
////  delay(2000);
////
////  // raise and lower arm once
////  if (lowerSwitch == LOW) {
////    armMotor.write(90); // stop motor
////    Serial.println("STOP!");
////    delay(2000);  // preference delay inbetween transisitions
////    while (upperSwitch == HIGH) {
////      armMotor.write(80); // raise arm slowly
////      Serial.println("RAISE");
////    }
////    if (upperSwitch == LOW) {
////      armMotor.write(90);
////      Serial.println("STOP!");
////      delay(2000);
////      while (lowerSwitch == HIGH) {
////        armMotor.write(100);  // lower arm slowly
////        Serial.println("LOWER");
////      } //while
////    } //if
////  } //if
////} //testArm()
