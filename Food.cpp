#include "Food.h"

Food::Food(LedControl* lc, int rows, int columns)
{
  _lc               = lc;
  _rows             = rows;
  _columns          = columns;
  _drawn            = false;
}

void Food::generateNewLocation()
{
  Serial.println("Generating new food location");
  clearImage();

  //randomSeed(analogRead(A0));

  int newX = random(0, _columns);
  int newY = random(0, _rows);

  _currentPos.x = newX;
  _currentPos.y = newY;
}

Position Food::getPosition()
{
  return _currentPos;
}

void Food::draw()
{
  if(!_drawn)
  {
    Serial.println("Draw food");
    _lc->setLed(0,_currentPos.y,_currentPos.x,true);
    _drawn = true;
  }
}

void Food::clearImage()
{
  if(_drawn)
  {
    _lc->setLed(0,_currentPos.y,_currentPos.x,false);
    _drawn = false;
  }
}

void Food::setDrawn(bool status)
{
  _drawn = status;
}

void Food::reset()
{
  _drawn            = false;
  generateNewLocation();
}
