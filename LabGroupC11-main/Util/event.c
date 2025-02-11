/*
 * eventq.c: Event scheduler in Util.lib for ECE 266
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

#include "event.h"

/*************************************************************************************
 * An event-driven scheduling system using sysTime
 ************************************************************************************/

// Number of bins
#define BIN_BITSIZE     4
#define BIN_NUM         (1 << BIN_BITSIZE)
#define BIN_MASK        (BIN_NUM - 1)

// A system time in milliseconds since the LaunchPad is powered on
static volatile time_t sys_time = 0;

// The data type for the head event of each bin. The sequence of those three fields must be consistent
// with that of Event type.
struct HeadEvent {
    Event *prev, *next;
    time_t time;
    uint8_t heap_position;
};

// Events are organized as bins of linked list
static HeadEvent bin_head[BIN_NUM];
static uint8_t current_bin_selection = 0;

// Heap for the earliest event time of each event tin
static uint8_t bin_head_heap[BIN_NUM];

// Bin head time is defined as the earliest event time for all events in a bin
#define BIN_HEAD_TIME(i) (bin_head[bin_head_heap[(i)]].next->time)

/*
 * Heapify the bin head heap downwards
 */
void HeapifyDownwards(uint8_t i)
{
    while (i < BIN_NUM)
    {
        // Check on the left child
        int left = (i << 1);
        if (left >= BIN_NUM)
            return;

        // Check on the right child and make a choice between the left and the right
        int right = left + 1, swap;
        if (right >= BIN_NUM || BIN_HEAD_TIME(left) <= BIN_HEAD_TIME(right))
            swap = left;
        else
            swap = right;

        // Return if the parent and the children are in-order
        if (BIN_HEAD_TIME(i) <= BIN_HEAD_TIME(swap))
            break;

        // Swap with the child
        uint8_t tmp = bin_head_heap[i];
        bin_head_heap[i] = bin_head_heap[swap];
        bin_head_heap[swap] = tmp;
        bin_head[bin_head_heap[i]].heap_position = i;
        bin_head[bin_head_heap[swap]].heap_position = swap;

        i = swap;
    }
}


/*
 * Heapify the bin head heap upwards
 */
void HeapifyUpwards(uint8_t i)
{
    while (i > 0)
    {
        int parent = (i >> 1);
        if (BIN_HEAD_TIME(i) < BIN_HEAD_TIME(parent))
        {
            // Swap the parent and the child
            uint8_t tmp = bin_head_heap[i];
            bin_head_heap[i] = bin_head_heap[parent];
            bin_head_heap[parent] = tmp;
            bin_head[bin_head_heap[i]].heap_position = i;
            bin_head[bin_head_heap[parent]].heap_position = parent;
        }
        i = parent;
    }
}

// System tick interrupt handler, for maintaining system time in microseconds
// FIXME The system can be made more efficient if the CPU sleeps until the next
// event arrives. Right now the system wakes up every 1 millisecond.
static void
SysTickIntHandler()
{
    sys_time++;
}

/*
 * Initialize the event scheduler
 */
void EventSchedulerInit()
{
    uint8_t i;

    // Initialize bin_head and bin_head_heap
    for (i = 0; i < BIN_NUM; i++)
    {
        HeadEvent *head = &bin_head[i];
        head->prev = (Event *)head;
        head->next = (Event *)head;
        head->time = MAX_EVENT_TIME;

        bin_head_heap[i] = i;
        head->heap_position = i;
    }

    /// Get the system running clock
    uint32_t clock_rate = SysCtlClockGet();

    // Configure and enable System Tick for 1 millisecond period,
    // and register an interrupt handler to maintain system time.
    // FIXME The overhead of 1ms interrupt can be high for some application.
    // It is desirable to make the period configurable.
    SysTickPeriodSet(clock_rate / 1000 - 1);
    SysTickIntRegister(SysTickIntHandler);
    SysTickIntEnable();
    SysTickEnable();
}

/*
 * Initialize an event object
 */
void EventInit(Event *ev, Callback callback)
{
    // Set callback function and map the event to an event bin
    ev->callback = callback;
    ev->bin = current_bin_selection;
    ev->time = MAX_EVENT_TIME;
    ev->flags.initialized = true;
    ev->flags.scheduled = false;
    current_bin_selection = (current_bin_selection + 1) % BIN_NUM;
}

/*
 * Schedule an event at a given time
 */
void EventSchedule(Event *ev, time_t time)
{
    assert(ev->flags.initialized == true);

    HeadEvent *head = &bin_head[ev->bin];
    time_t old_bin_head_time = head->next->time;
    Event *ev2 = head->next;

    // Double check the event is not scheduled; if it is, remove
    // it from the linked list (it will be re-inserted)
    if (ev->flags.scheduled)
    {
        ev->prev->next = ev->next;
        ev->next->prev = ev->prev;
    }

    // Search for the insert position of the event in the linked list
    while (ev2 != (Event *) head)
    {
        if (ev2->time > ev->time)
            break;
        ev2 = ev2->next;
    }

    // Insert the event into the linked list and update its state
    ev->prev = ev2->prev;
    ev2->prev->next = ev;
    ev2->prev = ev;
    ev->next = ev2;
    ev->time = time;
    ev->flags.scheduled = true;

    // The bin head time may decrease for a new event, or increase for
    // a re-scheduled event, so heapify may be needed
    if (head->next->time < old_bin_head_time)
        HeapifyUpwards(head->heap_position);
    else if (head->next->time > old_bin_head_time)
        HeapifyDownwards(head->heap_position);
}

/*
 * De-schedule a scheduled event
 */
void EventDeschedule(Event *ev)
{
    // Get the current bin head and bin head time
    assert(ev->flags.scheduled);
    HeadEvent *head = &bin_head[ev->bin];
    time_t old_bin_head_time = head->next->time;

    // Remove the event from the linked list
    ev->prev->next = ev->next;
    ev->next->prev = ev->prev;
    ev->flags.scheduled = false;

    // The bin's earliest event time may increase, if so, heapfiy downwards
    if (head->next->time > old_bin_head_time)
        HeapifyDownwards(ev->bin);
}

/*
 * Check and execute all events whose time is ready
 */
void EventExecute()
{
    while (true)
    {
        // If the first event of the root bin is not yet ready, exit loop
        Event *ev = bin_head[bin_head_heap[0]].next;
        if (ev->time > sys_time)
            break;

        // Remove the event from the linked list and update its state
        ev->prev->next = ev->next;
        ev->next->prev = ev->prev;
        ev->flags.scheduled = false;

        // Heapify the bin heap because the bin head time may have changed
        HeapifyDownwards(0);

        // Call the callback function
        ev->callback(ev);
    }
}

time_t EventGetCurrentTime()
{
    return sys_time;
}



