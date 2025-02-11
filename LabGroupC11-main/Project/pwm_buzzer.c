/*
 * pwm_buzzer.c: PWM Buzzer functions
 */

#include <pwm_buzzer.h>

/*
 * Initialize the timer PWM functions connected to the buzzer
 *      Buzzer:     PC5, WT0CCP1, Wide Timer 0, sub-timer B
 */
void PwmBuzzerInit()
{
    // Enable Timer 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    // Connect pins to Timer
    GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_5);
    GPIOPinConfigure(GPIO_PC5_WT0CCP1);

    // Select PWM for Timer 0, sub-Timer B
    TimerConfigure(WTIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM));

    // Invert the PWM waveform, so that the Match register value is the pulse width.
    // Otherwise, the pulse width will be (Load value) - (Match value).
    // The inversion is done by enabling output inversion on the PWM pins.
    TimerControlLevel(WTIMER0_BASE, TIMER_B, true /* output inversion */);

    // Enable the Timer 0's TimerB
    TimerEnable(WTIMER0_BASE, TIMER_B);
}

/*
 * Set buzzer frequency and volume
 */
void PwmBuzzerSet(float frequency, float volume)
{
    int period = (int)(50000000 / frequency); // Get the current PWM period
    int pulse_width = (int)(volume * period); // Calculate the pulse width based on volume percentage
    TimerLoadSet(WTIMER0_BASE, TIMER_B, period); // Set the load value value (period) for sub-timer B
    TimerMatchSet(WTIMER0_BASE, TIMER_B, pulse_width); // Set the match value (pulse width) for sub-timer B
}


/*
 * Turn on buzzer
 */
void BuzzerOn()
{
    TimerEnable(WTIMER0_BASE, TIMER_B);
}

/*
 * Turn off buzzer
 */
void BuzzerOff()
{
    TimerDisable(WTIMER0_BASE, TIMER_B);
}
