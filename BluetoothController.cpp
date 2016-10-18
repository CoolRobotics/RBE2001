#include "Arduino.h"
#include "ActionController.h"
#include "LCDController.h"


LCDController lcdCtrl;

void BluetoothController::setup(ActionController *actionCtrl) {
  msg.setup();
  lcdCtrl.setup();
  
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
  char str[5];
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
        actionCtrl->onStorageChange(msg.getStorage());
        break;
      case Messages::kSupplyAvailability:
//        sprintf(str, "%d", msg.getSupply());
//        lcdCtrl.println(1, str);
        actionCtrl->onSupplyChange(msg.getSupply());
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

