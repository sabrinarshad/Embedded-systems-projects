/*
 * rca_buzzer.c: Set Buzzer Functions
 */

#include "rca_buzzer.h"
#include "pwm_buzzer.h"

/*
 * Initialize the Buzzer for simulate alarm
 */
void RCAInitBuzzer()
{
    PwmBuzzerInit();
}

/*
 * Set the Buzzer for a given pitch and volume level.
 *
 * The function calculates the PWM parameters for the buzzer,
 * and then calls PwmBuzzerSetFreq() and PwmBuzzerSetVol() to set those parameters.
 */
void RCASetBuzzer(float frequency, float volume)
{
    PwmBuzzerSet(frequency, volume);
    BuzzerOn();
}

/*
 * Turn off the Buzzer
 */
void RCATurnOffBuzzer()
{
    BuzzerOff();
}
