#ifndef TMC2209_HPP
#define TMC2209_HPP

#define MAX_SPEED 40 // In timer value
#define MIN_SPEED 1000

#include <Arduino.h>

#define STALL_VALUE 100     // [0..255]
#define DRIVER_ADDRESS 0b00 // TMC2209 Driver address according to MS1 and MS2
#define MY_TMC_USART_PORT Serial1

#define R_SENSE 0.11f // Match to your driver
                      // SilentStepStick series use 0.11
                      // UltiMachine Einsy and Archim2 boards use 0.2
                      // Panucatt BSD2660 uses 0.1
                      // Watterott TMC5160 uses 0.075

void TMC2209_Init();
void TMC2209_Spin_Steps(int steps);
void TMC2209_Homing();
void TMC2209_Diag_Handler();
#endif // TMC2209_HPP