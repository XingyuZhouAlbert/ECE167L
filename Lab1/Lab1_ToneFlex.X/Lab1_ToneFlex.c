/*
 * File:   Lab1_ToneFlex.c
 * Author: xzhou45
 *
 * Created on January 16, 2020, 5:30 PM
 */


#include "xc.h"
#include <BOARD.h>
#include <ToneGeneration.h>
#include <AD.h>
#include <pwm.h>
#include <Oled.h>
#include <OledDriver.h>
#include <Ascii.h>
#include <stdio.h>

//Set up my variables to store the pot reading
uint16_t potVal;
uint16_t newpotVal = 0;
uint16_t linearizedVal;
uint16_t tone;

int main(void) {
    BOARD_Init();
    ToneGeneration_Init();
    AD_Init();
    OledInit();
    AD_AddPins(AD_A3);
    char Reading [100];
    char test[100];
    potVal = AD_ReadADPin(AD_A3);
    while (1) {
        newpotVal = AD_ReadADPin(AD_A3);
        // In here I make sure the frequency only changes if the new/old differences
        // is greater than 20 to eliminate the scratchy sound
        if ((potVal - newpotVal > 20) || (potVal - newpotVal < -20)) {
            potVal = newpotVal;
        }
        sprintf(test, "%6d", potVal);
        OledDrawString(test);
        OledUpdate();
        linearizedVal = (((potVal - 419.78) / 3.825) * 10) + 100;

        //        if (potVal < 20) {
        //            potVal = 20;
        //        } else if (potVal > 950) {
        //            potVal = 950;
        //        }
        if (linearizedVal > 950) {
            ToneGeneration_ToneOff();
        } else {
            sprintf(Reading, "\n%6d", linearizedVal);
            OledDrawString(Reading);
            OledUpdate();
            ToneGeneration_SetFrequency(linearizedVal);
            ToneGeneration_ToneOn();
        }
    }
}
