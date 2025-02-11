/*
 * pwm_buzzer.h: PWM Buzzer Functions
 */

#ifndef PWM_BUZZER_H_
#define PWM_BUZZER_H_

#include <stdbool.h>
#include <stdint.h>
#include <inc/hw_memmap.h>
#include <inc/hw_timer.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>

/*
 * Initialize the timer PWM functions connected to the buzzer.
 */
void PwmBuzzerInit();

/*
 * Set the frequency and volume for the buzzer
 */
void PwmBuzzerSet(float frequency, float volume);

/*
 * Turn on the buzzer
 */
void BuzzerOn();

/*
 * Turn off the buzzer
 */
void BuzzerOff();

#endif /* PWM_BUZZER_H_ */
