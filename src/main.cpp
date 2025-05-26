#include "main.hpp"
#include "tmc2209.hpp"

void setup()
{
  // using usart2 for debugging
  Serial2.begin(115200);
  GPIO_Init();
  TMC2209_Init();

  // print indicator for finishing all initialization
  Serial2.println(F("Done GPIO and TMC2209 setup!"));

  // test simple homing
  TMC2209_Homing();
}

void loop()
{
  // do nothing
}

void GPIO_Init()
{
  pinMode(EN_PIN, OUTPUT);
  pinMode(EN_PIN, HIGH);

  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(DIAG_PIN, INPUT);
}