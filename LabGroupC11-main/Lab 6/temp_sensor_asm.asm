; temp_sensor_asm.asm
; Lab 6, ECE 266, fall 2024
;
; Assembly code for the temperature sensor ISR
;
; Last update: Oct. 18, 2024

        ; To include names declared in C
        .cdecls "temp_sensor.h"

		; Reference to temp_sensor in temp_sensor.c, with the
		; following data structure:
		; typedef struct {
    	;	  Event *callback_event;
    	;     uint32_t raw_data;
    	;     bool new_input;
    	; } TempSensorState;
    	.ref temp_sensor

        .text
       	; fields to hold pointers
_temp_sensor	.field 		temp_sensor
_ADC0_BASE		.field 		ADC0_BASE

;******************************************************************************
; ISR for reading temperature data. It is called when ADC conversion is done.
;******************************************************************************
TempSensorISR
        .asmfunc
        push    {lr}

        ; Read the ADC data
        ; call ADCSequenceDataGet(ADC0_BASE, 0 /* sequencer */,
        ;               &temp_sensor.raw_data /* pointer to data buffer */);
        LDR     r0, _ADC0_BASE
        MOV     r1, #0
        LDR     r2, _temp_sensor
        ADD		r2, #4
        BL      ADCSequenceDataGet

        ; Set the flag for new data ready
        ; temp_sensor.new_input = true;
        LDR     r0, _temp_sensor
        MOV     r1, #true
        STRB    r1, [r0, #8]

        ; Schedule a data processing event
        ; call EventSchedule(temp_sensor.callback_event, EventGetCurrentTime());
        BL		EventGetCurrentTime
        MOV		r1, r0
        LDR		r0, _temp_sensor
        LDR		r0, [r0]
        BL		EventSchedule

        ; IMPORTANT: Clear the interrupt flag
        ; call ADCIntClear(ADC0_BASE, 0 /* sequencer */);
        LDR     r0, _ADC0_BASE
        MOV     r1, #0
        BL      ADCIntClear

        pop     {pc}
        .endasmfunc
