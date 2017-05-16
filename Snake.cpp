#include "Snake.h"

Snake::Snake(LedControl* lc, int rows, int columns)
{
  _rows             = rows;
  _columns          = columns;

  _lc               = lc;

  Segment head;
  _length           = 1;
  _direction        = Direction::DOWN;

  _drawn            = false;

  _body             = new Segment[_length];
  _body[0]          = head;
}

bool Snake::checkCollision(Food* food)
{
  Position foodPos = food->getPosition();

  if(_body[_length-1].x == foodPos.x && _body[_length-1].y == foodPos.y)
  {
    return true;
  }

  return false;
}

bool Snake::checkSelfCollision()
{
  for(int i=0; i<_length-1; i++)
  {
    if(_body[_length-1].x == _body[i].x && _body[_length-1].y == _body[i].y)
    {
      return true;
    }
  }

  return false;
}

void Snake::eat(Food* food)
{
    food->generateNewLocation();
}

void Snake::setDirection(int dir)
{
  _direction = dir;
}

int Snake::getDirection()
{
  return _direction;
}

int Snake::getLength()
{
  return _length;
}

void Snake::updateBody()
{
  // Serial.print("Old body: ");
  // Serial.println();
  // Serial.println("======");
  // for(int i=0; i<_length; i++)
  // {
  //   Serial.print(_body[i].x);
  //   Serial.print(" | ");
  //   Serial.print(_body[i].y);
  //   Serial.println();
  // }
  // Serial.println("======");

  // Serial.print("Copying old body in new body:");
  // Serial.println();
  // Serial.println("======");
  Segment* newBody = new Segment[_length + 1];
  for(int i=0; i<_length; i++)
  {
    // Serial.print("Old segment: ");
    // Serial.print(_body[i].x);
    // Serial.print(" | ");
    // Serial.print(_body[i].y);

    newBody[i] = _body[i];

    // Serial.print("New segment: ");
    // Serial.print(newBody[i].x);
    // Serial.print(" | ");
    // Serial.print(newBody[i].y);
  }

  // Serial.print("New body after copy: ");
  // Serial.println();
  // Serial.println("======");
  // for(int i=0; i<_length+1; i++)
  // {
  //   Serial.print(newBody[i].x);
  //   Serial.print(" | ");
  //   Serial.print(newBody[i].y);
  //   Serial.println();
  // }
  // Serial.println("======");

  Segment newHead = addHead();
  // Serial.print("Old head: ");
  // Serial.print(_body[_length-1].x);
  // Serial.print(" | ");
  // Serial.print(_body[_length-1].y);
  // Serial.println();
  //
  // Serial.print("New head: ");
  // Serial.print(newHead.x);
  // Serial.print(" | ");
  // Serial.print(newHead.y);
  // Serial.println();

  clearImage();
  delete[] _body;

  _length++;

  _body = newBody;
  _body[_length-1] = newHead;

  // Serial.print("Updated body: ");
  // Serial.println();
  // Serial.println("======");
  // for(int i=0; i<_length; i++)
  // {
  //   Serial.print(_body[i].x);
  //   Serial.print(" | ");
  //   Serial.print(_body[i].y);
  //   Serial.println();
  // }
  // Serial.println("======");
}

Segment Snake::addHead()
{
  int dir = getDirection();
  Segment newBodySeg;

  switch(dir)
  {
    case Direction::UP:
      newBodySeg.x = _body[_length-1].x;
      newBodySeg.y = (_body[_length-1].y - 1) % _rows;
      break;
    case Direction::DOWN:
      newBodySeg.x = _body[_length-1].x;
      newBodySeg.y = (_body[_length-1].y + 1) % _rows;
      break;
    case Direction::LEFT:
      newBodySeg.x = (_body[_length-1].x - 1) % _columns;
      newBodySeg.y = _body[_length-1].y;
      break;
    case Direction::RIGHT:
      newBodySeg.x = (_body[_length-1].x + 1) % _columns;
      newBodySeg.y = _body[_length-1].y;
      break;
  }

  Serial.print("new X: "); Serial.print(newBodySeg.x); Serial.print(" | "); Serial.print("new Y: "); Serial.print(newBodySeg.y); Serial.println();
  return newBodySeg;
}

void Snake::moveNewLocation()
{
  clearImage();
  Segment segHead = addHead();

  for(int i=0; i<_length; i++)
  {
    _body[i] = _body[i+1];
  }
  _body[_length - 1] = segHead;
}

void Snake::draw()
{
  if(!_drawn)
  {
    for(int i=0; i<_length; i++)
    {
      _lc->setLed(0,_body[i].y,_body[i].x,true);
      //Serial.print("Segment "); Serial.print(i); Serial.print(" : "); Serial.print(_body[i].x); Serial.print(" | "); Serial.print(_body[i].y);
    }
    _drawn = true;
  }
}

void Snake::clearImage()
{
  if(_drawn)
  {
    for(int i=0; i<_length; i++)
    {
      _lc->setLed(0,_body[i].y,_body[i].x,false);
    }
    _drawn = false;
  }
}

Segment Snake::getHeadPos()
{
  return _body[_length-1];
}

void Snake::setDrawn(bool status)
{
  _drawn = status;
}

void Snake::reset()
{
  Segment head;
  _length           = 1;
  _direction        = Direction::DOWN;
  _drawn            = false;
  _body             = new Segment[_length];
  _body[0]          = head;
}
