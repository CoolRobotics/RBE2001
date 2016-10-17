#include "ActionController.h"

//Messages msg;
unsigned long timeForHeartbeat;

// Keep track of current action
ActionController actionCtrl;

/**
   Initialize the messages class and the debug serial port
*/
void setup() {

  // Update our Field Element variables with the status of the field
  //  fieldStatus();
  Serial.begin(115200);

  actionCtrl.setup();
}

/**
   The loop method runs continuously, so call the Messages read method to check for input
   on the bluetooth serial port and parse any complete messages. Then send the heartbeat if
   it's time to do that.

   For the final project, one good way of implementing it is to use a state machine with high
   level tasks as states. The state will tell what you should be doing each time the loop
   function is called.

   Refer to the state machine lecture or look at the BTComms class for an example on how to
   implement state machines.
*/
void loop() {
  actionCtrl.act();
  //  messageUpdate();
  //  heartbeatUpdate();
  //  robotMain();
  //  LEDs();
  //  movementController.followForward();
  //  robotMain();
}

//void messageUpdate() {
//  if (msg.read()) {
//    switch (msg.getMessageType()) {
//      case Messages::kStopMovement:
//        // Received "Stop" message
//        lcd.print(1, "Stop");
//        break;
//      case Messages::kResumeMovement:
//        // Received "Resume" message
//        lcd.print(1, "Resume");
//        break;
//    }
//  }
//}
//
//void heartbeatUpdate() {
//  if (millis() > timeForHeartbeat) {
//    timeForHeartbeat = millis() + 1000;
//    msg.sendHeartbeat();
//  }
//}
