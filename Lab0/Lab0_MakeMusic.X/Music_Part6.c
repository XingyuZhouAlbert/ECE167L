/*
 * File:   Music_Part6.c
 * Author: xzhou
 *
 * Created on January 8, 2020, 8:40 PM
 */

#include <BOARD.h>
#include <AD.h>
#include <ToneGeneration.h>
#include <pwm.h>
#include <stdio.h>
#include <Oled.h>
#include <OledDriver.h>
#include <Ascii.h>
#include "xc.h"

#define BTN1_Down 0x01
#define BTN2_Down 0x02
#define BTN3_Down 0x04
#define BTN4_Down 0x08
static uint8_t buttonPressed;
uint16_t potVal;
uint16_t newpotVal;

int main(void) {
    BOARD_Init();
    OledInit();
    ToneGeneration_Init();
    AD_Init();
    AD_AddPins(AD_A0);
    char Reading [100];
    potVal = AD_ReadADPin(AD_A0);
    while (1) {
        newpotVal = AD_ReadADPin(AD_A0);
        if ((potVal - newpotVal > 20) || (potVal - newpotVal < -20)) {
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
        buttonPressed = BUTTON_STATES();
        if (buttonPressed == BTN1_Down) {
            ToneGeneration_SetFrequency(261.63+potVal);
            OledDrawChar(1, 1, 'C');
            OledUpdate();
            ToneGeneration_ToneOn();
        } else if (buttonPressed == BTN1_Down + BTN2_Down) {
            ToneGeneration_SetFrequency(392+potVal);
            OledDrawChar(1, 1, 'G');
            ToneGeneration_ToneOn();
        } else if (buttonPressed == BTN1_Down + BTN3_Down) {
            ToneGeneration_SetFrequency(440+potVal);
            OledDrawChar(1, 1, 'A');
            ToneGeneration_ToneOn();
        } else if (buttonPressed == BTN1_Down + BTN4_Down) {
            ToneGeneration_SetFrequency(493.88+potVal);
            OledDrawChar(1, 1, 'B');
            ToneGeneration_ToneOn();
        } else if (buttonPressed == BTN2_Down) {
            ToneGeneration_SetFrequency(293.66+potVal);
            OledDrawChar(1, 1, 'D');
            ToneGeneration_ToneOn();
        } else if (buttonPressed == BTN3_Down) {
            ToneGeneration_SetFrequency(329.63+potVal);
            OledDrawChar(1, 1, 'E');
            ToneGeneration_ToneOn();
        } else if (buttonPressed == BTN4_Down) {
            ToneGeneration_SetFrequency(349.23+potVal);
            OledDrawChar(1, 1, 'F');
            ToneGeneration_ToneOn();
        } else {
            OledClear(OLED_COLOR_BLACK);
            ToneGeneration_ToneOff();
        }
    }
}
