#include <LiquidCrystal.h>
#include "Arduino.h"

class LCD {
  public:
    LCD();
    void print(int row, const char* message);
    void print(int row, const char* deviceName, int value);
    void print(int row, const char* deviceName1, int value1, const char* deviceName2, int value2);
  private:
    LiquidCrystal lcd = LiquidCrystal(12, 9, 8, 7, 6, 5); // NEW !!! (dropped '10' and added '5')
};

