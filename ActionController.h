#include <QueueList.h>
#include "MovementController.h"
#include "ArmController.h"
#include "GrabberController.h"
#include "LCDController.h"
#include "BluetoothController.h"
#include "LEDController.h"

// Action classes
class Action {
  public:
    void (*init)(ActionController*);
    bool (*isFinished)(ActionController*);
    void (*act)(ActionController*);
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

// status actions
class NewFuelStatusAction: public Action {
  public:
    NewFuelStatusAction();
};

class SpentFuelStatusAction: public Action {
  public:
    SpentFuelStatusAction();
};

class NoFuelStatusAction: public Action {
  public:
    NoFuelStatusAction();
};

// reactor actions
class StartReactionAction: public Action {
  public:
    StartReactionAction();
};

class StopReactionAction: public Action {
  public:
    StopReactionAction();
};

// generate actions
class GenerateAction: public Action {
  public:
    GenerateAction();
};

class Location {
  public:
    typedef enum {
      reactorA,
      reactorB,
      storage,
      supply,
    } Target;

    void set(Target t, int n);

    Target target;
    int num;
};

// Action controller
class ActionController {
  public:
    typedef enum {
      newFuel,
      spentFuel,
      noFuel
    } Status;

    Status status;

    // Action controllers
    MovementController mvCtrl;
    ArmController armCtrl;
    GrabberController grabberCtrl;
    BluetoothController btCtrl;
    LEDController ledCtrl;
    // LCD
    LCDController lcdCtrl;
    void setup();
    void act();
    void start(Location::Target t, int n);
    void to(Location::Target t, int n);
    // Bluetooth controller outputs
    void onResume();
    void onStop();
    void onStorageChange(byte storageState);
    void onSupplyChange(byte supplyState);
    bool storage[4];
    bool supply[4];

    Location::Target currReactor;
    bool generatingAction;
    // 0 for reactor A and 1 for reactor B
    bool reacting[2];
    unsigned long reactionBegin[2];
    bool isReactionDone(unsigned long reactorTime);
    // Locations
    Location currLoc;
    Location dest;
    bool isActive;

    char* getLocationStr(Location loc);

    // Should all be private
    QueueList<Action> actions;
    // Status action
    NewFuelStatusAction newFuelStatusAction;
    SpentFuelStatusAction spentFuelStatusAction;
    NoFuelStatusAction noFuelStatusAction;
    // Reactor action
    StartReactionAction startReactionAction;
    StopReactionAction stopReactionAction;
    // Generate action
    GenerateAction generateAction;

  private:

    typedef enum {
      initActionStage,
      performActionStage,
      nextActionStage
    } ActionStage;

    Action currAction;
    ActionStage currActionStage;

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
};

