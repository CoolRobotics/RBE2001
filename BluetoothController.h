#include "Messages.h"

class ActionController;

class BluetoothController {
  public:
    void setup(ActionController *actionCtrl);
    void sendHeartbeat();
    void act();
  private:
    Messages msg;
    unsigned long timeForHeartbeat;
    ActionController *actionCtrl;
};

