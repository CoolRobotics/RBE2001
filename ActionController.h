#include <QueueList.h>
#include "MovementController.h"
#include "ArmController.h"
#include "GrabberController.h"
#include "LCDController.h"

// Action classes
class Action {
  public:
    void (*init)(MovementController*, ArmController*, GrabberController*);
    bool (*isFinished)(MovementController*, ArmController*, GrabberController*);
    void (*act)(MovementController*, ArmController*, GrabberController*);
};

// Movement Actions
class LeaveReactorAction: public Action {
  public:
    LeaveReactorAction();
};

class RotateOneEightyAction: public Action {
  public:
    RotateOneEightyAction();
};

class FollowLineToFieldAction: public Action {
  public:
    FollowLineToFieldAction();
};

class RotateLeftNintyAction: public Action {
  public:
    RotateLeftNintyAction();
};

class RotateRightNintyAction: public Action {
  public:
    RotateRightNintyAction();
};

class RotateLeftNintyEdgeAction: public Action {
  public:
    RotateLeftNintyEdgeAction();
};

class RotateRightNintyEdgeAction: public Action {
  public:
    RotateRightNintyEdgeAction();
};

class FollowLineToContainerAction: public Action {
  public:
    FollowLineToContainerAction();
};

class LeaveContainerAction: public Action {
  public:
    LeaveContainerAction();
};

class FollowLineAction: public Action {
  public:
    FollowLineAction();
};

class StopMovingAction: public Action {
  public:
    StopMovingAction();
};

// Arm Actions
class DropDownArmAction: public Action {
  public:
    DropDownArmAction();
};

class LiftUpArmAction: public Action {
  public:
    LiftUpArmAction();
};

// Grabber Actions
class GrabAction: public Action {
  public:
    GrabAction();
};

class ReleaseAction: public Action {
  public:
    ReleaseAction();
};

class RotateDownGrabberAction: public Action {
  public:
    RotateDownGrabberAction();
};

class RotateUpGrabberAction: public Action {
  public:
    RotateUpGrabberAction();
};

class RotateDownGrabberAtReactorAction: public Action {
  public:
    RotateDownGrabberAtReactorAction();
};

class Location {
  public:
    typedef enum {
      storage,
      supply,
      reactorA,
      reactorB
    } Target;

    void set(Target t, int n);

    Target target;
    int num;
};

// Action controller
class ActionController {
  public:
    void setup();
    void act();
    void start(Location::Target t, int n);
    void to(Location::Target t, int n);

  private:
    typedef enum {
      initActionStage,
      performActionStage,
      nextActionStage
    } ActionStage;

    // Locations
    Location currLoc;
    Location dest;
    
    Action currAction;
    ActionStage currActionStage;
    QueueList<Action> actions;

    // Action controllers
    MovementController mvCtrl;
    ArmController armCtrl;
    GrabberController grabberCtrl;

    // List of predefined actions
    // Movement Actions
    LeaveReactorAction leaveReactorAction;
    RotateOneEightyAction rotateOneEightyAction;
    FollowLineToFieldAction followLineToFieldAction;
    RotateLeftNintyAction rotateLeftNintyAction;
    RotateRightNintyAction rotateRightNintyAction;
    RotateLeftNintyEdgeAction rotateLeftNintyEdgeAction;
    RotateRightNintyEdgeAction rotateRightNintyEdgeAction;
    FollowLineToContainerAction followLineToContainerAction;
    LeaveContainerAction leaveContainerAction;
    FollowLineAction followLineAction;
    StopMovingAction stopMovingAction;
    // Arm Actions
    DropDownArmAction dropDownArmAction;
    LiftUpArmAction liftUpArmAction;
    // Grabber Actions
    GrabAction grabAction;
    ReleaseAction releaseAction;
    RotateDownGrabberAction rotateDownGrabberAction;
    RotateUpGrabberAction rotateUpGrabberAction;
    RotateDownGrabberAtReactorAction rotateDownGrabberAtReactorAction;
    // Aggregated actions
    void addGrabFromReactorAtions();
    void addLeaveReactorToFieldAtions();
    void addReleaseAtReactorActions();
    void addGrabFromSupplyActions();
    void addFollowLines(int n);
    // Generate actions based on source and destination
    void addActionsToDest();
    char* getLocationStr(Location loc);
    // LCD
    LCDController lcdCtrl;
};

