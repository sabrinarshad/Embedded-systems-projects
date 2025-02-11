/*
 * ras.h
 *
 * Rotary Angle Sensor module
 *
 */

#ifndef RA_SENSOR_H_
#define RA_SENSOR_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/sysctl.h>
#include <driverlib/interrupt.h>
#include <driverlib/adc.h>
#include "launchpad.h"

// Initialize RAS
void RASInit();

// Trigger RAS reading
void RASTriggerReading();

// Check if any new reading is ready (for I/O polling)
bool RASDataReady();

// Return angle reading in degrees
float RASDataRead();

// Set callback event for RAS ISR
void RASEventRegister(Event *event);

// The ISR function
void RASensorISR();

#endif /* RAS_H_ */
