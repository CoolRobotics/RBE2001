#include <Servo.h>

Servo rightMotor;
Servo leftMotor;

// left side
int leftFrontLineTrackerPin = 25;
int leftHorizontalLineTrackerPin = 24;
int leftBackLineTrackerPin = 23;
int leftBackMostLineTrackerPin = 22;

// right side
int rightFrontLineTrackerPin = 27;
int rightHorizontalLineTrackerPin = 5;
int rightBackLineTrackerPin = 26;
int rightBackMostLineTrackerPin = 4;

int ForwardLineFollowFrontSensors = 39;
int ForwardLineFollowBackSensors = 41;
int BackwardLineFollowFrontSensors = 43;
int BackwardLineFollowBackSensors = 45;

int frontLimitSwitchPin = 28;

void setup() {
  Serial.begin(9600);
  
  // Left Side
  pinMode(leftFrontLineTrackerPin, INPUT_PULLUP);
  pinMode(leftHorizontalLineTrackerPin, INPUT_PULLUP);
  pinMode(leftBackLineTrackerPin, INPUT_PULLUP);
  pinMode(leftBackMostLineTrackerPin, INPUT_PULLUP);

  // Right side
  pinMode(rightFrontLineTrackerPin, INPUT_PULLUP);
  pinMode(rightHorizontalLineTrackerPin, INPUT_PULLUP);
  pinMode(rightBackLineTrackerPin, INPUT_PULLUP);
  pinMode(rightBackMostLineTrackerPin, INPUT_PULLUP);

  // Initialize motors
  leftMotor.attach(11);
  rightMotor.attach(10);

  pinMode(ForwardLineFollowFrontSensors, INPUT_PULLUP);
  pinMode(ForwardLineFollowBackSensors, INPUT_PULLUP);
  pinMode(BackwardLineFollowFrontSensors, INPUT_PULLUP);
  pinMode(BackwardLineFollowBackSensors, INPUT_PULLUP);

  pinMode(46, OUTPUT);
  pinMode(48, OUTPUT);
  pinMode(50, OUTPUT);
  pinMode(52, OUTPUT);
  pinMode(47, OUTPUT);
  pinMode(49, OUTPUT);
  pinMode(51, OUTPUT);
  pinMode(53, OUTPUT);

  pinMode(28, INPUT_PULLUP);
}

void loop() {
//  jumper();

//  ForwardLineFollow_FrontSensors(); // pretty good, try to use back sensors as well (like from backwards with front sensors)
//  ForwardLineFollow_BackSensors(); // s-bends out of line (change how the robot corrects itself)
//  BackwardLineFollow_FrontSensors(); // needs work (would help to have back sensors to assist when both front are HIGH)
//  BackwardLineFollow_BackSensors(); // WORKS!

// Learned that the front sensors could be used for line following but should also use the back sensors for help. more importantly to just use the back sensors
// for line following and the front sensors for counting the perpendicular lines.
 
//  delay(2000);
//  BackwardLineFollow_BackSensors();
//  delay(2000);

  if (digitalRead(frontLimitSwitchPin) == LOW) {
    ForwardLineFollow_BackSensors();
  } else {
    BackwardLineFollow_BackSensors();
  }

}



void ForwardLineFollow_FrontSensors() {
  bool leftSensor = digitalRead(leftFrontLineTrackerPin);
  bool rightSensor = digitalRead(rightFrontLineTrackerPin);
  int Speed = 105;
  int Stop = 90;
  if (leftSensor == LOW && rightSensor == LOW) {
    digitalWrite(46, LOW);
    digitalWrite(47, LOW);
    leftMotor.write(Speed);
    rightMotor.write(Speed);
  } else if (leftSensor == HIGH && rightSensor == LOW) {
    digitalWrite(46, LOW);
    digitalWrite(47, LOW);
    leftMotor.write(Stop);
    rightMotor.write(Speed);
  } else if (leftSensor == LOW && rightSensor == HIGH) {
    digitalWrite(46, LOW);
    digitalWrite(47, LOW);
    leftMotor.write(Speed);
    rightMotor.write(Stop);
  } else {
    digitalWrite(46, HIGH);
    digitalWrite(47, HIGH);
  }
}

