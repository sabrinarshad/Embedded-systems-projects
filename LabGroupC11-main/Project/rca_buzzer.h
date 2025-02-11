/*
 * rca_buzzer.h: RCA functions
 */

#ifndef RCA_BUZZER_H
#define RCA_BUZZER_H

#include "launchpad.h"

// Initialize the buzzer to act as alarm
void RCAInitBuzzer();

// Set the buzzer depending on the distance
void RCASetBuzzer(float distance, float volume);

// Turn off buzzer
void RCATurnOffBuzzer();

#endif /* RCA_BUZZER_H_ */
