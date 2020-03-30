/*
 * File:   HardwareFilter_Part5.c
 * Author: xzhou45
 *
 * Created on January 9, 2020, 4:31 PM
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

int main(void) {
    BOARD_Init();
    ToneGeneration_Init();
    AD_Init();
    OledInit();
    AD_AddPins(AD_A0);
    char Reading [100];
    //    int x = 0;
    potVal = AD_ReadADPin(AD_A0);
    while (1) {
        newpotVal = AD_ReadADPin(AD_A0);
        if((potVal - newpotVal > 20) || (potVal - newpotVal < -20)){
            potVal = newpotVal;
            
        }
        //        potVal = 0;
        //        while (x < 50) {
        //            potVal = AD_ReadADPin(AD_A0) + potVal;
        //            x++;
        //        }
        //        potVal = potVal / 51;
        //        x = 0;
                if (potVal < 20) {
                    potVal = 20;
                } else if (potVal > 950) {
                    potVal = 950;
                }
        
                sprintf(Reading, "\n%4d", potVal);
                OledDrawString(Reading);
                OledUpdate();
                ToneGeneration_SetFrequency(potVal);
                ToneGeneration_ToneOn();

        //        potVal = AD_ReadADPin(AD_A0);
        //        newpotVal = AD_ReadADPin(AD_A0);
        //        if (newpotVal - potVal > 10) {
        //            if (newpotVal < 20) {
        //                newpotVal = 20;
        //            } else if (newpotVal > 950) {
        //                newpotVal = 950;
        //            }
        //            sprintf(Reading, "\n%4d", newpotVal);
        //            OledDrawString(Reading);
        //            OledUpdate();
        //            ToneGeneration_SetFrequency(newpotVal);
        //            ToneGeneration_ToneOn();
        //        } 
        //        else {
        //            if (potVal < 20) {
        //                potVal = 20;
        //            } else if (potVal > 950) {
        //                potVal = 950;
        //            }
        //
        //            sprintf(Reading, "\n%4d", potVal);
        //            OledDrawString(Reading);
        //            OledUpdate();
        //            ToneGeneration_SetFrequency(potVal);
        //            ToneGeneration_ToneOn();
        //        }

    }

}