void ForwardLineFollow_BackSensors() {
  bool leftBackSensor = digitalRead(leftBackLineTrackerPin);
  bool rightBackSensor = digitalRead(rightBackLineTrackerPin);
  bool leftBackMostSensor = digitalRead(leftBackMostLineTrackerPin);
  bool rightBackMostSensor = digitalRead(rightBackMostLineTrackerPin);
  int Speed = 108;
  int Stop = 90;
//  if (leftSensor == LOW && rightSensor == LOW) {
//    digitalWrite(52, LOW);
//    digitalWrite(53, LOW);
//    leftMotor.write(Speed);
//    rightMotor.write(Speed);
//  } else if (leftSensor == HIGH && rightSensor == LOW) {
//    digitalWrite(52, HIGH);
//    digitalWrite(53, LOW);
//    leftMotor.write(Speed);
//    rightMotor.write(Stop);
//  } else if (leftSensor == LOW && rightSensor == HIGH) {
//    digitalWrite(52, LOW);
//    digitalWrite(53, HIGH);
//    leftMotor.write(Stop);
//    rightMotor.write(Speed);
//  } else {
//    digitalWrite(52, HIGH);
//    digitalWrite(53, HIGH);
//  }
  if (leftBackSensor == LOW && rightBackSensor == LOW && leftBackMostSensor == LOW && rightBackMostSensor == LOW) {
    digitalWrite(50, LOW);
    digitalWrite(51, LOW);
    digitalWrite(52, LOW);
    digitalWrite(53, LOW);
    leftMotor.write(Speed);
    rightMotor.write(Speed);
  } else if (leftBackSensor == HIGH && rightBackSensor == LOW && leftBackMostSensor == HIGH && rightBackMostSensor == LOW) {
    digitalWrite(50, HIGH);
    digitalWrite(51, LOW);
    digitalWrite(52, HIGH);
    digitalWrite(53, LOW);
    leftMotor.write(Speed - 2); // merge slowly onto left line
    rightMotor.write(Speed);
  } else if (leftBackSensor == LOW && rightBackSensor == HIGH && leftBackMostSensor == LOW && rightBackMostSensor == HIGH) {
    digitalWrite(50, LOW);
    digitalWrite(51, HIGH);
    digitalWrite(52, LOW);
    digitalWrite(53, HIGH);
    leftMotor.write(Speed);
    rightMotor.write(Speed - 2); // merge slowly onto right line
  } else if (leftBackSensor == LOW && rightBackSensor == HIGH && leftBackMostSensor == HIGH && rightBackMostSensor == HIGH) {
    digitalWrite(50, LOW);
    digitalWrite(51, HIGH);
    digitalWrite(52, HIGH);
    digitalWrite(53, HIGH);
    leftMotor.write(Stop);
    rightMotor.write(Speed);
  } else if (leftBackSensor == HIGH && rightBackSensor == LOW && leftBackMostSensor == HIGH && rightBackMostSensor == HIGH) {
    digitalWrite(50, HIGH);
    digitalWrite(51, LOW);
    digitalWrite(52, HIGH);
    digitalWrite(53, HIGH);
    leftMotor.write(Speed);
    rightMotor.write(Stop);
  } else if (leftBackSensor == HIGH && rightBackSensor == HIGH && leftBackMostSensor == LOW && rightBackMostSensor == HIGH) {
    digitalWrite(50, HIGH);
    digitalWrite(51, HIGH);
    digitalWrite(52, LOW);
    digitalWrite(53, HIGH);
    leftMotor.write(Stop);
    rightMotor.write(Speed);
  } else if (leftBackSensor == HIGH && rightBackSensor == HIGH && leftBackMostSensor == HIGH && rightBackMostSensor == LOW) {
    digitalWrite(50, HIGH);
    digitalWrite(51, HIGH);
    digitalWrite(52, HIGH);
    digitalWrite(53, LOW);
    leftMotor.write(Speed);
    rightMotor.write(Stop);
  } else if (leftBackSensor == HIGH && rightBackSensor == HIGH && leftBackMostSensor == HIGH && rightBackMostSensor == HIGH) {
    digitalWrite(50, HIGH);
    digitalWrite(51, HIGH);
    digitalWrite(52, HIGH);
    digitalWrite(53, HIGH);
    leftMotor.write(Speed);
    rightMotor.write(Speed);    
  } else {
    digitalWrite(50, LOW);
    digitalWrite(51, LOW);
    digitalWrite(52, LOW);
    digitalWrite(53, LOW);
//    leftMotor.write(Speed - 33);
//    rightMotor.write(Speed - 33);
  }
}

