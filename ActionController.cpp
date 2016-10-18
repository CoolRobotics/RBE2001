#include "ActionController.h"

/**
   Field map
                             Storage

                        4    3    2    1
                        |    |    |    |
                        |    |    |    |
   Reactor A ------------------------------------- Reactor B
                        |    |    |    |
                        |    |    |    |
                        1    2    3    4

                             Supply

                        4   3     2    1
*/
void ActionController::setup() {
  mvCtrl.setup();
  armCtrl.setup();
  grabberCtrl.setup();
//  lcdCtrl.setup();
  btCtrl.setup(this);

  currActionStage = nextActionStage;
  isActive = false;

  
}

void ActionController::act() {
  // Process Bluetooth actions
  // Send heartbeats to field
  btCtrl.sendHeartbeat();
  btCtrl.act();

  // Only move or grab rod when robot is active
  if (isActive)
    switch (currActionStage) {
      case nextActionStage:
        if (!actions.isEmpty()) {
          currAction = actions.pop();
          currActionStage = initActionStage;
        } else currLoc.set(dest.target, dest.num);
        break;
      case initActionStage:
        currAction.init(this);
        currActionStage = performActionStage;
        break;
      case performActionStage:
        if (currAction.isFinished(this)) currActionStage = nextActionStage;
        else currAction.act(this);
        break;
    } else mvCtrl.stop();
}

// Bluetooth controller outputs
void ActionController::onResume() {
  isActive = true;
}
void ActionController::onStop() {
  isActive = false;
}

void ActionController::start(Location::Target t, int n) {
  if (t == Location::reactorA || t == Location::reactorB)
    actions.push(followLineToContainerAction);
  dest.set(t, n);
}

void ActionController::to(Location::Target t, int n) {
  currLoc.set(dest.target, dest.num);
  dest.set(t, n);
  addActionsToDest();

}

void ActionController::addFollowLines(int n) {
  for (int i = 0; i < n; i++)
    actions.push(followLineAction);
}

/**
   Get label of target
*/
char* ActionController::getLocationStr(Location loc) {
  char str[17];
  switch (loc.target) {
    case Location::reactorA:
      sprintf(str, "%s", "Reactor A");
      break;
    case Location::reactorB:
      sprintf(str, "%s", "Reactor B");
      break;
    case Location::supply:
      sprintf(str, "%s %d", "Supply", loc.num);
      break;
    case Location::storage:
      sprintf(str, "%s %d", "Storage", loc.num);
      break;
  }
  return str;
}

