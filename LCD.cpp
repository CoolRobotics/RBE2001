#include <LiquidCrystal.h>
#include <stdio.h>
#include "Arduino.h"
#include "LCD.h"

LCD::LCD() {
  // set up the LCD's number of columns and rows:
  LCD::lcd.begin(16, 2);
}

void LCD::print(int row, const char* message) {
//  char buffer[17];     // The messaage buffer
//  sprintf(buffer, "%-16s", message);
  // print the message onto LCD
  LCD::lcd.setCursor(0, row);
  LCD::lcd.print("Good");
}

void LCD::print(int row, const char* deviceName, int value) {
  char buffer[17];     // The messaage buffer
  char messageBuffer[17];     // The messaage buffer
  
  sprintf(buffer, "%s: %d", deviceName, value);
//  sprintf(messageBuffer, "%-16d", buffer);
  // print the value onto LCD
  LCD::lcd.setCursor(0, row);
  LCD::lcd.print(buffer);
}

void LCD::print(int row, const char* deviceName1, int value1, const char* deviceName2, int value2) {
  char buffer[17];     // The messaage buffer
  char messageBuffer[17];     // The messaage buffer
  
  sprintf(buffer, "%s %d %s %d", deviceName1, value1, deviceName2, value2);
  sprintf(messageBuffer, "%-16d", buffer);
  // print the value onto LCD
  LCD::lcd.setCursor(0, row);
  LCD::lcd.print(messageBuffer);
}
