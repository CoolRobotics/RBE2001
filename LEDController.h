class LEDController {
  public:
    void setup();
    void redOn();
    void greenOn();
    void yellowOn();
    void allOff();
    
  private:
    const int FLASH_PERIOD = 500;
    int redLEDPin; 
    int greenLEDPin;
    int yellowLEDPin;
    bool redState;
    bool greenState;
    bool yellowState;
    unsigned long redTime;
    unsigned long greenTime;
    unsigned long yellowTime;
};

