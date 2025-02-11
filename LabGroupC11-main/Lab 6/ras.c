/*
 * ras.c: Starter code for ECE 266 Lab 6, spring 2024
 *
 * Rotary angle sensor module
 *
 */

#include "ras.h"

/*
 * Global declarations
 */

// Rotary angle sensor reading states
typedef struct {
    Event *callback_event;      // the callback event for data processing
    uint32_t raw_data;          // the raw reading from the ADC connected to the sensor
    bool new_input;             // if any new data input is ready
} RASensorState;

RASensorState ra_sensor;

/*
 * Initialize ADC to use Rotary angle sensor
 *
 * Resources: ADC0, sequence #1, special channel 7
 * Configurations: processor trigger, interrupt enabled, use step 0 only
 */
void RASInit()
{
    // Initialize sensor state in memory
    ra_sensor.callback_event = NULL;
    ra_sensor.new_input = false;

    // Enable the ADC0 peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    // Configure ADC0's sequencer #1
    ADCSequenceConfigure(ADC0_BASE, 1 /* sequencer */, ADC_TRIGGER_PROCESSOR, 0 /* priority */);

    // Configure step 0 of sequencer 1 to use the rotary angle sensor, with
    // interrupt enable, as the end of the sequence
    ADCSequenceStepConfigure(ADC0_BASE, 1 /* sequencer */, 0 /* step */,
                             ADC_CTL_CH7 | ADC_CTL_IE | ADC_CTL_END);

    // Configure the interrupt system for rotary angle sensor
    ADCIntRegister(ADC0_BASE, 1 /* sequencer */, RASensorISR);
    ADCIntEnable(ADC0_BASE, 1 /* sequencer */);

    // Enable ADC0, sequencer 1
    ADCSequenceEnable(ADC0_BASE, 1 /* sequencer */);
}

/*
 * Trigger angle reading
 */
void RASTriggerReading()
{
    // Trigger the sampling
    ADCProcessorTrigger(ADC0_BASE, 1 /* sequencer */);
}

/*
 * Check if any new data is ready (for I/O polling)
 */
bool RASDataReady()
{
    return ra_sensor.new_input;
}

/*
 * Read the current input from the rotary angle sensor
 */
float RASDataRead()
{

    // Reset new data ready flag and return the reading
    ra_sensor.new_input = false;
    return ra_sensor.raw_data;
}

/*
 * Set callback event for rotary angle sensor ISR
 */
void RASEventRegister(Event *event)
{
    ra_sensor.callback_event = event;
}

/*
 * ISR for rotary angle sensor data
 */
void RASensorISR()
{
    // Read the ADC data
    ADCSequenceDataGet(ADC0_BASE, 1 /* sequencer */,
                       &ra_sensor.raw_data /* pointer to raw data */);

    // Set the flag for new input
    ra_sensor.new_input = true;

    // Schedule the callback event
    if (ra_sensor.callback_event != NULL)
        EventSchedule(ra_sensor.callback_event, EventGetCurrentTime());

    // IMPORTANT: Clear the interrupt flag
    ADCIntClear(ADC0_BASE, 1 /* sequencer */);
}
