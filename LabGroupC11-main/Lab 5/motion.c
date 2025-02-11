/******************************************************************************
 *****************************************************************************/

#include "motion.h"

// Motion Sensor input state
typedef struct {
    Event *callback_event;
    bool pin_value;
} MotionSensorState;

// Declare a data structure and variable
MotionSensorState motion_sensor;

// pre-declare the ISR function
void MotionSensorISR();

/******************************************************************************
 * Initialize the motion sensor
 * Pin usage: Grove base port J16, Tiva C PC4 (Port C, Pin 4)
 *****************************************************************************/
void MotionInit()
{

    // Initialize push button state
    motion_sensor.callback_event = NULL;
    motion_sensor.pin_value = false;

    // Enable Port C peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    // Configure PC4 as input pin
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4);

    // Register the interrupt handler
    GPIOIntRegister(GPIO_PORTC_BASE, MotionSensorISR);

    // Interrupts on both edges (motion detected, motion stopped)
    GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_BOTH_EDGES);

    // Enable interrupts on motion sensor
    GPIOIntEnable(GPIO_PORTC_BASE, GPIO_PIN_4);
}

void MotionSensorEventRegister(Event* callback_event)
{
    assert(EventInitialized(callback_event));
    motion_sensor.callback_event = callback_event;
}

/*
 * Motion Sensor ISR
 */
void MotionSensorISR()
{

    // Read PC4 into pin_value
    motion_sensor.pin_value = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4);


    // Schedule callback event
    if (motion_sensor.callback_event != NULL)
        EventSchedule(motion_sensor.callback_event, EventGetCurrentTime());

    // IMPORTANT: Clear interrupt flag
    GPIOIntClear(GPIO_PORTC_BASE, GPIO_PIN_4);
}
