#ifndef Food_h
#define Food_h

#include "Arduino.h"
#include "LedControl.h"
#include "Data_Structs.h"

class Food
{
  public:
    Food(LedControl* lc, int rows, int columns);
    void generateNewLocation();
    Position getPosition();
    void draw();
    void clearImage();
    void setDrawn(bool status);
    void reset();
  private:
    Position _currentPos;
    int _rows;
    int _columns;
    bool _drawn;
    LedControl* _lc;
};

#endif
