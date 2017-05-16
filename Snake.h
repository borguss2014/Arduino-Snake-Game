#ifndef Snake_h
#define Snake_h

#include "Arduino.h"
#include "Food.h"
#include "LedControl.h"
#include "Data_Structs.h"

class Snake
{
  public:
    Snake(LedControl* lc, int rows, int columns);

    bool checkCollision(Food* food);
    bool checkSelfCollision();

    void eat(Food* food);

    void setDirection(int dir);
    int getDirection();
    void draw();

    int getLength();
    void updateBody();

    void moveNewLocation();

    void clearImage();

    void setDrawn(bool status);

    void reset();

    Segment addHead();

    Segment getHeadPos();
  private:
    int _length;
    int _direction;
    int _columns;
    int _rows;
    bool _drawn;
    Segment* _body;
    LedControl* _lc;
};

#endif
