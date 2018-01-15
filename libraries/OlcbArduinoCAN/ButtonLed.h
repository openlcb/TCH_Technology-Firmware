#ifndef ButtonLed_h
#define ButtonLed_h



#include "Arduino.h"

class ButtonLed {
  private:
    long timeOfLastChange;    // time at whihc the button last changed
    int lastLEDPeriod;        // last period processed for LED 
    int lastButtonPeriod;     // last period processed for button
    bool lastState;           // state of button in last scan
    bool newState;            // state of button this scan
    //bool next, bnext;         // wait-next-timeslot flag in output loop
    void init(uint8_t s);     // init
	bool lastUState;
  public:
    uint16_t sense;            // active sense of button/LED 
    long pattern;             // current output drive pattern (rotates)
    long duration;            // how long the button has been in its current state in msec
    long lastDuration;        // how long it was in the previous state, in msec
    uint8_t pin;              // connected pin number
    uint8_t once;             // mask for single blink
    bool ledState;            // current output drive state
    bool state;               // current button state

    ButtonLed(uint8_t pin, uint8_t sense);  // associate pin and sense for active state
    ButtonLed(uint8_t pin);                 // associate pin, default sense=HIGH
	ButtonLed();
	void setPinSense(uint8_t p, uint8_t s);
    void on(long pattern);                  // set the output repeating pattern
    void blink(uint8_t pattern);            // set the output one-time blink pattern
    void process();                         // call periodically
	bool unique();
};

#endif 