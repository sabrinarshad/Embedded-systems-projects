/**
 * launchpad.h
 *
 * This file declares a set of functions for programming the TI Tiva C LaunchPad.
 *
 * Author: Zhao Zhang
 * First created: Summer 2014
 * Last revised: Spring 2022
 *
 *
 * Copyright (c) 2016, Zhao Zhang
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those
 * of the authors and should not be interpreted as representing official policies,
 * either expressed or implied, of the FreeBSD Project.
 */

#ifndef LAUNCHPAD_H_
#define LAUNCHPAD_H_

// Include commonly used header files for the Tiva C platform, including
// data types, stdio, device base addresses, system control, and interrupt
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <inc/hw_gpio.h>
#include <inc/hw_adc.h>
#include <inc/hw_timer.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/interrupt.h>
#include <driverlib/systick.h>
#include <driverlib/gpio.h>
#include <driverlib/uart.h>
#include <driverlib/adc.h>
#include <driverlib/timer.h>
#include <assert.h>

#include "event.h"

#define CPU_CLOCK_RATE  50000000L

// Initialize the most fundamental part of the LaunchPad
void LaunchPadInit();

/*****************************************************************************
 * Time functions
 ****************************************************************************/

// Wait for a given number of microseconds, using loop waiting
void WaitUs(uint32_t timeUs);

/*****************************************************************************
 * LEDs functions
 *
 * There are three LEDs: red, green, and blue.
 ****************************************************************************/

// Initialize the LEDs
void LedInit();

// Turn on/off the LEDs individually. There are three LEDs of color red, blue, or green.
void LedTurnOnOff(bool red, bool blue, bool green);

/****************************************************************************
 * Push button functions
 ****************************************************************************/

// Initialize the push button. This function is called from LaunchPadInit().
void PushButtonInit();

// Read push button input recorded by push button ISR
int PushButtonRead();

// Set callback event for push button ISR
void PushButtonEventRegister(Event *callback_event);

// Change the de-bouncing delay
void PushButtonSetDebouncingDelay(int debouncing_delay);

/****************************************************************************
 * UART functions
 ****************************************************************************/

// Enable UART0 with baud rate of 115,200 bps, and 8-N-1 frame
void UartInit();

// Send a character
void UartPutChar(char ch);

// Receive a character
char UartGetChar();

// Send a string
int UartPutString(char *buffer);

// Print a string through UART0 using an internal buffer of 80 characters.
// Return the number of characters printed.
int uprintf(char* fmt, ...);

#endif  // LAUNCHPAD_H_
