/*
 * Starter code for ECE 266 Lab 7, Sound Player (PWM), fall 2024
 *
 * The program sets LED for the intended music note, but does not yet play sound on the buzzer.
 *
 * main.c
 *
 * Created by Zhao Zhang
 * Last update: 10/13/2022
 */

// REVISE AS NEEDED
#include <pwm_led.h>
#include "launchpad.h"
#include "music.h"

/*
 * Global data structures and variables
 */

// Events in the system
Event check_push_button_event;
Event play_sound_event;

// System state
typedef struct
{
    int pitch;            // Current pitch level
    int volume;           // Current sound level
    bool buzzer_on;       // If buzzer is turned on or off
} SysState;

SysState sys = { 0, 0, false };


/*
 * Task 1: Check the push buttons.
 *
 * Use interrupt-based push button input.
 */
void CheckPushButton(Event *event)
{
    // Process the pushbutton input
    switch (PushButtonRead())
    {
    case 1:
        sys.pitch = (sys.pitch + 1) % NUM_PITCH;
        break;

    case 2:
        sys.volume = (sys.volume + 1) % NUM_VOLUME_LEVEL;
        break;
    }
}


/*
 * Task 2: Play the buzzer.
 *
 * REVISE THE CODE
 */
void PlaySound(Event *event)
{
    int delay;

    if (!sys.buzzer_on) {
        // Turn on the buzzer with the current PWM setting
        MusicSetLed(sys.pitch, sys.volume);
        MusicSetBuzzer(sys.pitch, sys.volume);
        sys.buzzer_on = true;
        delay = 500;
    }
    else
    {
        // Turn off the buzzer
        MusicTurnOffLed();
        MusicTurnOffBuzzer();
        sys.buzzer_on = false;
        delay = 1000;
    }

    // Increment the position and schedule the next callback
    EventSchedule(&play_sound_event, event->time + delay);
}


/*
 * The main function
 *
 * REVISE THE CODE
 */
void main(void)
{
    // Initialize the LannchPad, PWM function for LED, and PMW function for buzzer
    LaunchPadInit();
    MusicInitLed();
    MusicInitBuzzer();

    // Initialize the events
    EventInit(&play_sound_event, PlaySound);
    EventInit(&check_push_button_event, CheckPushButton);

    // Register ISR event
    PushButtonEventRegister(&check_push_button_event);

    // Schedule time events
    EventSchedule(&play_sound_event, 100);

    // Loop forever
    while (true)
    {
        // Wait for interrupt
        asm("   wfi");

        // Execute scheduled callbacks
        EventExecute();
    }
}
