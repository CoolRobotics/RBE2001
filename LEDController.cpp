#include "Arduino.h"
#include "LEDController.h"

void LEDController::setup() {
  redLEDPin = 37;
  greenLEDPin = 33;
  yellowLEDPin = 35;
  
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);
  pinMode(yellowLEDPin, OUTPUT);

  redTime = millis();
  greenTime = millis();
  yellowTime = millis();

  redState = LOW;
  greenState = LOW;
  yellowState = LOW;
}

void LEDController::allOff() {
  digitalWrite(redLEDPin, LOW);
  digitalWrite(greenLEDPin, LOW);
  digitalWrite(yellowLEDPin, LOW);

  redState = LOW;
  greenState = LOW;
  yellowState = LOW;
}

void  LEDController::redOn() {
  if(millis() - redTime > FLASH_PERIOD)  {
    redState = !redState;
    redTime = millis();
  }
  digitalWrite(redLEDPin, redState);
}
void  LEDController::greenOn() {
  if(millis() - greenTime > FLASH_PERIOD)  {
    greenState = !greenState;
    greenTime = millis();
  }
  digitalWrite(greenLEDPin, greenState);
}

void  LEDController::yellowOn() {
  if(millis() - yellowTime > FLASH_PERIOD)  {
    yellowState = !yellowState;
    yellowTime = millis();
  }
  digitalWrite(yellowLEDPin, yellowState);
}