void BackwardLineFollow_FrontSensors() {
  bool leftSensor = digitalRead(leftFrontLineTrackerPin);
  bool rightSensor = digitalRead(rightFrontLineTrackerPin);
  int Speed = 80;
  int Stop = 90;
  if (leftSensor == LOW && rightSensor == LOW) {
    digitalWrite(52, LOW);
    digitalWrite(53, LOW);
    leftMotor.write(Speed);
    rightMotor.write(Speed);
  } else if (leftSensor == HIGH && rightSensor == LOW) {
    digitalWrite(52, HIGH);
    digitalWrite(53, LOW);
    leftMotor.write(Stop);
    rightMotor.write(Speed);
  } else if (leftSensor == LOW && rightSensor == HIGH) {
    digitalWrite(52, LOW);
    digitalWrite(53, HIGH);
    leftMotor.write(Speed);
    rightMotor.write(Stop);
  } else {
    digitalWrite(52, HIGH);
    digitalWrite(53, HIGH);
  }
}

void BackwardLineFollow_BackSensors() {
  bool leftSensor = digitalRead(leftBackMostLineTrackerPin);
  bool rightSensor = digitalRead(rightBackMostLineTrackerPin);
  int Speed = 78;
  int Stop = 90;
  if (leftSensor == LOW && rightSensor == LOW) {
    digitalWrite(52, LOW);
    digitalWrite(53, LOW);
    leftMotor.write(Speed);
    rightMotor.write(Speed);
  } else if (leftSensor == HIGH && rightSensor == LOW) {
    digitalWrite(52, HIGH);
    digitalWrite(53, LOW);
    leftMotor.write(Stop);
    rightMotor.write(Speed);
  } else if (leftSensor == LOW && rightSensor == HIGH) {
    digitalWrite(52, LOW);
    digitalWrite(53, HIGH);
    leftMotor.write(Speed);
    rightMotor.write(Stop);
  } else {
    digitalWrite(52, HIGH);
    digitalWrite(53, HIGH);
  }
}

void jumper() {
//    if (digitalRead(ForwardLineFollowFrontSensors) == HIGH) {
//    ForwardLineFollow_FrontSensors();
//  } else if (digitalRead(ForwardLineFollowBackSensors) == HIGH) {
//    ForwardLineFollow_BackSensors();
//  } else if (digitalRead(BackwardLineFollowFrontSensors) == HIGH) {
//    BackwardLineFollow_FrontSensors();
//  } else if (digitalRead(BackwardLineFollowBackSensors) == HIGH) {
//    BackwardLineFollow_BackSensors(); // WORKS!
//  } else {
//    leftMotor.write(90);
//    rightMotor.write(90);
//    digitalWrite(53, HIGH);
//    delay(500);
//    digitalWrite(53, LOW);
//    delay(450);
//  }
// // delay(50);
}

