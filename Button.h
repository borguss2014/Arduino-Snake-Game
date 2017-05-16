#ifndef Button_h
#define Button_h

#include "Arduino.h"

class Button
{
  public:
    Button(int pin);

    void checkStatus();

    void update();

    void onPress();
    void onRelease();

    void setOnClickListener(
      void (*clickFunction)(int),
      int direction
    );

  private:
    bool _pressed;
    bool _executed;
    bool _clickListenerSet;

    int _lastState;
    unsigned int _debounceDelay;
    unsigned long _lastPressed;
    int _buttonState;

    int _pin;

    int _direction;

    void (*_callback)(int);
};

#endif
