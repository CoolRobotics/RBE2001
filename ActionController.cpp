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
  lcdCtrl.setup();

  currActionStage = nextActionStage;
}

void ActionController::start(Location::Target t, int n) {
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
  lcdCtrl.println(0, getLocationStr(currLoc));
  lcdCtrl.println(1, getLocationStr(dest));

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

void ActionController::act() {
  switch (currActionStage) {
    case nextActionStage:
      if (!actions.isEmpty()) {
        currAction = actions.pop();
        currActionStage = initActionStage;
      } else currLoc.set(dest.target, dest.num);
      break;
    case initActionStage:
      currAction.init(&mvCtrl, &armCtrl, &grabberCtrl);
      currActionStage = performActionStage;
      break;
    case performActionStage:
      if (currAction.isFinished(&mvCtrl, &armCtrl, &grabberCtrl)) currActionStage = nextActionStage;
      else currAction.act(&mvCtrl, &armCtrl, &grabberCtrl);
      break;
  }
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
  init = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("LeaveReactorAction init");
    mvCtrl->initFrontLineCounter();
  };

  act = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("LeaveReactorAction act");
    mvCtrl->goBackward();
  };

  isFinished = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> bool {
    Serial.println("LeaveReactorAction isFinished");
    return mvCtrl->isFrontCross(1);
  };
}

RotateOneEightyAction::RotateOneEightyAction() {
  init = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("RotateOneEightyAction init");
    mvCtrl->initRotation(MovementController::LEFT_DIRECTION);
  };

  act = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("RotateOneEightyAction act");
    mvCtrl->rotateCCW();
  };

  isFinished = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> bool {
    Serial.println("RotateOneEightyAction isFinished");
    return mvCtrl->isRotationDone(180);
  };
}

FollowLineToFieldAction::FollowLineToFieldAction() {
  init = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("FollowLineToFieldAction init");
    mvCtrl->initLineCounter();
    mvCtrl->initFrontLineCounter();
  };

  act = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("FollowLineToFieldAction act");
    mvCtrl->followLine();
  };

  isFinished = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> bool {
    Serial.print("FollowLineToFieldAction isFinished");
    return mvCtrl->hasFrontCross() && mvCtrl->isCross(1);
  };
}

RotateLeftNintyAction::RotateLeftNintyAction() {
  init = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("RotateLeftNintyAction init");
    mvCtrl->initRotation(MovementController::LEFT_DIRECTION);
  };

  act = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("RotateLeftNintyAction act");
    mvCtrl->rotateCCW();
  };

  isFinished = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> bool {
    Serial.println("RotateLeftNintyAction isFinished");
    return mvCtrl->isNintyDone(MovementController::LEFT_DIRECTION);
  };
}

RotateRightNintyAction::RotateRightNintyAction() {
  init = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("RotateRightNintyA init");
    mvCtrl->initRotation(MovementController::RIGHT_DIRECTION);
  };

  act = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("RotateRightNintyA act");
    mvCtrl->rotateCW();
  };

  isFinished = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> bool {
    Serial.println("RotateRightNintyA isFinished");
    return mvCtrl->isNintyDone(MovementController::RIGHT_DIRECTION);
  };
}

RotateLeftNintyEdgeAction::RotateLeftNintyEdgeAction() {
  init = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("RotateLeftNintyEdgeAction init");
    mvCtrl->initRotation(MovementController::LEFT_DIRECTION);
  };

  act = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("RotateLeftNintyEdgeAction act");
    mvCtrl->rotateCCW();
  };

  isFinished = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> bool {
    Serial.println("RotateLeftNintyEdgeAction isFinished");
    return mvCtrl->isNintyDoneEdge(MovementController::LEFT_DIRECTION);
  };
}

RotateRightNintyEdgeAction::RotateRightNintyEdgeAction() {
  init = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("RotateRightNintyEdgeAction init");
    mvCtrl->initRotation(MovementController::RIGHT_DIRECTION);
  };

  act = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("RotateRightNintyEdgeAction act");
    mvCtrl->rotateCW();
  };

  isFinished = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> bool {
    Serial.println("RotateRightNintyEdgeAction isFinished");
    return mvCtrl->isNintyDoneEdge(MovementController::RIGHT_DIRECTION);
  };
}

