/*
 * Starter code for ECE 266 Lab 7, Sound Player (PWM), fall 2024
 *
 * pwmled.c: PWM LED functions
 *
 * Created by Zhao Zhang
 */

#ifndef PWM_LED_H_
#define PWM_LED_H_

#include <stdbool.h>
#include <stdint.h>
#include <inc/hw_memmap.h>
#include <inc/hw_timer.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>

/*
 * Initialize the timer PWM functions connected to the three sub-LEDs.
 */
void PwmLedInit();

/*
 * Set the LED parameters for the three sub-LEDs
 */
void PwmLedSet(int pulsePeriod, int pulseWidthRed, int pulseWidthBlue, int pulseWidthGreen);

#endif /* PWDLED_H_ */
