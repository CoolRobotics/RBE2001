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
    bool isFrontCross();
    bool isCross();
    bool isRotationDone(int dstDegree);
    void initRotation(Direction direction);
    void initLineCounter();
    void goBackward();
    void stop();
    bool isLine();
    void goStraightLine();
    void followLineWeak();
    bool isNintyDone(Direction direction);
    bool isReachingContainer();
    void initCrossDetection();

  private:
    // motors
    Servo leftMotor;
    Servo rightMotor;

    int oldDegree;

    int oldColor;
    int numLinePassed;

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