void ActionController::addActionsToDest() {
  // Show source and destination
//  lcdCtrl.println(0, getLocationStr(currLoc));
//  lcdCtrl.println(1, getLocationStr(dest));

  if (currLoc.target == Location::reactorA ||
      currLoc.target == Location::reactorB) {
    if (dest.target == Location::storage)
      addGrabFromReactorAtions();
    addLeaveReactorToFieldAtions();
  }

  // Calculate corresponding turns and follow line actions
  int d;
  if (currLoc.target == Location::reactorA) {
    if (dest.target == Location::storage) {
      d = 4 - dest.num;
      addFollowLines(d);
      if (dest.num == 1) actions.push(rotateLeftNintyEdgeAction);
      else actions.push(rotateLeftNintyAction);
    } else if (dest.target == Location::supply) {
      d = dest.num - 1;
      addFollowLines(d);
      if (dest.num == 4) actions.push(rotateRightNintyEdgeAction);
      else actions.push(rotateRightNintyAction);
    }
  } else if (currLoc.target == Location::reactorB) {
    if (dest.target == Location::storage) {
      d = dest.num - 1;
      addFollowLines(d);
      if (dest.num == 4) actions.push(rotateRightNintyEdgeAction);
      else actions.push(rotateRightNintyAction);
    } else if (dest.target == Location::supply) {
      d = 4 - dest.num;
      addFollowLines(d);
      if (dest.num == 1) actions.push(rotateLeftNintyEdgeAction);
      else actions.push(rotateLeftNintyAction);
    }
  } else if (currLoc.target == Location::storage) {
    actions.push(leaveContainerAction);
    if (dest.target == Location::reactorA)
      if (currLoc.num == 4) actions.push(rotateLeftNintyEdgeAction);
      else actions.push(rotateLeftNintyAction);
    else if (dest.target == Location::reactorB)
      if (currLoc.num == 1) actions.push(rotateRightNintyEdgeAction);
      else actions.push(rotateRightNintyAction);
    else {
      int d;
      if (dest.target == Location::storage)
        d = dest.num - currLoc.num;
      else if (dest.target == Location::supply)
        d = 5 - dest.num - currLoc.num;

      if (d > 0) {
        actions.push(rotateLeftNintyAction);
        addFollowLines(d + 1);
        if (dest.target == Location::storage)
          if (dest.num == 4) actions.push(rotateRightNintyEdgeAction);
          else actions.push(rotateRightNintyAction);
        else if (dest.target == Location::supply)
          if (dest.num == 1) actions.push(rotateLeftNintyEdgeAction);
          else actions.push(rotateLeftNintyAction);
      } else if (d < 0) {
        actions.push(rotateRightNintyAction);
        addFollowLines(-d + 1);
        if (dest.target == Location::storage)
          if (dest.num == 1) actions.push(rotateLeftNintyEdgeAction);
          else actions.push(rotateLeftNintyAction);
        else if (dest.target == Location::supply)
          if (dest.num == 4) actions.push(rotateRightNintyEdgeAction);
          else actions.push(rotateRightNintyAction);
      } else if (dest.target == Location::supply) {
        if (dest.num == 4) {
          actions.push(rotateLeftNintyAction);
          actions.push(followLineAction);
          actions.push(rotateLeftNintyAction);
        } else {
          actions.push(rotateRightNintyAction);
          actions.push(followLineAction);
          actions.push(rotateRightNintyAction);
        }
      }
    }
  }
  else if (currLoc.target == Location::supply) {
    // Grab new fuel rod from supply
    if (dest.target == Location::reactorA ||
        dest.target == Location::reactorB)
      addGrabFromSupplyActions();

    actions.push(leaveContainerAction);
    if (dest.target == Location::reactorA)
      if (dest.num == 1) actions.push(rotateRightNintyEdgeAction);
      else actions.push(rotateRightNintyAction);
    else if (dest.target == Location::reactorB)
      if (dest.num == 4) actions.push(rotateLeftNintyEdgeAction);
      else actions.push(rotateLeftNintyAction);
    else {
      int d;
      if (dest.target == Location::supply)
        d = dest.num - currLoc.num;
      else if (dest.target == Location::storage)
        d = 5 - dest.num - currLoc.num;

      if (d > 0) {
        actions.push(rotateLeftNintyAction);
        addFollowLines(d + 1);
        if (dest.target == Location::storage)
          if (dest.num == 1) actions.push(rotateLeftNintyEdgeAction);
          else actions.push(rotateLeftNintyAction);
        else if (dest.target == Location::supply)
          if (dest.num == 4) actions.push(rotateRightNintyEdgeAction);
          else actions.push(rotateRightNintyAction);
      } else if (d < 0) {
        actions.push(rotateRightNintyAction);
        addFollowLines(-d + 1);
        if (dest.target == Location::storage)
          if (dest.num == 4) actions.push(rotateRightNintyEdgeAction);
          else actions.push(rotateRightNintyAction);
        else if (dest.target == Location::supply)
          if (dest.num == 1) actions.push(rotateLeftNintyEdgeAction);
          else actions.push(rotateLeftNintyAction);
      } else if (dest.target == Location::storage) {
        if (dest.num == 1) {
          actions.push(rotateLeftNintyAction);
          actions.push(followLineAction);
          actions.push(rotateLeftNintyAction);
        } else {
          actions.push(rotateRightNintyAction);
          actions.push(followLineAction);
          actions.push(rotateRightNintyAction);
        }
      }
    }
  }

  actions.push(followLineToContainerAction);

  // Release at storage
  if ((currLoc.target == Location::reactorA ||
       currLoc.target == Location::reactorB) &&
      dest.target == Location::storage)
    actions.push(releaseAction);

  // Release at reactor
  if (currLoc.target == Location::supply &&
      (dest.target == Location::reactorA ||
       dest.target == Location::reactorB))
    addReleaseAtReactorActions();
}

