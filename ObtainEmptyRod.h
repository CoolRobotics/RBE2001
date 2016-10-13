#include <Servo.h>
//#include <Encoder.h>

class ObtainEmptyRod {
  public:
    ObtainEmptyRod();
//    void setup();
//    void followForward();
//    void followBackward();
//    void rotateCW();
//    bool isCross();
//    bool isRotationDone();
//    void initRotation();
//    void stop();
  
  private:
    // motors
    Servo leftMotor;
    Servo rightMotor;

//    int degree = 0;
//    int oldDegree = 0;

    // left side
    int leftFrontLineTrackerPin;
    int leftHorizontalLineTrackerPin;
    int leftBackLineTrackerPin;
    int leftBackMostLineTrackerPin;

    // right side
    int rightFrontLineTrackerPin;
    int rightHorizontalLineTrackerPin;
    int rightBackLineTrackerPin;
    int rightBackMostLineTrackerPin;

    
};
