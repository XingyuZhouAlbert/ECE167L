/*
 * File:   CAPTOUCH_Main.c
 * Author: xzhou45
 *
 * Created on February 7, 2020, 12:29 PM
 */


#include "xc.h"
#include <BOARD.h>
#include <CAPTOUCH.h>
#include <timers.h>
#include <Oled.h>
#include <OledDriver.h>
#include <AD.h>
#include <Ascii.h>
#include <stdio.h>
int i;

int main(void) {
    BOARD_Init();
    CAPTOUCH_Init();
    OledInit();
    while (1) {
        OledClear(0);
        if (CAPTOUCH_IsTouched()) {
            OledDrawString("\nTOUCHED!!!!!");
            OledUpdate();
        } else {
            OledDrawString("NOT TOUCHED!!!!\n");
            OledUpdate();
        }
//        for (i = 0; i < 10000; i++);
    }
}
