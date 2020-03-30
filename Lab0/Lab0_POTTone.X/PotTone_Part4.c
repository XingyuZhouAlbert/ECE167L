/*
 * File:   PotTone_Part4.c
 * Author: xzhou45
 *
 * Created on January 8, 2020, 2:41 PM
 */


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
        // In here I make sure the frequency only changes if the new/old differences
        // is greater than 20 to eliminate the scratchy sound
        if ((potVal - newpotVal > 20) || (potVal - newpotVal < -20)) {
            potVal = newpotVal;

        }
        /* method two is to take an average value for a certain amount of sample
         * to eliminate the scratchy sound. But that doesn't sound as good as the first 
         * method */

        //        potVal = 0;
        //        while (x < 50) {
        //            potVal = AD_ReadADPin(AD_A0) + potVal;
        //            x++;
        //        }
        //        potVal = potVal / 51;
        //        x = 0;
        // Make sure I can hear the sound.
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
