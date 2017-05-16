#include "Arduino.h"
#include "Button.h"

Button::Button(int pin)
{
  _pressed          = false;
  _executed         = false;
  _clickListenerSet = false;
  _pin              = pin;

  _debounceDelay = 50;
  _lastPressed = 0;
  _lastState = LOW;

  pinMode(pin, INPUT_PULLUP);
}

//Read button status and debounce
void Button::checkStatus()
{
  int status = digitalRead(_pin);

  if(status != _lastState)
  {
    _lastPressed = millis();
  }

  if((millis() - _lastPressed) > _debounceDelay)
  {
    // if the button state has changed:
    if (status != _buttonState) {
      _buttonState = status;

      if(_buttonState == LOW)
      {
        _pressed = true;
      }
      else
      {
        _pressed = false;
      }
    }
  }
  _lastState = status;
}

//Based on current button status, caLL callback functions
void Button::update()
{
  checkStatus();

  if(_pressed && !_executed)
  {
    onPress();
  }
  else if(!_pressed && _executed)
  {
    onRelease();
  }
}

//Callback function that calls an appropriate function
void Button::onPress()
{
  if(_clickListenerSet)
  {
    Serial.println("Button pressed");
    _callback(_direction);
  }
  else
  {
    Serial.print("Button on pin ");
    Serial.print(_pin);
    Serial.println(" has no click listener");
  }
  _executed = true;
}

//Callback function called on button release
void Button::onRelease()
{
  //Serial.println("Button release");
  _executed = false;
}

//Passes a pointer to a function that will be called on button click
//Also sets a message to be printed on button click
void Button::setOnClickListener(
  void (*clickFunction)(int),
  int direction)
{
  _callback         = clickFunction;
  _clickListenerSet = true;
  _direction         = direction;
}
