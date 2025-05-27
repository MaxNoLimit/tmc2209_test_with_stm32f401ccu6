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
  delay(100);
  // TMC2209_Spin_Steps(8000);
  // Serial2.println(F("Done spin test!"));

  delay(100);
  attachInterrupt(DIAG_PIN, TMC2209_Diag_Handler, RISING);

  // test simple homing
  TMC2209_Homing();
  Serial2.println(F("Done!!"));
}

void loop()
{
  // do nothing
}

void GPIO_Init()
{
  pinMode(EN_PIN, OUTPUT);
  // pinMode(EN_PIN, HIGH);

  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(DIAG_PIN, INPUT);

  pinMode(PC13, OUTPUT);
}