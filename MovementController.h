#include <Servo.h>
#include <Encoder.h>

class MovementController {
  public:
    MovementController();
    void setup();
    void followLine();
    void rotateCW();
    void rotateCCW();
    bool isFrontCross();
    bool isCross();
    bool isRotationDone(int dstDegree);
    void initRotation();
    void goBackward();
    void stop();
    bool isLine();
    void goStraightLine();
    void followLineWeak();
  
  private:
    // motors
    Servo leftMotor;
    Servo rightMotor;
    
    int oldDegree;

    // left side
    int leftMostLineTrackerPin;
    int leftMoreLineTrackerPin;
    int leftLineTrackerPin;
    int leftCrossLineTackerPin;

    // right side
    int rightMostLineTrackerPin;
    int rightMoreLineTrackerPin;
    int rightLineTrackerPin;
    int rightCrossLineTrackerPin;
};

