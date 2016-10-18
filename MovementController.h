#include <Servo.h>
#include <Encoder.h>

class MovementController {
  public:
    typedef enum {
      LEFT_DIRECTION,
      RIGHT_DIRECTION
    } Direction;
    MovementController();
    void setup();
    void followLine();
    void rotateCW();
    void rotateCCW();
    bool isFrontCross(int num);
    bool hasFrontCross();
    bool isCross(int num);
    bool isRotationDone(int dstDegree);
    void initRotation(Direction direction);
    void initLineCounter();
    void goBackward();
    bool isLine();
    void followLineWeak();
    bool isNintyDone(Direction direction);
    bool isNintyDoneEdge(Direction direction);
    int isNintyDoneShare(Direction direction);
    bool isReachingContainer();
    void initFrontLineCounter();
    void stop();
    bool MovementController::isStopped();

  private:
    // motors
    Servo leftMotor;
    Servo rightMotor;

    int oldDegree;

    int oldColor;
    int numLinePassed;

    int frontLeftOldColor;
    int frontRightOldColor;
    int numFrontLinePassed;

    // left side
    int leftMostLineTrackerPin;
    int leftMoreLineTrackerPin;
    int leftLineTrackerPin;
    int leftCrossLineTrackerPin;

    // right side
    int rightMostLineTrackerPin;
    int rightMoreLineTrackerPin;
    int rightLineTrackerPin;
    int rightCrossLineTrackerPin;
    
    // limit switch
    int frontLimitSwitchPin;
};

