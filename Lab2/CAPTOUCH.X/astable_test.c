///*
// * File:   astable_test.c
// * Author: xzhou45
// *
// * Created on February 6, 2020, 12:41 PM
// */
//
//
//#include "xc.h"
//#include <BOARD.h>
//#include <pwm.h>
//#include <AD.h>
//#include <Oled.h>
//#include <OledDriver.h>
//#include <stdio.h>
//#include <timers.h>
//
//unsigned int ADRead;
//unsigned int newAD;
//char Reading[100];
//char AVG[50];
//
//int main(void) {
//    BOARD_Init();
//    OledInit();
//    AD_Init();
//    TIMERS_Init();
//    int i = 0;
//    AD_AddPins(AD_A3);
//    while (1) {
//        i = 0;
//        newAD = 0;
//        OledClear(0);
//        //        ADRead = AD_ReadADPin(AD_A3);
//
//        //        if (ADRead < 800) {
//        //            OledDrawString("\nNO TAP\n");
//        //            OledUpdate();
//        //        } else if (ADRead >= 900) {
//        //            OledDrawString("\n\nTAP");
//        //            OledUpdate();
//        //        }
//        for (i = 0; i < 500; i++) {
//            ADRead = AD_ReadADPin(AD_A3);
//            newAD += ADRead;
//        }
//        ADRead = newAD / i;
//        sprintf(Reading, "%4d\n", ADRead);
//        OledDrawString(Reading);
//        OledUpdate();
//
//        if (ADRead < 450) {
//            OledDrawString("\nIs it tapped?\n");
//            OledUpdate();
//        } else if (ADRead > 450) {
//            OledDrawString("\n\nTAP");
//            OledUpdate();
//        }
//        for (i = 0; i < 10000; i++);
//    }
//}
