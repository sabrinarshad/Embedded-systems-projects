/******************************************************************************
 *****************************************************************************/

#include "motion.h"

/******************************************************************************
 * Initialize the motion sensor
 * Pin usage: Grove base port J16, Tiva C PC4 (Port C, Pin 4)
 *****************************************************************************/
void MotionInit()
{
    // Enable Port C peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    // Configure PC4 as input pin
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4);
}
