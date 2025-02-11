;
; clockupdate.asm: ECE 266 Lab 3 Starter code, fall 2024
;
; Assembly code for updating the 7-seg to show a running clock.
;
; Created by Zhao Zhang
;

; include C header file
        .cdecls "clockupdate.h"
        .text

; Declaration fields (pointers) to seg7 and ClockUpdate
_seg7           .field  seg7
_ClockUpdate    .field  ClockUpdate

;************************************************************************************
; Task 1: Update the clock
;
; C prototype: void ClockUpdate(uint32_t time)
;
; This is the ClockUpdate callback function in assembly. It replaces the
; same function in your lab2_main.c.
;
; This is the STARTER CODE. At this time it only flashes the colon of the 7-seg.
; Complete the code so that it updates all the four clock digits.
;************************************************************************************
        .global ClockUpdate
        .asmfunc
ClockUpdate     
        PUSH    {LR, r0}        	; push return address and event

        ; check seg7.colon_on and turn it on/off
        LDR     r0, _seg7           ; load the address of seg7 to r0
        LDRB    r1, [r0, #4]        ; r1 = seg7.colon_on
        CMP     r1, #0              ; compare r1 and 0
        BEQ     turn_on_colon       ; if the colon is turned off, jump to the code turning it on
        MOVS    r1, #0              ; now the colon must have been turned on, so turn it off
        STRB    r1, [r0, #4]        ; store 0 to seg7.colon_on
        B       update_seg7_and_return   ; jump to return

turn_on_colon
        MOV     r1, #1              ; r1 = 1
        STRB    r1, [r0, #4]        ; store 1 to seg7.colon_on

        ;**************************************************************
        ; YOUR CODE STARTS HERE
        ;**************************************************************

        ; Load the digit[0] (LSB) of seg7
        LDRB    r1, [r0]            ; r1 = seg7.digit[0]
        ADD     r1, #1              ; increment digit[0]
        CMP     r1, #10             ; if digit[0] == 10, reset and increment next
        BNE     store_digit0        ; if not 10, skip to storing digit[0]

        ; digit[0] == 10, reset to 0 and increment digit[1]
        MOV     r1, #0              ; reset digit[0]
        STRB    r1, [r0]            ; store updated digit[0]

        ; Load digit[1] and increment
        LDRB    r1, [r0, #1]        ; r1 = seg7.digit[1]
        ADD     r1, #1              ; increment digit[1]
        CMP     r1, #6              ; if digit[1] == 6, reset and increment next
        BNE     store_digit1        ; if not 6, skip to storing digit[1]

        ; digit[1] == 6, reset to 0 and increment digit[2]
        MOV     r1, #0              ; reset digit[1]
        STRB    r1, [r0, #1]        ; store updated digit[1]

        ; Load digit[2] and increment
        LDRB    r1, [r0, #2]        ; r1 = seg7.digit[2]
        ADD     r1, #1              ; increment digit[2]
        CMP     r1, #10             ; if digit[2] == 10, reset and increment next
        BNE     store_digit2        ; if not 10, skip to storing digit[2]

        ; digit[2] == 10, reset to 0 and increment digit[3]
        MOV     r1, #0              ; reset digit[2]
        STRB    r1, [r0, #2]        ; store updated digit[2]

        ; Load digit[3] and increment
        LDRB    r1, [r0, #3]        ; r1 = seg7.digit[3]
        ADD     r1, #1              ; increment digit[3]
        CMP     r1, #6              ; if digit[3] == 6, reset it
        BNE     store_digit3        ; if not 6, skip to storing digit[3]

        ; digit[3] == 6, reset to 0
        MOV     r1, #0              ; reset digit[3]
        STRB    r1, [r0, #3]        ; store updated digit[3]
        B       update_seg7_and_return ; jump to updating 7-segment

store_digit0
        STRB    r1, [r0]            ; store updated digit[0]
        B       update_seg7_and_return ; done, update 7-segment

store_digit1
        STRB    r1, [r0, #1]        ; store updated digit[1]
        B       update_seg7_and_return

store_digit2
        STRB    r1, [r0, #2]        ; store updated digit[2]
        B       update_seg7_and_return

store_digit3
        STRB    r1, [r0, #3]        ; store updated digit[3]


        ;**************************************************************
        ; YOUR CODE ENDS HERE
        ;**************************************************************

update_seg7_and_return
        ; Physically update the 7-seg
        ;    Call Seg7Update(&seg)
        BL      Seg7Update          ; note that r0 = &seg7 at this time

        ; Schedule a callback after 0.5 seconds:
        ;   Call ScheduleCallback(event, event->time + 500);
        POP     {r0}                ; restore event and time
        LDR     r1, [r0, #8]        ; r1 = event->time
        ADD     r1, #500            ; time += 500
        BL      EventSchedule

        POP     {PC}                ; return
        .end
