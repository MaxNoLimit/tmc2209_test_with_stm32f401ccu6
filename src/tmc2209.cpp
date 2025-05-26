#include <TMCStepper.h>
#include "tmc2209.hpp"
#include "main.hpp"

TMC2209Stepper driver(&MY_TMC_USART_PORT, R_SENSE, DRIVER_ADDRESS);
// AccelStepper stepper = AccelStepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

bool isStallDetected = false;

void TMC2209_Init()
{
    Serial1.begin(115200);
    driver.beginSerial(115200);

    digitalWrite(EN_PIN, LOW);
    driver.begin();
    driver.toff(4);
    driver.blank_time(24);
    driver.rms_current(1000); // mA
    driver.microsteps(2);
    driver.TCOOLTHRS(0xFFFFF); // 20bit max
    driver.semin(5);

    driver.semax(2);
    driver.sedn(0b01);
    driver.SGTHRS(STALL_VALUE);

    attachInterrupt(DIAG_PIN, TMC2209_Diag_Handler, RISING);
}

void TMC2209_Spin_Steps(int steps)
{
    // enable driver to spin
    digitalWrite(EN_PIN, LOW);

    for (int i = 0; i < steps; i++)
    {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(200);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(200);
    }

    // disable driver to spin
    digitalWrite(EN_PIN, HIGH);
}

void TMC2209_Homing()
{
    Serial2.println(F("Homing action..."));
    // enable driver to spin
    digitalWrite(EN_PIN, LOW);

    digitalWrite(DIR_PIN, HIGH);

    while (isStallDetected != true)
    {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(1000);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(1000);
    }

    // disable driver to spin
    digitalWrite(EN_PIN, HIGH);
    isStallDetected = !isStallDetected;
    Serial2.println(F("Homing action done!!"));
}

void TMC2209_Diag_Handler()
{
    isStallDetected = !isStallDetected;
    digitalWrite(DIR_PIN, LOW);
    TMC2209_Spin_Steps(1000);
}
