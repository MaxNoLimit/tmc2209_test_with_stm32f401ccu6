#ifndef TMC2209_HPP
#define TMC2209_HPP

#include <Arduino.h>

#define STALL_VALUE 60  // [0..255]
#define STALL_VALUE2 60 // [0..255]
#define HOMING_STALL_VALUE 30
#define DRIVER_ADDRESS1 0b00 // TMC2209 Driver address according to MS1 and MS2
#define DRIVER_ADDRESS2 0b01
#define MY_TMC_USART_PORT Serial1
#define FREQ_NORMALSPIN 800
#define RAIL_DISTANCE 620 // mm

#define R_SENSE 0.11f // Match to your driver
                      // SilentStepStick series use 0.11
                      // UltiMachine Einsy and Archim2 boards use 0.2
                      // Panucatt BSD2660 uses 0.1
                      // Watterott TMC5160 uses 0.075

void TMC2209_Init();
void TMC2209_Spin_Steps(int steps, bool dir);
void TMC2209_Spin_Steps2(int steps, bool dir);
void TMC2209_Homing();
void TMC2209_Homing2();
void TMC2209_Diag_Handler();
void TMC2209_Diag_Handler2();

void TMC2209_MeassureDistance();
void TMC2209_MeassureDistance2();

float getConstant1();
float getConstant2();

#endif // TMC2209_HPP