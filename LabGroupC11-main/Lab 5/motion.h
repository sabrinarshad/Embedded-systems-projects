/******************************************************************************
 *****************************************************************************/

#ifndef MOTION_H_
#define MOTION_H_

#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <inc/hw_gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include "launchpad.h"

// Initialize the motion sensor
void MotionInit();

// Check the motion sensor
bool CheckMotion();

// Register an event object
void MotionSensorEventRegister(Event* callback_event);

#endif /* MOTION_H_ */
