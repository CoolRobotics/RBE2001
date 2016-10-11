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

void setup() {
  Serial.begin(115200);

//  pinMode(leftFrontLineTrackerPin, INPUT_PULLUP);
//  pinMode(leftFrontLineTrackerPin, INPUT_PULLUP);
//  pinMode(leftFrontLineTrackerPin, INPUT_PULLUP);
//  pinMode(leftFrontLineTrackerPin, INPUT_PULLUP);
//  
//  pinMode(leftFrontLineTrackerPin, INPUT_PULLUP);
//  pinMode(leftFrontLineTrackerPin, INPUT_PULLUP);
//  pinMode(leftFrontLineTrackerPin, INPUT_PULLUP);
//  pinMode(leftFrontLineTrackerPin, INPUT_PULLUP);
  pinMode(46, OUTPUT);
  pinMode(48, OUTPUT);
  pinMode(50, OUTPUT);
  pinMode(52, OUTPUT);
  pinMode(47, OUTPUT);
  pinMode(49, OUTPUT);
  pinMode(51, OUTPUT);
  pinMode(53, OUTPUT);
}

void loop() {
  printSwitchInputs();
  LEDs();
  


}

void printSwitchInputs() {
  int leftFront = digitalRead(leftFrontLineTrackerPin);
  int leftHorizontal = digitalRead(leftHorizontalLineTrackerPin);
  int leftBack = digitalRead(leftBackLineTrackerPin);
  int leftBackMost = digitalRead(leftBackMostLineTrackerPin);

  int rightFront = digitalRead(rightFrontLineTrackerPin);
  int rightHorizontal = digitalRead(rightHorizontalLineTrackerPin);
  int rightBack = digitalRead(rightBackLineTrackerPin);
  int rightBackMost = digitalRead(rightBackMostLineTrackerPin);

  Serial.print(leftFront);
  Serial.print(" ");
  Serial.print(leftHorizontal);
  Serial.print(" ");
  Serial.print(leftBack);
  Serial.print(" ");
  Serial.print(leftBackMost);
  Serial.print(" ");
  Serial.print(rightFront);
  Serial.print(" ");
  Serial.print(rightHorizontal);
  Serial.print(" ");
  Serial.print(rightBack);
  Serial.print(" ");
  Serial.println(rightBackMost);
}

void LEDs() {
  if (digitalRead(leftFrontLineTrackerPin) == HIGH) {
    digitalWrite(46, HIGH);
  } else {
    digitalWrite(46, LOW);
  }

  if (digitalRead(leftHorizontalLineTrackerPin) == HIGH) {
    digitalWrite(48, HIGH);
  } else {
    digitalWrite(48, LOW);
  }

  if (digitalRead(leftBackLineTrackerPin) == HIGH) {
    digitalWrite(50, HIGH);
  } else {
    digitalWrite(50, LOW);
  }

  if (digitalRead(leftBackMostLineTrackerPin) == HIGH) {
    digitalWrite(52, HIGH);
  } else {
    digitalWrite(52, LOW);
  }

  if (digitalRead(rightFrontLineTrackerPin) == HIGH) {
    digitalWrite(47, HIGH);
  } else {
    digitalWrite(47, LOW);
  }

  if (digitalRead(rightHorizontalLineTrackerPin) == HIGH) {
    digitalWrite(49, HIGH);
  } else {
    digitalWrite(49, LOW);
  }

  if (digitalRead(rightBackLineTrackerPin) == HIGH) {
    digitalWrite(51, HIGH);
  } else {
    digitalWrite(51, LOW);
  }

  if (digitalRead(rightBackMostLineTrackerPin) == HIGH) {
    digitalWrite(53, HIGH);
  } else {
    digitalWrite(53, LOW);
  }
}

