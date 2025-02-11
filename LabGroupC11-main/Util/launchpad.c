/**
 * launchpad.c
 *
 * This file includes miscellaneous functions for the TI LaunchPad.
 *
 * Zhao Zhang
 *
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

#include "launchpad.h"

// System tick clock frequency, which is the PIOSC in this library
#define SYSTICK_CLOCK_RATE 		16000000L

// maximum size for callback queue, see below
#define MAX_CALLBACK		    32

/***************************************************************************************
 * System time wait functions
 **************************************************************************************/

/*
 * Wait for a given number of microseconds
 */
void WaitUs(uint32_t timeUs)
{
    // Return immediately if timeMs is zero; otherwise, SysCtlDelay() would wait for a long time
    if (timeUs == 0)
        return;

    // Note: delayCount for SysCtlDelay() is three clock cycles
    uint32_t delay_count = (uint64_t) timeUs * CPU_CLOCK_RATE / 1000000;
    delay_count /= 3;
    SysCtlDelay(delay_count);
}

/*
 * Initialize Tiva C LaunchPad for ECE 266 labs
 */
void LaunchPadInit()
{
    /// Specifies a 50-MHz CPU clock with a PLL divider of 4, using the
    /// main OSC.
    // Note: The code is from the workshop workbook.
    // Guess: The base clock is 25MHz (default value), boosted by PLL to
    // 200MHz, and then divided by 4 to 50MHz (I checked the datasheet).
    // I am not sure about the last flag.
    SysCtlClockSet(
            SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN
                    | SYSCTL_XTAL_16MHZ);
    assert(SysCtlClockGet() == CPU_CLOCK_RATE);

    // Initialize the event-based scheduler
    EventSchedulerInit();

    // Initialize push button
    PushButtonInit();

    // Initialize the on-board LED
    LedInit();

    // Initialize UART0
    UartInit();
}
