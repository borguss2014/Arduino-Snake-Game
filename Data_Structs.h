#ifndef Data_Structs_h
#define Data_Structs_h

#include "Arduino.h"

enum GameState
{
  PLAYING,
  STOP
};

enum Direction
{
  UP,
  DOWN,
  LEFT,
  RIGHT,
  MIDDLE
};

struct Position
{
  unsigned int x = 0;
  unsigned int y = 0;
};

struct Segment
{
  unsigned int x = 0;
  unsigned int y = 0;
};

#endif
