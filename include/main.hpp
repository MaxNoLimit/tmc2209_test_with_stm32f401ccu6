#ifndef MAIN_HPP
#define MAIN_HPP

#include "Arduino.h"
#define EN_PIN PB3
#define STEP_PIN PB4
#define DIR_PIN PB5
#define DIAG_PIN PB6

#define EN2_PIN PA15
#define STEP2_PIN PA8
#define DIR2_PIN PA7
#define DIAG2_PIN PB7

void GPIO_Init();

#endif // MAIN_HPP