#include <LiquidCrystal.h>

class LCDController {
  public:
    void setup();
    void println(int row, char* str);
    void clear();
  private:
    LiquidCrystal lcd = LiquidCrystal(40, 41, 42, 43, 44, 45);
    
};
