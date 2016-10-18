#include "Arduino.h"
#include "ActionController.h"

void BluetoothController::setup(ActionController *actionCtrl) {
  msg.setup();
  timeForHeartbeat = millis() + 1000;
  this->actionCtrl = actionCtrl;
}

//        kStorageAvailability,
//                  kSupplyAvailability,
//                  kRadiationAlert,
//                  kStopMovement,
//                  kResumeMovement,
//                  kRobotStatus,
//                  kHeartbeat
void BluetoothController::act() {
  if (msg.read()) {
    switch (msg.getMessageType()) {
      case Messages::kStopMovement:
        // Received "Stop" message
        actionCtrl->onStop();
        break;
      case Messages::kResumeMovement:
        // Received "Resume" message
        actionCtrl->onResume();
        break;
      case Messages::kStorageAvailability:
        
        break;
      case Messages::kSupplyAvailability:
        break;
    }
  }
}

void BluetoothController::sendHeartbeat() {
  if (millis() > timeForHeartbeat) {
    timeForHeartbeat = millis() + 1000;
    msg.sendHeartbeat();
  }
}

