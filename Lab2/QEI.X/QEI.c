/*
 * File:   QEI.c
 * Author: xzhou45
 *
 * Created on January 29, 2020, 4:08 PM
 */


#include "xc.h"
#include <BOARD.h>
#include <Ascii.h>
#include <Oled.h>
#include <OledDriver.h>
#include <QEI.h>
#include <stdio.h>




char Apin;
char Bpin;
char nApin;
char nBpin;
int counter;
int enState;
int oldenState;

/**
 * @function QEI_Init(void)
 * @param none
 * @brief  Enables the Change Notify peripheral and sets up the interrupt, anything
 *         else that needs to be done to initialize the module. 
 * @return SUCCESS or ERROR (as defined in BOARD.h)
 */
char QEI_Init(void) {
    // INIT Change notify
    CNCONbits.ON = 1; // Change Notify On
    CNENbits.CNEN15 = 1; //enable one phase
    CNENbits.CNEN16 = 1; //enable other phase
    int temp = PORTD; // this is intentional to ensure a interrupt occur immediately upon enabling
    IFS1bits.CNIF = 0; // clear interrupt flag
    IPC6bits.CNIP = 1; //set priority
    IPC6bits.CNIS = 3; // and sub priority
    IEC1bits.CNIE = 1; // enable change notify
    // the rest of the function goes here
    // Indicate the pins attached to the system
    TRISDbits.TRISD6 = 1;
    TRISDbits.TRISD7 = 1;
    Apin = PORTDbits.RD6;
    Bpin = PORTDbits.RD7;
    //initialize the counter
    counter = 0;
    enState = 0;
    oldenState = 0;

    //set states for my previous reading
    // define each states
    if (Apin == 0 && Bpin == 0) {
        oldenState = 0;
    }
    if (Apin == 1 && Bpin == 0) {
        oldenState = 10;
    }
    if (Apin == 0 && Bpin == 1) {
        oldenState = 1;
    }
    if (Apin == 1 && Bpin == 1) {
        oldenState = 11;
    }


}

/**
 * @function QEI_GetPosition(void) 
 * @param none
 * @brief This function returns the current count of the Quadrature Encoder in ticks.      
 */
int QEI_GetPosition(void) {
    return counter;
}

/**
 * @Function QEI_ResetPosition(void) 
 * @param  none
 * @return none
 * @brief  Resets the encoder such that it starts counting from 0.
 */
void QEI_ResetPosition() {

    counter = 0;
}

void __ISR(_CHANGE_NOTICE_VECTOR) ChangeNotice_Handler(void) {
    static char readPort = 0;
    readPort = PORTD; // this read is required to make the interrupt work
    IFS1bits.CNIF = 0;
    //anything else that needs to happen goes here
    // read the new pin value and compare with the old one.
    nApin = PORTDbits.RD6;
    nBpin = PORTDbits.RD7;

    // define each states
    if (nApin == 0 && nBpin == 0) {
        enState = 0;
    }
    if (nApin == 1 && nBpin == 0) {
        enState = 10;
    }
    if (nApin == 0 && nBpin == 1) {
        enState = 1;
    }
    if (nApin == 1 && nBpin == 1) {
        enState = 11;
    }



    if (enState == 0) {
        if (oldenState == 1) {
            counter++;
            if (counter > 96) {
                counter = 0;
            }
        } else if (oldenState == 10) {
            counter--;
            if (counter < -96) {
                counter = 0;
            }
        }
    } else if (enState == 10) {
        if (oldenState == 11) {
            counter--;
            if (counter < -96) {
                counter = 0;
            }
        } else if (oldenState == 0) {
            counter++;
            if (counter > 96) {
                counter = 0;
            }
        }
    } else if (enState == 11) {
        if (oldenState == 1) {
            counter--;
            if (counter < -96) {
                counter = 0;
            }
        } else if (oldenState == 10) {
            counter++;
            if (counter > 96) {
                counter = 0;
            }
        }
    } else if (enState == 1) {
        if (oldenState == 0) {
            counter--;
            if (counter < -96) {
                counter = 0;
            }
        } else if (oldenState == 11) {
            counter++;
            if (counter > 96) {
                counter = 0;
            }
        }
    }
    oldenState = enState;
}




