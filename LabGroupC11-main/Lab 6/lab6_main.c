/*
 * Starter code for ECE 266 Lab 6, main.c, fall 2024
 *
 * Lab 6: Temperature reading and knob input (ADC)
 *
 * Created by Zhao Zhang
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "launchpad.h"
#include "seg7.h"
#include "temp_sensor.h"
#include "ras.h"

// The events
Event ts_trigger_event;
Event ts_data_event;
Event ras_trigger_event;
Event ras_data_event;

// Initial 7-segment display with everything off
Seg7Display seg7 = { { 10, 10, 10, 10 }, false /* colon off */};

/*
 * Trigger the temperature reading periodically
 */
void TriggerTemperatureReading(Event *event)
{
    // Trigger ADC sampling for the temperature sensor
    TsTriggerReading();

    // Schedule the next callback in 4000 milliseconds
    EventSchedule(event, event->time + 4000);
}

/*
 * Process temperature reading data
 */
void ProcessTemperatureReading(Event *event)
{
    float temp_F = TsDataRead();
    int temp10 = temp_F * 10;

    // Print on terminal
    uprintf("Temperature Reading: %u.%u F\r\n", temp10 / 10, temp10 % 10);

    // Set each digit for temperature
    seg7.digit[0] = 10;
    seg7.digit[1] = temp10 % 10;
    seg7.digit[2] = (temp10 / 10) % 10;
    seg7.digit[3] = temp10 / 100;
    seg7.colon_on = true;

    // Update seg7
    Seg7Update(&seg7);
}

/*
 * Trigger the angle reading periodically
 */
void TriggerAngleReading(Event *event)
{
    // Trigger ADC sampling for the rotary angle sensor
    RASTriggerReading();

    // Schedule the next callback in 4000 milliseconds
    EventSchedule(event, event->time + 4000);
}

/*
 * Process angle reading data
 */
void ProcessAngleReading(Event *event)
{
    float angle = RASDataRead();
    int setAngle = (angle / 4095) * 180;

    // Print on terminal
    uprintf("RAS Reading: %u°\r\n", setAngle);

    // Set each digit for angle without leading zeros
    seg7.digit[0] = setAngle % 10;
    if (setAngle != 0) {
        seg7.digit[1] = (setAngle / 10) % 10;
    } else {
        seg7.digit[1] = 10;
    }
    if (setAngle > 99) {
        seg7.digit[2] = setAngle / 100;
    } else {
        seg7.digit[2] = 10;
    }
    seg7.digit[3] = 10;
    seg7.colon_on = false;

    // Update seg7
    Seg7Update(&seg7);
}

/*
 * The main function: Initialize the system and run event scheduler loop
 */
int main(void)
{
    // Initialize all hardware components being used
    LaunchPadInit();
    TsInit();
    RASInit();
    Seg7Init();

    // Initialize and schedule timing events
    EventInit(&ts_trigger_event, TriggerTemperatureReading);
    EventInit(&ras_trigger_event, TriggerAngleReading);
    EventSchedule(&ts_trigger_event, 100);
    EventSchedule(&ras_trigger_event, 1900);

    // Initialize and register ISR events
    EventInit(&ts_data_event, ProcessTemperatureReading);
    EventInit(&ras_data_event, ProcessAngleReading);
    TsEventRegister(&ts_data_event);
    RASEventRegister(&ras_data_event);

    uprintf("%s\n\r", "Lab 6: Temperature reading and knob input (ADC)");

    // Loop forever
    while (true)
    {
        // Wait for interrupt to happen
        __asm("    wfi");

        // Call the callback scheduler
        EventExecute();
    }
}
