/*
 * lab8_main.c: Starter code for ECE 266 Lab 8, main.c, fall 2024
 * Note: In this semester, a small part of starter code is provided
 * (none was provided before).
 *
 * Lab 8: Distance Measurement
 *
 * Created by Zhao Zhang
 * Last update: fall 2024
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "launchpad.h"
#include "seg7.h"
#include "ranger.h"

/*
 * Global variables
 */

// System state
typedef struct {
    enum {Millimeter, Inch} display_mode;       // display inch or millimeter
} SysState;

// The events
Event trigger_ranger_reading_event;
Event ranger_data_event;
Event check_push_button_event;

SysState sys = {Millimeter};

// Initial 7-segment display with everything off
Seg7Display seg7 = { { 10, 10, 10, 10 }, false /* colon off */ };

/*******************************************
 * Task 1: Trigger and read the ranger
 ******************************************/

// Trigger the ranger reading every 0.5 seconds
void TriggerRangerReading(Event *event)
{
    RangerTriggerReading();

    EventSchedule(event, event->time + 500);
}

// Updates seg7 display
void DisplayUpdate(float distance_m)
{
    if (sys.display_mode == Millimeter)
    {
        // Convert meters to millimeters
        int distance_mm = distance_m * 1000;

        // Cap at 9999mm
        if (distance_mm > 9999)
        {
            distance_mm = 9999;
        }

        // Set digits for millimeters
        seg7.digit[0] = distance_mm % 10;
        seg7.digit[1] = (distance_mm > 9) ? ((distance_mm / 10) % 10) : 10;
        seg7.digit[2] = (distance_mm > 99) ? ((distance_mm / 100) % 10) : 10;
        seg7.digit[3] = (distance_mm > 999) ? ((distance_mm / 1000) % 10) : 10;
        seg7.colon_on = false;
    } else if (sys.display_mode == Inch)
    {
        // Convert meters to feet in inches
        int inches = distance_m / 0.0254;
        int overInches = inches % 12;
        int feet = (inches / 12);

        // Set digits for feet and inches
        // Use 11 instead of 10 for empty digits as 10 is being used for millimeters
        seg7.digit[0] = overInches % 10;
        seg7.digit[1] = (overInches > 9) ? ((overInches / 10) % 10) : 11;
        seg7.digit[2] = feet % 10;
        seg7.digit[3] = (feet > 9) ? ((feet / 10) % 10) : 11;
        seg7.colon_on = true;
    }

    // Update seg7
    Seg7Update(&seg7);
}

// Process the ranger reading
void ProcessPulseWidthReading(Event *event)
{
    if (RangerDataReady()) {  // Only process if new data is ready
        uint32_t pulse_width = RangerGetData();
        uprintf("Pulse width: %u clock cycles\n\r", pulse_width);

        // Calculate distance in meters
        float distance_m = ((float)pulse_width / (2.0 * 50000000.0)) * 340.0;
        uprintf("Distance: %.3f meters\n\r", distance_m);

        // Set to 0 if noise
        if (distance_m > 5) {
            distance_m = 0;
        }

        // Update the 7-segment display
        DisplayUpdate(distance_m);
    }

}

// Function to handle pushbuttons (SW1 = Millimeter, SW2 = Inch)
void CheckPushButton(Event *event)
{
    switch (PushButtonRead()) {
    case 1: // SW1 pressed, switch to Millimeter Mode
        sys.display_mode = Millimeter;
        break;
    case 2: // SW2 pressed, switch to Inch Mode
        sys.display_mode = Inch;
        break;
    }
}


/*******************************************
 * The main function
 ******************************************/
void main(void)
{
    // Initialize the LaunchPad and peripherals
    LaunchPadInit();
    RangerInit();
    Seg7Init();

    // Initialize the events
    EventInit(&trigger_ranger_reading_event, TriggerRangerReading);

    // Schedule time event
    EventSchedule(&trigger_ranger_reading_event, 100);

    // Initialize and register ISR events
    EventInit(&ranger_data_event, ProcessPulseWidthReading);
    EventInit(&check_push_button_event, CheckPushButton);
    RangerEventRegister(&ranger_data_event);
    PushButtonEventRegister(&check_push_button_event);

    // Loop forever
    while (true)
    {
        // Wait for interrupt
        asm("   wfi");

        // Execute scheduled callbacks
        EventExecute();
    }
}
