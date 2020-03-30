/*
 * File:   PING.c
 * Author: xzhou45
 *
 * Created on February 3, 2020, 3:34 PM
 */


#include "xc.h"
#include <BOARD.h>
#include <timers.h>
#include <AD.h>
#include <Oled.h>
#include <OledDriver.h>
#include <PING.h>
#include <proc/p32mx320f128h.h>


uint16_t distance;
uint16_t start;
uint16_t finish;
uint16_t flighttime;
char echoPin;
char curechoPin;
uint16_t newtime;
uint16_t oldtime;

/**
 * @function    PING_Init(void)
 * @brief       Sets up both the timer and Change notify peripherals along with their
 *              respective interrupts. Also handles any other tasks needed such as pin 
 *              I/O directions, and any other things you need to initialize the sensor.
 *              TIMERS library must be inited before this library.
 * @return      SUCCESS or ERROR (as defined in BOARD.h)
 */
char PING_Init(void) {
    // following block inits the timer
    T4CON = 0;
    T4CONbits.TCKPS = 0b110;
    PR4 = 0xFFFF; // this is not the timer value wanted
    T4CONbits.ON = 1;
    IFS0bits.T4IF = 0;
    IPC4bits.T4IP = 3;
    IEC0bits.T4IE = 1;

    // following block inits change notify
    CNCONbits.ON = 1; // Change Notify On
    CNENbits.CNEN14 = 1;
    int temp = PORTD; // this is intentional to ensure a interrupt occur immediately upon enabling
    IFS1bits.CNIF = 0; // clear interrupt flag
    IPC6bits.CNIP = 1; //set priority
    IPC6bits.CNIS = 3; // and sub priority
    IEC1bits.CNIE = 1; // enable change notify
    //Anything else that needs to occur goes here
    //Set up the input pin
    TRISDbits.TRISD5 = 1; // This is pin #35 aka RD5
    echoPin = PORTDbits.RD5; //Save the initial state of the echo pin
    TRISDbits.TRISD8 = 0; // enable pin #2 for the trigger
    LATDbits.LATD8 = 0; // set the initial state to low.
    //initialize variable
    distance = 0;
    start = 0;
    finish = 0;
    flighttime = 0;
    PR4 = 6; //Here I just set it to 6 since it stands for 10 us.
    oldtime = 0;
}

void __ISR(_CHANGE_NOTICE_VECTOR) ChangeNotice_Handler(void) {
    static char readPort = 0;
    readPort = PORTD; // this read is required to make the interrupt work
    IFS1bits.CNIF = 0;
    //Anything else that needs to occur goes here
    curechoPin = PORTDbits.RD5; //Read the status of the pulse, and calculate the time difference
    if (curechoPin != echoPin) { //Rising edge since the initial stage is 0.
        start = TIMERS_GetMicroSeconds();
    } else {
        finish = TIMERS_GetMicroSeconds();
    }
    // same software filter used in lab0 to reduce the noise from the distance reading
    newtime = finish - start;
    //    unsigned int difference = newtime - oldtime;
    //    if (((oldtime - newtime) > 5300) || ((oldtime - newtime) < -5300)) {
    //        oldtime = newtime;
    //
    //    }

}

void __ISR(_TIMER_4_VECTOR) Timer4IntHandler(void) {
    IFS0bits.T4IF = 0;
    //Anything else that needs to occur goes here
    // Here I just simply switch between two states
    // if it's already low, then switch to high. Vice versa.
    // PR4 = 6 stands for 10 us. Because according to my calculation, 1 ticks means 1.7 us.
    if (PR4 == 6) {
        PR4 = 37500;
        LATDbits.LATD8 = 0;
    } else if (PR4 == 37500) { //37500 is roughly 60.01 ms.
        PR4 = 6;
        LATDbits.LATD8 = 1;
    }

}

/**
 * @function    PING_GetDistance(void)
 * @brief       Returns the calculated distance in mm using the sensor model determined
 *              experimentally. 
 *              No I/O should be done in this function
 * @return      distance in mm
 */
unsigned int PING_GetDistance(void) {
    distance = (((PING_GetTimeofFlight() + 29) / 57));
    return distance;
}

/**
 * @function    PING_GetTimeofFlight(void)
 * @brief       Returns the raw microsecond duration of the echo from the sensor.
 *              NO I/O should be done in this function.
 * @return      time of flight in uSec
 */
unsigned int PING_GetTimeofFlight(void) {

    return newtime;
}






