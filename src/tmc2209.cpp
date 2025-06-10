#include <TMCStepper.h>
#include "tmc2209.hpp"
#include "main.hpp"

TMC2209Stepper driver(&MY_TMC_USART_PORT, R_SENSE, DRIVER_ADDRESS1);
TMC2209Stepper driver2(&MY_TMC_USART_PORT, R_SENSE, DRIVER_ADDRESS2);
// AccelStepper stepper = AccelStepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

bool isStallDetected = false;
bool isStallDetected2 = false;

float constant1 = 0.00;
float constant2 = 0.00;

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
    driver.semin(0);

    driver.semax(2);
    driver.sedn(0b01);
    driver.SGTHRS(100);
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
    driver2.SGTHRS(100);
    digitalWrite(EN2_PIN, HIGH);
}

// low dir for walk to scene, high dir leave to scene
void TMC2209_Spin_Steps(int steps, bool dir)
{
    // enable driver to spin
    Serial2.println(F("Attempting to spin 1"));
    driver.TCOOLTHRS(0xFFFFF);
    driver.SGTHRS(100);
    driver.microsteps(4);
    digitalWrite(EN_PIN, LOW);
    digitalWrite(DIR_PIN, dir);

    for (int i = 0; i < steps; i++)
    {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(FREQ_NORMALSPIN);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(FREQ_NORMALSPIN);
        // Serial2.printf("SG_RESULT: %d\n", driver.SG_RESULT());
    }

    // disable driver to spin
    digitalWrite(EN_PIN, HIGH);
    Serial2.println("Done Spinning TMC_1");
}

// high dir for walk to scene, low dir for leave the scene
void TMC2209_Spin_Steps2(int steps, bool dir)
{
    // enable driver to spin
    Serial2.println(F("Attempting to spin 2"));
    driver2.TCOOLTHRS(0xFFFFF);
    driver2.SGTHRS(100);
    driver2.microsteps(4);
    digitalWrite(EN2_PIN, LOW);
    digitalWrite(DIR2_PIN, dir);

    for (int i = 0; i < steps; i++)
    {
        digitalWrite(STEP2_PIN, HIGH);
        delayMicroseconds(FREQ_NORMALSPIN);
        digitalWrite(STEP2_PIN, LOW);
        delayMicroseconds(FREQ_NORMALSPIN);
        // Serial2.printf("SG_RESULT: %d\n", driver.SG_RESULT());
    }

    // disable driver to spin
    digitalWrite(EN2_PIN, HIGH);
    Serial2.println("Done Spinning TMC_2");
}

void TMC2209_Homing()
{
    isStallDetected = false;
    Serial2.println(F("Homing action..."));
    driver.TCOOLTHRS(0xAFFFF);
    driver.SGTHRS(HOMING_STALL_VALUE);
    driver.microsteps(8);

    // enable driver to spin
    digitalWrite(EN_PIN, LOW);

    digitalWrite(DIR_PIN, HIGH);

    while (isStallDetected != true)
    {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(500);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(500);
        // Serial2.printf("SG_RESULT: %d\n", driver.SG_RESULT());
    }

    // disable driver to spin
    digitalWrite(EN_PIN, HIGH);
    isStallDetected = !isStallDetected;
    Serial2.println(F("Homing action done!!"));
}

void TMC2209_MeassureDistance()
{
    isStallDetected = false;
    Serial2.println(F("Start meassuring!"));
    int nCount = 0;
    driver.TCOOLTHRS(0xAFFFF);
    driver.SGTHRS(40);
    driver.microsteps(8);

    // enable driver to spin
    digitalWrite(EN_PIN, LOW);
    digitalWrite(DIR_PIN, LOW);

    while (isStallDetected != true)
    {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(500);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(500);
        nCount++;
        // Serial2.printf("SG_RESULT: %d\n", driver.SG_RESULT());
    }

    // disable driver to spin
    digitalWrite(EN_PIN, HIGH);
    isStallDetected = !isStallDetected;
    Serial2.println(F("Homing action done!! 2"));
    constant1 = (float)nCount / (float)RAIL_DISTANCE;
    Serial2.println("Constant value 1: " + (String)constant1);
}

void TMC2209_Homing2()
{
    isStallDetected2 = false;
    Serial2.println(F("Homing action... 2"));
    driver2.TCOOLTHRS(0xFFFFF);
    driver2.SGTHRS(HOMING_STALL_VALUE + 20);
    driver2.microsteps(8);

    // enable driver to spin
    digitalWrite(EN2_PIN, LOW);
    digitalWrite(DIR2_PIN, LOW);

    while (isStallDetected2 != true)
    {
        digitalWrite(STEP2_PIN, HIGH);
        delayMicroseconds(500);
        digitalWrite(STEP2_PIN, LOW);
        delayMicroseconds(500);
    }

    // disable driver to spin
    digitalWrite(EN2_PIN, HIGH);
    isStallDetected2 = !isStallDetected2;
    Serial2.println(F("Homing action done!! 2"));
    driver2.SGTHRS(0);
}

void TMC2209_MeassureDistance2()
{
    isStallDetected2 = false;
    Serial2.println(F("Start meassuring!"));
    int nCount = 0;
    driver2.TCOOLTHRS(0x0FFFF);
    driver2.SGTHRS(70);
    driver2.microsteps(8);

    // enable driver to spin
    digitalWrite(EN2_PIN, LOW);
    digitalWrite(DIR2_PIN, HIGH);

    while (isStallDetected2 != true)
    {
        digitalWrite(STEP2_PIN, HIGH);
        delayMicroseconds(500);
        digitalWrite(STEP2_PIN, LOW);
        delayMicroseconds(500);
        nCount++;
        // Serial2.printf("SG_RESULT: %d\n", driver.SG_RESULT());
    }

    // disable driver to spin
    digitalWrite(EN2_PIN, HIGH);
    isStallDetected2 = !isStallDetected2;
    Serial2.println(F("Homing action done!! 2"));
    constant2 = (float)nCount / (float)RAIL_DISTANCE;
    Serial2.println("Constant value 2: " + (String)constant2);
}

void TMC2209_Diag_Handler()
{
    isStallDetected = !isStallDetected;
    Serial2.println(F("Stall detected!!"));
    // Serial2.printf("SG_RESULT: %d\n", driver.SG_RESULT());
    // digitalWrite(DIR_PIN, LOW);
    TMC2209_Spin_Steps(1, LOW);
}

void TMC2209_Diag_Handler2()
{
    isStallDetected2 = !isStallDetected2;
    Serial2.println(F("Stall detected 2!!"));
    // Serial2.printf("SG_RESULT: %d\n", driver.SG_RESULT());
    // digitalWrite(DIR2_PIN, HIGH);
    TMC2209_Spin_Steps2(1, HIGH);
}

float getConstant1()
{
    return constant1;
}

float getConstant2()
{
    return constant2;
}