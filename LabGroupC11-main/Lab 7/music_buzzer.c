/*
 *
 * music_buzzer.c: Set Buzzer for a music note
 *
 */

#include "music.h"
#include "pwm_buzzer.h"

/*
 * Frequencies of each pitch level
 */
const float noteFreq[NUM_PITCH] =
{
     261.63,    // C4
     293.66,    // D4
     329.63,    // E4
     349.23,    // F4
     392.00,    // G4
     440.00,    // A4
     493.88     // B4
};

/*
 * Volume levels
 */
const float volLevel[NUM_VOLUME_LEVEL] =
{
     0.001,     // Low
     0.002,     // Medium
     0.003      // High
};

/*
 * Initialize the Buzzer for music note playing
 */
void MusicInitBuzzer()
{
    PwmBuzzerInit();
}

/*
 * Set the Buzzer for a given pitch and volume level.
 *
 * The function calculates the PWM parameters for the buzzer,
 * and then calls PwmBuzzerSetFreq() and PwmBuzzerSetVol() to set those parameters.
 */
void MusicSetBuzzer(int pitch, int volume)
{
    float frequency = noteFreq[pitch];
    float volumeLevel = volLevel[volume];
    PwmBuzzerSet(frequency, volumeLevel);
    BuzzerOn();
}

/*
 * Turn off the Buzzer
 */
void MusicTurnOffBuzzer()
{
    BuzzerOff();
}
