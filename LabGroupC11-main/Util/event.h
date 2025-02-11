/*
 * eventq.h: Event scheduler in Util.lib for ECE 266
 *
 *  Created on: Dec 22, 2023
 *      Author: zzhang
 *  Last update on Jan. 12, 2024
 *
 *
 * Copyright (c) 2023, Zhao Zhang
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


#ifndef EVENT_H_
#define EVENT_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <inc/hw_ints.h>
#include <driverlib/systick.h>
#include <driverlib/sysctl.h>

// Use 32-bit time in the current implementation
typedef uint32_t time_t;
#define MAX_EVENT_TIME      0xFFFFFFFF
#define IMMEDIATE           0x00000000

typedef struct Event Event;
typedef struct HeadEvent HeadEvent;

// Define a type for callback functions
typedef void (*Callback)(Event *ev);

// Data structure for events. Events will be organized in double linked lists.
struct Event
{
    Event *prev, *next;                 // linked list of events
    time_t time;                        // time of this event, 0 if immediately
    uint8_t bin;                        // the bin that the event is mapped to; see event.c
    struct
    {
        uint8_t initialized : 1;        // if the event is initialized
        uint8_t scheduled : 1;          // if the event is scheduled
    } flags;
    Callback callback;                  // callback function of this event
};

void EventSchedulerInit();
void EventInit(Event *event, Callback callback);
void EventSchedule(Event *event, time_t time);
void EventDeschedule(Event* event);
void EventExecute();
time_t EventGetCurrentTime();

inline bool EventInitialized(Event *event)
{
    return event->flags.initialized;
}

#endif /* EVENT_H_ */
