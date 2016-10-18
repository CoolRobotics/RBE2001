#include "Arduino.h"
#include "ActionController.h"
//#include "LCDController.h"


LCDController lcdCtrl;

void BluetoothController::setup(ActionController *actionCtrl) {
  msg.setup();
  lcdCtrl.setup();

  timeForHeartbeat = millis() + 1000;
  timeForAlert = millis() + 1000;
  this->actionCtrl = actionCtrl;
}

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
        actionCtrl->onStorageChange(msg.getStorage());
        break;
      case Messages::kSupplyAvailability:
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

void BluetoothController::sendAlert(bool isNew) {
  if (millis() > timeForAlert) {
    timeForAlert = millis() + 1000;
    msg.sendAlert(isNew);
  }
}

