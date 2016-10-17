#include "Servo.h"

class GrabberController {
  public:
    void setup();
    void grab();
    void release();
    bool isGrabFinished();
    bool isReleaseFinished();
    void rotateDownGrabber();
    bool isRotateDownGrabberDone();
    void rotateUpGrabber();
    bool isRotateUpGrabberDone();
    void rotateDownGrabberAtReactor();
    bool isRotateDownGrabberAtReactorDone();
  private:
    Servo grabberServo;
    Servo transPlaneServo;
};