void ActionController::addGrabFromSupplyActions() {
  // Grab rod from supply
  actions.push(releaseAction);
  actions.push(liftUpArmAction);
  actions.push(rotateUpGrabberAction);
  actions.push(grabAction);
}

void ActionController::addGrabFromReactorAtions() {
  // Grab rod from reactor
  actions.push(releaseAction);
  actions.push(rotateDownGrabberAction);
  actions.push(dropDownArmAction);
  actions.push(grabAction);
  actions.push(liftUpArmAction);
  actions.push(rotateUpGrabberAction);
}

void ActionController::addLeaveReactorToFieldAtions() {
  // Leave to reactor to field
  actions.push(leaveReactorAction);
  actions.push(rotateOneEightyAction);
  actions.push(followLineToFieldAction);
}

void ActionController::addReleaseAtReactorActions() {
  // Release rod at reactor
  actions.push(rotateDownGrabberAtReactorAction);
  actions.push(dropDownArmAction);
  actions.push(releaseAction);
  actions.push(liftUpArmAction);
  actions.push(rotateUpGrabberAction);
}

// Movement Actions
LeaveReactorAction::LeaveReactorAction() {
  init = [](ActionController* actionCtrl) -> void {
    Serial.println("LeaveReactorAction init");
    actionCtrl->mvCtrl.initFrontLineCounter();
  };

  act = [](ActionController* actionCtrl) -> void {
    Serial.println("LeaveReactorAction act");
    actionCtrl->mvCtrl.goBackward();
  };

  isFinished = [](ActionController* actionCtrl) -> bool {
    Serial.println("LeaveReactorAction isFinished");
    return actionCtrl->mvCtrl.isFrontCross(1);
  };
}

RotateOneEightyAction::RotateOneEightyAction() {
  init = [](ActionController* actionCtrl) -> void {
    Serial.println("RotateOneEightyAction init");
    actionCtrl->mvCtrl.initRotation(MovementController::LEFT_DIRECTION);
  };

  act = [](ActionController* actionCtrl) -> void {
    Serial.println("RotateOneEightyAction act");
    actionCtrl->mvCtrl.rotateCCW();
  };

  isFinished = [](ActionController* actionCtrl) -> bool {
    Serial.println("RotateOneEightyAction isFinished");
    return actionCtrl->mvCtrl.isRotationDone(180);
  };
}

FollowLineToFieldAction::FollowLineToFieldAction() {
  init = [](ActionController* actionCtrl) -> void {
    Serial.println("FollowLineToFieldAction init");
    actionCtrl->mvCtrl.initLineCounter();
    actionCtrl->mvCtrl.initFrontLineCounter();
  };

  act = [](ActionController* actionCtrl) -> void {
    Serial.println("FollowLineToFieldAction act");
    actionCtrl->mvCtrl.followLine();
  };

  isFinished = [](ActionController* actionCtrl) -> bool {
    Serial.print("FollowLineToFieldAction isFinished");
    return actionCtrl->mvCtrl.hasFrontCross() && actionCtrl->mvCtrl.isCross(1);
  };
}

RotateLeftNintyAction::RotateLeftNintyAction() {
  init = [](ActionController* actionCtrl) -> void {
    Serial.println("RotateLeftNintyAction init");
    actionCtrl->mvCtrl.initRotation(MovementController::LEFT_DIRECTION);
  };

  act = [](ActionController* actionCtrl) -> void {
    Serial.println("RotateLeftNintyAction act");
    actionCtrl->mvCtrl.rotateCCW();
  };

  isFinished = [](ActionController* actionCtrl) -> bool {
    Serial.println("RotateLeftNintyAction isFinished");
    return actionCtrl->mvCtrl.isNintyDone(MovementController::LEFT_DIRECTION);
  };
}

