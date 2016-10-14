#include "Servo.h"

class ArmController {
  public:
    ArmController();
    setup();
    void dropDown();
    bool isDropDownFinish();
    void stablize();
    void grab();
    void release();
    bool isGrabFinished();
    void liftUp();
    bool isLiftedUp();
    bool isReleaseFinished();
    void rotateDownGraber();
  private:
    int lowerLimitSwitchPin;
    int upperLimitSwitchPin;
    Servo armServo;
    Servo graberServo;
    Servo transPlaneServo;
};
