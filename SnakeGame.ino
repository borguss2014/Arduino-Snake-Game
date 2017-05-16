#include "LedControl.h"
#include "Food.h"
#include "Snake.h"
#include "Data_Structs.h"
#include "Button.h"

void draw();
void updateGame();
void setSnakeDirection(int direction);
void middleButtonAction(int direction);
void middleButtonStopAction(int direction);

/*
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn
 pin 11 is connected to the CLK
 pin 10 is connected to LOAD
 We have only a single MAX72XX.
 */

LedControl ledControl=LedControl(12,11,10,1);

int columns = 8;
int rows = 8;

Food food(&ledControl, rows, columns);
Snake snake(&ledControl, rows, columns);

int currentGameState;
int updateSpeed = 600;

unsigned long initialTime;
unsigned long currentTime;

Button button_up(7);
Button button_down(3);
Button button_left(5);
Button button_right(4);
Button button_middle(6);

bool state;

void setup()
{
  Serial.begin(9600);

  ledControl.shutdown(0,false);
  /* Set the brightness to a medium values */
  ledControl.setIntensity(0,8);
  /* and clear the display */
  ledControl.clearDisplay(0);

  button_up.setOnClickListener(setSnakeDirection, Direction::UP);
  button_down.setOnClickListener(setSnakeDirection, Direction::DOWN);
  button_left.setOnClickListener(setSnakeDirection, Direction::LEFT);
  button_right.setOnClickListener(setSnakeDirection, Direction::RIGHT);
  button_middle.setOnClickListener(middleButtonPlayingAction, Direction::MIDDLE);

  randomSeed(analogRead(A0));

  initialTime = millis();

  currentGameState = GameState::PLAYING;

  Serial.print("Initial snake position: ");
  Serial.print(snake.getHeadPos().x);
  Serial.print(" ");
  Serial.print(snake.getHeadPos().y);
  Serial.println();
  food.generateNewLocation();
  state = true;
}

void loop()
{
  while(currentGameState == GameState::PLAYING)
  {
   //Serial.println("LOOPING");
   button_middle.update();
   if(state)
   {
     updateGame();
     draw();
   }
  }

  byte clickToPlayOn[8]={
    B00011000,
    B00011000,
    B00000000,
    B11011011,
    B11011011,
    B00000000,
    B00011000,
    B00011000
  };

  byte clickToPlayOff[8]={
    B00011000,
    B00011000,
    B00000000,
    B11000011,
    B11000011,
    B00000000,
    B00011000,
    B00011000
  };

  bool blink = true;

  while(currentGameState == GameState::STOP)
  {
    button_middle.update();

    currentTime = millis();

    if(currentTime - initialTime >= updateSpeed && currentGameState == GameState::STOP)
    {
      if(blink)
      {
        for(int i=0; i<8; i++)
        {
          ledControl.setRow(0,i,clickToPlayOn[i]);
        }
        blink = false;
      }
      else
      {
        for(int i=0; i<8; i++)
        {
          ledControl.setRow(0,i,clickToPlayOff[i]);
        }
        blink = true;
      }

      initialTime = currentTime;
    }
  }
}

void updateGame()
{
  button_up.update();
  button_down.update();
  button_left.update();
  button_right.update();
  //button_middle.update();

  currentTime = millis();
  bool onFood = snake.checkCollision(&food);
  if(onFood)
  {
    Serial.println("Eating food...");
    snake.eat(&food);
    snake.updateBody();
  }

  bool selfCollision = snake.checkSelfCollision();
  if(selfCollision)
  {
    Serial.println("Self collision... Game over");
    currentGameState = GameState::STOP;

    for(int i=0; i<3; i++)
    {
      snake.clearImage();
      delay(300);
      snake.draw();
      delay(300);
    }

    delay(1500);

    snake.clearImage();
    food.clearImage();

    snake.setDrawn(true);
    food.setDrawn(true);

    button_middle.setOnClickListener(middleButtonStopAction, Direction::MIDDLE);
  }

  if(currentTime - initialTime >= updateSpeed)
  {
    Serial.println("Moving snake");
    snake.moveNewLocation();
    initialTime = currentTime;
  }
}

void draw()
{
  food.draw();
  snake.draw();
}

void setSnakeDirection(int direction)
{
  snake.setDirection(direction);
}

void middleButtonPlayingAction(int direction)
{
  state = !state;
}

void middleButtonStopAction(int direction)
{
  //Re-initialize game
  byte newGame[8]={
    B00011000,
    B00011000,
    B00011000,
    B11111111,
    B11111111,
    B00011000,
    B00011000,
    B00011000
  };

  byte clearScreen[8]={
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000
  };

  for(int i=0; i<8; i++)
  {
    ledControl.setRow(0,i,newGame[i]);
  }
  delay(2000);


  snake.reset();
  food.reset();

  for(int i=0; i<8; i++)
  {
    ledControl.setRow(0,i,clearScreen[i]);
  }

  currentGameState = GameState::PLAYING;
  button_middle.setOnClickListener(middleButtonPlayingAction, Direction::MIDDLE);
}
