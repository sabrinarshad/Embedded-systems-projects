/*
 * Starter code for ECE 266 Lab 7, Sound Player (PWM), fall 2024
 *
 * pwmled.c: PWM LED functions
 *
 * Created by Zhao Zhang
 */

#ifndef MUSIC_H_
#define MUSIC_H_

#include "launchpad.h"

// Define the number of supported music pitches and volume levels
#define NUM_PITCH           7
#define NUM_VOLUME_LEVEL    3

// Declare an enum type for music pitches (for future use)
typedef enum {
    C4, D4, E4, F4, G4, A4, B4
} MusicNote;

// Initialize the LEDs for music note playing
void MusicInitLed();

// Set the LED for a given pitch and volume level
void MusicSetLed(int pitch, int volume);

// Turn off LED
void MusicTurnOffLed();

// Initialize the buzzer for music note playing
void MusicInitBuzzer();

// Set the buzzer for music note and volume level
void MusicSetBuzzer(int pitch, int volume);

// Turn off buzzer
void MusicTurnOffBuzzer();

#endif /* MUSIC_H_ */
