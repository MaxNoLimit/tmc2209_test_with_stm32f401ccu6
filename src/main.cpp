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
  attachInterrupt(DIAG_PIN, TMC2209_Diag_Handler, RISING);
  attachInterrupt(DIAG2_PIN, TMC2209_Diag_Handler2, RISING);

  // TMC2209_Spin_Steps(1000);
  // TMC2209_Homing();

  // test simple homing
  // TMC2209_Spin_Steps2(1000);
  // TMC2209_Homing2();

  Serial2.println(F("Done!!"));
}

void loop()
{
  String t_msg = Serial2.readString();
  if (t_msg == "homing1")
  {
    TMC2209_Homing();
  }
  else if (t_msg == "homing2")
  {
    TMC2209_Homing2();
  }
  else if (t_msg == "move1")
  {
    TMC2209_Spin_Steps(1000, LOW);
  }
  else if (t_msg == "move2")
  {
    TMC2209_Spin_Steps2(1000, HIGH);
  }
  else if (t_msg == "VSlotCalibration")
  {
    TMC2209_Homing();
    Serial2.println(F("Wait..."));
    delay(1000);
    TMC2209_MeassureDistance();
    TMC2209_Spin_Steps((getConstant1() * 200.0 / 2.0), HIGH);

    TMC2209_Homing2();
    Serial2.println(F("Wait..."));
    delay(1000);
    TMC2209_MeassureDistance2();
    TMC2209_Spin_Steps2((getConstant2() * 200.0 / 2.0), LOW);
  }
  else if (t_msg == "Measure1")
  {
    TMC2209_MeassureDistance();
    TMC2209_Spin_Steps((getConstant1() * 200.0 / 2.0), HIGH);
  }
  else if (t_msg == "Measure2")
  {
    TMC2209_MeassureDistance2();
    TMC2209_Spin_Steps2((getConstant2() * 200.0 / 2.0), LOW);
  }
}

void GPIO_Init()
{
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, HIGH);
  pinMode(EN2_PIN, OUTPUT);
  digitalWrite(EN2_PIN, HIGH);

  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(DIAG_PIN, INPUT);

  pinMode(STEP2_PIN, OUTPUT);
  pinMode(DIR2_PIN, OUTPUT);
  pinMode(DIAG2_PIN, INPUT);
}