/*
   Messages.cpp
*/
#include "Arduino.h"
#include "Messages.h"
#include "BTComms.h"

BTComms comms;
Messages::MessageType msgType;


/**
   Constuctor
   Initialize everything here when the class is created
   Note: you cannot call methods that depend on other classes having already been created
*/
Messages::Messages() {
  stopped = true;
  me = 0x10;
  all = 0x00;
  supply = 0x0;
  storage = 0x0;
}

/**
   Setup class code that is called from the Arduino sketch setup() function. This doesn't
   get called until all the other classes have been created.
*/
void Messages::setup() {
  comms.setup();
  // Receiver

}

/**
   Check if the field is currently in the "stop" state
   @returns bool value that is true if the robot should be stopped
*/
bool Messages::isStopped() {
  return stopped;
}

/**
   Send a heartbeat message to the field to let it know that your code is alive
   This should be called by your robot program periodically, say once per second. This
   timing can easily be done in the loop() function of your program.
*/
void Messages::sendHeartbeat() {
  comms.writeMessage(kHeartbeat, me, all);
}

/**
   Print message for debugging
   This method prints the message as a string of hex numbers strictly for debugging
   purposes and is not required to be called for any other purpose.
*/
void Messages::printMessage() {
  for (int i = 0; i < comms.getMessageLength(); i++) {
    Serial.print(comms.getMessageByte(i), HEX);
    Serial.print(" ");
  }
  Serial.println();
}

bool Messages::isToMe() {
  unsigned int dest = comms.getMessageByte(2);
  return dest == me || dest == all;
}

/**
   Read messages from the Bluetooth serial connection
   This method should be called from the loop() function in your arduino code. It will check
   to see if the lower level comms object has received a complete message, and run the appropriate
   code to handle the message type. This should just save the state of the message inside this class
   inside member variables. Then add getters/setters to retrieve the status from your program.
*/
bool Messages::read() {
  int data_mask = 0xf;
  if (comms.read() && isToMe()) {
    switch (comms.getMessageByte(0)) {
      case kStorageAvailability:
        setMessageType(kStorageAvailability);
        storage = comms.getMessageByte(3) & data_mask;
        break;
      case kSupplyAvailability:
        setMessageType(kSupplyAvailability);
        supply = comms.getMessageByte(3) & data_mask;
        break;
      case kRadiationAlert:
        setMessageType(kRadiationAlert);
        break;
      case kStopMovement:
        setMessageType(kStopMovement);
        break;
      case kResumeMovement:
        setMessageType(kResumeMovement);
        break;
      case kRobotStatus:
        setMessageType(kRobotStatus);
        break;
      case kHeartbeat:
        setMessageType(kHeartbeat);
        break;
    }
    return true;
  }
  return false;
}

byte Messages::getSupply() {
  return supply;
}

byte  Messages::getStorage() {
  return storage;
}

void Messages::setMessageType(Messages::MessageType type) {
  msgType = type;
}

Messages::MessageType Messages::getMessageType() {
  return msgType;
}
