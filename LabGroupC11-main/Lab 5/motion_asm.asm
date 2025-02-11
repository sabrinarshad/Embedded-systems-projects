;******************************************************************************
;******************************************************************************

        ; To include names declared in C
        .cdecls "motion.h"

        .text

        ; Field/pointer to motion sensor state variable in .c file
 					.ref motion_sensor
_motion_sensor 		.field motion_sensor

;******************************************************************************
; Motion signal pin usage: Grove base port J16, Tiva C PC4 (Port C, Pin 4)
;******************************************************************************

        .global CheckMotion
        .asmfunc

CheckMotion
        LDR     r0, _motion_sensor
        LDRB	r0, [r0, #4]
        BX		LR      			; return
        .endasmfunc