RotateRightNintyAction::RotateRightNintyAction() {
  init = [](ActionController* actionCtrl) -> void {
    Serial.println("RotateRightNintyA init");
    actionCtrl->mvCtrl.initRotation(MovementController::RIGHT_DIRECTION);
  };

  act = [](ActionController* actionCtrl) -> void {
    Serial.println("RotateRightNintyA act");
    actionCtrl->mvCtrl.rotateCW();
  };

  isFinished = [](ActionController* actionCtrl) -> bool {
    Serial.println("RotateRightNintyA isFinished");
    return actionCtrl->mvCtrl.isNintyDone(MovementController::RIGHT_DIRECTION);
  };
}

RotateLeftNintyEdgeAction::RotateLeftNintyEdgeAction() {
  init = [](ActionController* actionCtrl) -> void {
    Serial.println("RotateLeftNintyEdgeAction init");
    actionCtrl->mvCtrl.initRotation(MovementController::LEFT_DIRECTION);
  };

  act = [](ActionController* actionCtrl) -> void {
    Serial.println("RotateLeftNintyEdgeAction act");
    actionCtrl->mvCtrl.rotateCCW();
  };

  isFinished = [](ActionController* actionCtrl) -> bool {
    Serial.println("RotateLeftNintyEdgeAction isFinished");
    return actionCtrl->mvCtrl.isNintyDoneEdge(MovementController::LEFT_DIRECTION);
  };
}

RotateRightNintyEdgeAction::RotateRightNintyEdgeAction() {
  init = [](ActionController* actionCtrl) -> void {
    Serial.println("RotateRightNintyEdgeAction init");
    actionCtrl->mvCtrl.initRotation(MovementController::RIGHT_DIRECTION);
  };

  act = [](ActionController* actionCtrl) -> void {
    Serial.println("RotateRightNintyEdgeAction act");
    actionCtrl->mvCtrl.rotateCW();
  };

  isFinished = [](ActionController* actionCtrl) -> bool {
    Serial.println("RotateRightNintyEdgeAction isFinished");
    return actionCtrl->mvCtrl.isNintyDoneEdge(MovementController::RIGHT_DIRECTION);
  };
}

FollowLineToContainerAction::FollowLineToContainerAction() {
  init = [](ActionController* actionCtrl) -> void {
    Serial.println("FollowLineToContainerAction init");
  };

  act = [](ActionController* actionCtrl) -> void {
    Serial.println("FollowLineToContainerAction act");
    actionCtrl->mvCtrl.followLine();
  };

  isFinished = [](ActionController* actionCtrl) -> bool {
    Serial.print("FollowLineToContainerAction isFinished");
    return actionCtrl->mvCtrl.isReachingContainer();
  };
}

LeaveContainerAction::LeaveContainerAction() {
  init = [](ActionController* actionCtrl) -> void {
    Serial.println("LeaveContainerAction init");
    actionCtrl->mvCtrl.initLineCounter();
  };

  act = [](ActionController* actionCtrl) -> void {
    Serial.println("LeaveContainerLeftAction act");
    actionCtrl->mvCtrl.goBackward();
  };

  isFinished = [](ActionController* actionCtrl) -> bool {
    Serial.println("LeaveContainerLeftAction isFinished");
    return actionCtrl->mvCtrl.isCross(1);
  };
}

FollowLineAction::FollowLineAction() {
  init = [](ActionController* actionCtrl) -> void {
    Serial.println("FollowLineAction init");
    actionCtrl->mvCtrl.initLineCounter();
  };

  act = [](ActionController* actionCtrl) -> void {
    Serial.println("FollowLineAction act");
    actionCtrl->mvCtrl.followLine();
  };

  isFinished = [](ActionController* actionCtrl) -> bool {
    Serial.println("FollowLineAction isFinished");
    return actionCtrl->mvCtrl.isCross(1);
  };
}

// Arm Actions
DropDownArmAction::DropDownArmAction() {
  init = [](ActionController* actionCtrl) -> void {
    Serial.println("DropDownArmAction init");
  };

  act = [](ActionController* actionCtrl) -> void {
    Serial.println("DropDownArmAction act");
    actionCtrl->armCtrl.dropDown();
  };

  isFinished = [](ActionController* actionCtrl) -> bool {
    Serial.println("DropDownArmAction isFinished");
    return actionCtrl->armCtrl.isDropDownFinish();
  };
}

