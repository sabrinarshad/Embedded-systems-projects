/*
 * rca_main.c
 *
 * Reverse Car Alarm Project
 *
 * Created by Carlos C. and Sabrina A.
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "launchpad.h"
#include "seg7.h"
#include "ranger.h"
#include "rca_buzzer.h"

/*
 * Global variables
 */

// System state
typedef struct {
    bool activated;
} SysState;

// The events
Event trigger_ranger_reading_event;
Event ranger_data_event;
Event check_push_button_event;

// The system state
static SysState sys =
{
    false /* not activated */,
};

// Initial 7-segment display with everything off
Seg7Display seg7 = { { 13, 13, 13, 13 }, false /* colon off */ };

// Trigger the ranger reading every 0.5 seconds
void TriggerRangerReading(Event *event)
{
    RangerTriggerReading();

    EventSchedule(event, event->time + 500);
}

// Updates seg7 display
void DisplayUpdate(float distance_m)
{
    if (distance_m > 0.152)
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
        seg7.digit[1] = (distance_mm > 9) ? ((distance_mm / 10) % 10) : 14;
        seg7.digit[2] = (distance_mm > 99) ? ((distance_mm / 100) % 10) : 14;
        seg7.digit[3] = (distance_mm > 999) ? ((distance_mm / 1000) % 10) : 14;
    } else
    {
        // Display "STOP" if less than a half foot (0.152 m)
        seg7.digit[0] = 12;
        seg7.digit[1] = 0;
        seg7.digit[2] = 11;
        seg7.digit[3] = 10;
    }

    // Update seg7
    Seg7Update(&seg7);
}

// Play the buzzer
void PlaySound(float distance) {
    if (sys.activated) {
        if (distance < 0.152) { // Below half foot
            RCASetBuzzer(800, 0.03);  // High pitch for close proximity and loud volume
        } else if (distance < 0.762) { // Between 0.5 and 2.5 feet
            RCASetBuzzer((300 + ((1 / distance) * 100)), 0.02); // Gradual pitch increase and moderate volume
        } else { // Beyond 2.5 feet
            RCATurnOffBuzzer();
        }
    }
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

        // Play Buzzer with updated distance
        PlaySound(distance_m);
    }
}

// Function to handle pushbuttons (SW1 = Millimeter, SW2 = Inch)
void CheckPushButton(Event *event)
{
    switch (PushButtonRead()) {
    case 1: // SW1 pressed, activate the buzzer
        sys.activated = true;
        LedTurnOnOff(true /* red */, false /* blue */, false /* green */); // Red light to signal buzzer is on
        break;
    case 2: // SW2 pressed, deactivate the buzzer and turn off LED
        sys.activated = false;
        RCATurnOffBuzzer();
        LedTurnOnOff(false /* red */, false /* blue */, false /* green */); // Turn off lights to signal buzzer is off
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
    RCAInitBuzzer();

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
