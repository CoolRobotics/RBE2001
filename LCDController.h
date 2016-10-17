#include <LiquidCrystal.h>

class LCDController {
  public:
    void setup();
  private:
    LiquidCrystal lcd = LiquidCrystal(40, 41, 42, 43, 44, 45);
};
