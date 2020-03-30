/*
 * File:   Lab1_TonePiezo.c
 * Author: xzhou45
 *
 * Created on January 21, 2020, 3:20 PM
 */


#include "xc.h"
#include <AD.h>
#include <Ascii.h>
#include <BOARD.h>
#include <Oled.h>
#include <OledDriver.h>
#include <ToneGeneration.h>
#include <stdio.h>


uint16_t piezoVal;
uint16_t flexVal;
uint16_t potVal;
uint16_t newpotVal = 0;
uint16_t linearizedVal;
uint16_t i;

int main(void) {
    BOARD_Init();
    ToneGeneration_Init();
    AD_Init();
    OledInit();
    AD_AddPins(AD_A2);
    AD_AddPins(AD_A3);
    char piezo [100];
    char linear [100];
    potVal = AD_ReadADPin(AD_A3);
    while (1) {
        newpotVal = AD_ReadADPin(AD_A3);
        // In here I make sure the frequency only changes if the new/old differences
        // is greater than 20 to eliminate the scratchy sound
        if ((potVal - newpotVal > 20) || (potVal - newpotVal < -20)) {
            potVal = newpotVal;
        }
        linearizedVal = (((potVal - 419.78) / 3.825) * 10) + 100;
        sprintf(linear, "\n%6d", linearizedVal);
        OledDrawString(linear);
        OledUpdate();

        piezoVal = AD_ReadADPin(AD_A2);
        sprintf(piezo, "%6d", piezoVal);
        OledDrawString(piezo);
        OledUpdate();
        if (piezoVal >= 100) {
            for(i=0;i<100;i++) {
                newpotVal = AD_ReadADPin(AD_A3);
                // In here I make sure the frequency only changes if the new/old differences
                // is greater than 20 to eliminate the scratchy sound
                if ((potVal - newpotVal > 20) || (potVal - newpotVal < -20)) {
                    potVal = newpotVal;
                }
                linearizedVal = (((potVal - 419.78) / 3.825) * 10) + 100;
                sprintf(linear, "\n%6d", linearizedVal);
                OledDrawString(linear);
                OledUpdate();
                ToneGeneration_SetFrequency(linearizedVal);
                ToneGeneration_ToneOn();
                piezoVal = AD_ReadADPin(AD_A2);
                sprintf(piezo, "%6d", piezoVal);
                OledDrawString(piezo);
                if(piezoVal >= 100){
                    i = 0;
                }
            }
            ToneGeneration_ToneOff();
        }
    }
}
