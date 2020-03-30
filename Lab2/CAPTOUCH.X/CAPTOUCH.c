/*
 * File:   CAPTOUCH.c
 * Author: xzhou45
 *
 * Created on February 6, 2020, 11:47 AM
 */


#include <proc/p32mx320f128h.h>

#include "xc.h"
#include <BOARD.h>
#include <CAPTOUCH.h>
#include <timers.h>
#include <stdio.h>


unsigned int timediff;
unsigned int t;
unsigned int current;
unsigned int last;

/**
 * @function    CAPTOUCH_Init(void)
 * @brief       This function initializes the module for use. Initialization is 
 *              done by opening and configuring timer 2, opening and configuring the input capture
 *              peripheral, and setting up the interrupt.
 * @return      SUCCESS or ERROR (as defined in BOARD.h)
 */
char CAPTOUCH_Init(void) {
    // following block inits the timer
    T2CON = 0;
    T2CONbits.TCKPS = 0b011;
    PR2 = 0xFFFF;
    T2CONbits.ON = 1;

    //this block inits input capture
    IC4CON = 0;
    IC4CONbits.ICTMR = 1;
    IC4CONbits.ICM = 0b010;

    IFS0bits.IC4IF = 0;
    IPC4bits.IC4IP = 7;
    IEC0bits.IC4IE = 1;
    IC4CONbits.ON = 1;
    // whatever else you need to do to initialize your module
    //    TRISDbits.TRISD11 = 1; //Pin #35

    // values init
    t = 0;
    timediff = 0;
    current = 0;
    last = 0;
}

void __ISR(_INPUT_CAPTURE_4_VECTOR) InputCapture_Handler(void) {
    IFS0bits.IC4IF = 0;
    // IC4BUF contains the timer value when the rising edge occurred.
    current = IC4BUF;
    //if the last time is 0, it means that ISR just got triggered for the first time 

    if (last == 0) {
        last = current; //update the last time;
    } else {
        // some random large number in the reading will cause flickering
        // this is to make sure I get rid off it.
        if (current - last < 10000) {
            timediff = current - last;
        }
        last = current; // update the last time again to current time since I'll only be reading the current time in ISR
    }

}

/**
 * @function    CAPTOUCH_IsTouched(void)
 * @brief       Returns TRUE if finger is detected. Averaging of previous measurements 
 *              may occur within this function, however you are NOT allowed to do any I/O
 *              inside this function.
 * @return      TRUE or FALSE
 */
char CAPTOUCH_IsTouched(void) {
    printf("%u\r\n", timediff);
    // the threshold is different each time I got on a different pc.
    if (timediff < 4100) {
        // sensor is not touched
        return 0;
    } else if (timediff > 4100) {
        return 1;
    }

}

