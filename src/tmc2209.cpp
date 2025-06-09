#include <TMCStepper.h>
#include "tmc2209.hpp"
#include "main.hpp"

TMC2209Stepper driver(&MY_TMC_USART_PORT, R_SENSE, DRIVER_ADDRESS1);
TMC2209Stepper driver2(&MY_TMC_USART_PORT, R_SENSE, DRIVER_ADDRESS2);
// AccelStepper stepper = AccelStepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

bool isStallDetected = false;
bool isStallDetected2 = false;

void TMC2209_Init()
{
    Serial1.begin(115200);
    driver.beginSerial(115200);

    digitalWrite(EN_PIN, LOW);
    driver.begin();
    driver.toff(4);
    driver.blank_time(24);
    driver.rms_current(1000); // mA
    driver.microsteps(4);
    driver.TCOOLTHRS(0xFFFFF); // 20bit max
    driver.semin(5);

    driver.semax(2);
    driver.sedn(0b01);
    driver.SGTHRS(STALL_VALUE);
    digitalWrite(EN_PIN, HIGH);

    driver2.beginSerial(115200);

    digitalWrite(EN2_PIN, LOW);
    driver2.begin();
    driver2.toff(4);
    driver2.blank_time(24);
    driver2.rms_current(1000); // mA
    driver2.microsteps(4);
    driver2.TCOOLTHRS(0xFFFFF); // 20bit max
    driver2.semin(5);

    driver2.semax(2);
    driver2.sedn(0b01);
    driver2.SGTHRS(STALL_VALUE2);
    digitalWrite(EN2_PIN, HIGH);
}

void TMC2209_Spin_Steps(int steps)
{
    // enable driver to spin
    driver.microsteps(4);
    digitalWrite(EN_PIN, LOW);

    for (int i = 0; i < steps; i++)
    {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(1000);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(1000);
        // Serial2.printf("SG_RESULT: %d\n", driver.SG_RESULT());
    }

    // disable driver to spin
    digitalWrite(EN_PIN, HIGH);
}

void TMC2209_Spin_Steps2(int steps)
{
    // enable driver to spin
    Serial2.println(F("Attempting to spin 2"));
    driver2.microsteps(4);
    digitalWrite(EN2_PIN, LOW);
    digitalWrite(DIR2_PIN, HIGH);

    for (int i = 0; i < steps; i++)
    {
        digitalWrite(STEP2_PIN, HIGH);
        delayMicroseconds(1000);
        digitalWrite(STEP2_PIN, LOW);
        delayMicroseconds(1000);
        // Serial2.printf("SG_RESULT: %d\n", driver.SG_RESULT());
    }

    // disable driver to spin
    digitalWrite(EN2_PIN, HIGH);
    Serial2.println("Done Spinning TMC_2");
}

void TMC2209_Homing()
{
    Serial2.println(F("Homing action..."));
    driver.microsteps(8);

    // enable driver to spin
    digitalWrite(EN_PIN, LOW);

    digitalWrite(DIR_PIN, HIGH);

    while (isStallDetected != true)
    {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(1000);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(1000);
        // Serial2.printf("SG_RESULT: %d\n", driver.SG_RESULT());
    }

    // disable driver to spin
    digitalWrite(EN_PIN, HIGH);
    isStallDetected = !isStallDetected;
    Serial2.println(F("Homing action done!!"));
}

void TMC2209_Homing2()
{
    Serial2.println(F("Homing action... 2"));
    driver2.microsteps(8);

    // enable driver to spin
    digitalWrite(EN2_PIN, LOW);
    digitalWrite(DIR2_PIN, LOW);

    while (isStallDetected2 != true)
    {
        digitalWrite(STEP2_PIN, HIGH);
        delayMicroseconds(1000);
        digitalWrite(STEP2_PIN, LOW);
        delayMicroseconds(1000);
        // Serial2.printf("SG_RESULT: %d\n", driver.SG_RESULT());
    }

    // disable driver to spin
    digitalWrite(EN2_PIN, HIGH);
    isStallDetected2 = !isStallDetected2;
    Serial2.println(F("Homing action done!! 2"));
}
void TMC2209_Diag_Handler()
{
    isStallDetected = !isStallDetected;
    Serial2.println(F("Stall detected!!"));
    // Serial2.printf("SG_RESULT: %d\n", driver.SG_RESULT());
    digitalWrite(DIR_PIN, LOW);
    TMC2209_Spin_Steps(1);
}

void TMC2209_Diag_Handler2()
{
    isStallDetected2 = !isStallDetected2;
    Serial2.println(F("Stall detected 2!!"));
    // Serial2.printf("SG_RESULT: %d\n", driver.SG_RESULT());
    digitalWrite(DIR2_PIN, HIGH);
    TMC2209_Spin_Steps2(1);
}