#include "LCDController.h"

void LCDController::setup() {
  lcd.begin(16, 2);
}

void LCDController::clear() {
  lcd.clear();
}


void LCDController::println(int row, char* str) {
  char buffer[17];
  sprintf(buffer, "%-16s", str);
     
  lcd.setCursor(0, row);
  lcd.print(buffer);
}

void LCDController::println(int row, int n) {
  char buffer[17];
  sprintf(buffer, "%-16ld", n);
     
  lcd.setCursor(0, row);
  lcd.print(buffer);
}