LiftUpArmAction::LiftUpArmAction() {
  init = [](ActionController* actionCtrl) -> void {
    Serial.println("LiftUpArmAction init");
  };

  act = [](ActionController* actionCtrl) -> void {
    Serial.println("LiftUpArmAction act");
    actionCtrl->armCtrl.liftUp();
  };

  isFinished = [](ActionController* actionCtrl) -> bool {
    Serial.println("LiftUpArmAction isFinished");
    return actionCtrl->armCtrl.isLiftedUp();
  };
}

// Grabber Actions
GrabAction::GrabAction() {
  init = [](ActionController* actionCtrl) -> void {
    Serial.println("GrabAction init");
  };

  act = [](ActionController* actionCtrl) -> void {
    Serial.println("GrabAction act");
    actionCtrl->grabberCtrl.grab();
  };

  isFinished = [](ActionController* actionCtrl) -> bool {
    Serial.println("GrabAction isFinished");
    return actionCtrl->grabberCtrl.isGrabFinished();
  };
}

ReleaseAction::ReleaseAction() {
  init = [](ActionController* actionCtrl) -> void {
    Serial.println("ReleaseAction init");
  };

  act = [](ActionController* actionCtrl) -> void {
    Serial.println("ReleaseAction act");
    actionCtrl->grabberCtrl.release();
  };

  isFinished = [](ActionController* actionCtrl) -> bool {
    Serial.println("ReleaseAction isFinished");
    return actionCtrl->grabberCtrl.isReleaseFinished();
  };
}


RotateDownGrabberAction::RotateDownGrabberAction() {
  init = [](ActionController* actionCtrl) -> void {
    Serial.println("RotateDownGrabberAction init");
  };

  act = [](ActionController* actionCtrl) -> void {
    Serial.println("RotateDownGrabberAction act");
    actionCtrl->grabberCtrl.rotateDownGrabber();
  };

  isFinished = [](ActionController* actionCtrl) -> bool {
    Serial.println("RotateDownGrabberAction isFinished");
    return actionCtrl->grabberCtrl.isRotateDownGrabberDone();
  };
}

RotateUpGrabberAction::RotateUpGrabberAction() {
  init = [](ActionController* actionCtrl) -> void {
    Serial.println("RotateUpGrabberAction init");
  };

  act = [](ActionController* actionCtrl) -> void {
    Serial.println("RotateUpGrabberAction act");
    actionCtrl->grabberCtrl.rotateUpGrabber();
  };

  isFinished = [](ActionController* actionCtrl) -> bool {
    Serial.println("RotateUpGrabberAction isFinished");
    return actionCtrl->grabberCtrl.isRotateUpGrabberDone();
  };
}

RotateDownGrabberAtReactorAction::RotateDownGrabberAtReactorAction() {
  init = [](ActionController* actionCtrl) -> void {
    Serial.println("RotateDownGrabberAtReactorAction init");
  };

  act = [](ActionController* actionCtrl) -> void {
    Serial.println("RotateDownGrabberAtReactorAction act");
    actionCtrl->grabberCtrl.rotateDownGrabberAtReactor();
  };

  isFinished = [](ActionController* actionCtrl) -> bool {
    Serial.println("RotateDownGrabberAtReactorAction isFinished");
    return actionCtrl->grabberCtrl.isRotateDownGrabberAtReactorDone();
  };
}

StopMovingAction::StopMovingAction() {
  init = [](ActionController* actionCtrl) -> void {
    Serial.println("StopMovingAction init");
  };

  act = [](ActionController* actionCtrl) -> void {
    Serial.println("StopMovingAction act");
    actionCtrl->mvCtrl.stop();
  };

  isFinished = [](ActionController* actionCtrl) -> bool {
    Serial.println("StopMovingAction isFinished");
    return actionCtrl->mvCtrl.isStopped();
  };
}

void Location::set(Target t, int n) {
  target = t;
  num = n;
}

