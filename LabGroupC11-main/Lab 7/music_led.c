/*
 * Starter code for ECE 266 Lab 7, Sound Player (PWM), fall 2024
 *
 * music_led.c: Set LED for a music note
 *
 * Created by Zhao Zhang
 */

#include "music.h"
#include "pwm_led.h"

/*
 * Color and brightness settings of the three sub-LEDs for a given
 * pitch level and volume level.
 */

// LED color settings for a given pitch
typedef struct
{
    float red;          // weight for red color
    float blue;         // weight for blue color
    float green;        // weight for green color
} LEDColorSetting;

LEDColorSetting led_color[NUM_PITCH] =
{
    // weights for red, blue, and green colors
    {0.33, 0.33, 0.33},     // C4
    {1.0, 0.0, 0.0},        // D4
    {0.5, 0.5, 0.0},        // E4
    {0.0, 1.0, 0.0},        // F4
    {0.0, 0.5, 0.5},        // G4
    {0.0, 0.0, 1.0},        // A4
    {0.5, 0.0, 0.5},        // B4
};

// LED brightness settings defined as PWM period and PWM pulse width
typedef struct
{
    int period;
    int pulse_width;
} LEDBrightnessSetting;

LEDBrightnessSetting led_brightness[NUM_VOLUME_LEVEL] =
{
   {5000, 10},              // level 0
   {5000, 20},              // level 1
   {5000, 100}              // level 2
};

/*
 * Initialize the LEDs for music note playing
 */
void MusicInitLed()
{
    PwmLedInit();
}

/*
 * Set the LED for a given pitch and volume level.
 *
 * The function calculates the PWM parameters for the three sub-LEDs,
 * and then call LedPwmSet() to set those parameters.
 */
void MusicSetLed(int pitch, int volume)
{
    int pulse_width_red = led_brightness[volume].pulse_width * led_color[pitch].red;
    int pulse_width_blue = led_brightness[volume].pulse_width * led_color[pitch].blue;
    int pulse_width_green = led_brightness[volume].pulse_width * led_color[pitch].green;

    PwmLedSet(led_brightness[volume].period, pulse_width_red, pulse_width_blue,
              pulse_width_green);
}

/*
 * Turn off the LED
 */
void MusicTurnOffLed()
{
    PwmLedSet(1000, 0, 0, 0);
}
