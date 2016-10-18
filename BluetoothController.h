#include "Messages.h"

class ActionController;

class BluetoothController {
  public:
    void setup(ActionController *actionCtrl);
    void sendHeartbeat();
    void sendAlert(bool isNew);
    void act();
  private:
    Messages msg;
    unsigned long timeForHeartbeat;
    unsigned long timeForAlert;
    ActionController *actionCtrl;
};

