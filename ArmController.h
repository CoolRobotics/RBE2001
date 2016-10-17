#include "Servo.h"

class ArmController {
  public:
    ArmController();
    void setup();
    void dropDown();
    bool isDropDownFinish();
    void liftUp();
    bool isLiftedUp();

  private:
    int lowerLimitSwitchPin;
    int upperLimitSwitchPin;
    Servo armServo;
};