FollowLineToContainerAction::FollowLineToContainerAction() {
  init = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("FollowLineToContainerAction init");
  };

  act = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("FollowLineToContainerAction act");
    mvCtrl->followLine();
  };

  isFinished = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> bool {
    Serial.print("FollowLineToContainerAction isFinished");
    return mvCtrl->isReachingContainer();
  };
}

LeaveContainerAction::LeaveContainerAction() {
  init = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("LeaveContainerAction init");
    mvCtrl->initLineCounter();
  };

  act = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("LeaveContainerLeftAction act");
    mvCtrl->goBackward();
  };

  isFinished = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> bool {
    Serial.println("LeaveContainerLeftAction isFinished");
    return mvCtrl->isCross(1);
  };
}

FollowLineAction::FollowLineAction() {
  init = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("FollowLineAction init");
    mvCtrl->initLineCounter();
  };

  act = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("FollowLineAction act");
    mvCtrl->followLine();
  };

  isFinished = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> bool {
    Serial.println("FollowLineAction isFinished");
    return mvCtrl->isCross(1);
  };
}

// Arm Actions
DropDownArmAction::DropDownArmAction() {
  init = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("DropDownArmAction init");
  };

  act = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("DropDownArmAction act");
    armCtrl->dropDown();
  };

  isFinished = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> bool {
    Serial.println("DropDownArmAction isFinished");
    return armCtrl->isDropDownFinish();
  };
}

LiftUpArmAction::LiftUpArmAction() {
  init = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("LiftUpArmAction init");
  };

  act = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("LiftUpArmAction act");
    armCtrl->liftUp();
  };

  isFinished = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> bool {
    Serial.println("LiftUpArmAction isFinished");
    return armCtrl->isLiftedUp();
  };
}

// Grabber Actions
GrabAction::GrabAction() {
  init = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("GrabAction init");
  };

  act = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("GrabAction act");
    grabberCtrl->grab();
  };

  isFinished = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> bool {
    Serial.println("GrabAction isFinished");
    return grabberCtrl->isGrabFinished();
  };
}

ReleaseAction::ReleaseAction() {
  init = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("ReleaseAction init");
  };

  act = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("ReleaseAction act");
    grabberCtrl->release();
  };

  isFinished = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> bool {
    Serial.println("ReleaseAction isFinished");
    return grabberCtrl->isReleaseFinished();
  };
}


RotateDownGrabberAction::RotateDownGrabberAction() {
  init = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("RotateDownGrabberAction init");
  };

  act = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("RotateDownGrabberAction act");
    grabberCtrl->rotateDownGrabber();
  };

  isFinished = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> bool {
    Serial.println("RotateDownGrabberAction isFinished");
    return grabberCtrl->isRotateDownGrabberDone();
  };
}

RotateUpGrabberAction::RotateUpGrabberAction() {
  init = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("RotateUpGrabberAction init");
  };

  act = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("RotateUpGrabberAction act");
    grabberCtrl->rotateUpGrabber();
  };

  isFinished = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> bool {
    Serial.println("RotateUpGrabberAction isFinished");
    return grabberCtrl->isRotateUpGrabberDone();
  };
}

RotateDownGrabberAtReactorAction::RotateDownGrabberAtReactorAction() {
  init = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("RotateDownGrabberAtReactorAction init");
  };

  act = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("RotateDownGrabberAtReactorAction act");
    grabberCtrl->rotateDownGrabberAtReactor();
  };

  isFinished = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> bool {
    Serial.println("RotateDownGrabberAtReactorAction isFinished");
    return grabberCtrl->isRotateDownGrabberAtReactorDone();
  };
}

StopMovingAction::StopMovingAction() {
  init = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("StopMovingAction init");
  };

  act = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> void {
    Serial.println("StopMovingAction act");
    mvCtrl->stop();
  };

  isFinished = [](MovementController * mvCtrl, ArmController * armCtrl, GrabberController * grabberCtrl) -> bool {
    Serial.println("StopMovingAction isFinished");
    return mvCtrl->isStopped();
  };
}

void Location::set(Target t, int n) {
  target = t;
  num = n;
}

