/*
 * File:   Part1Main.c
 * Author: xzhou45
 *
 * Created on January 31, 2020, 11:33 AM
 */


#include "xc.h"
#include <BOARD.h>
#include <Ascii.h>
#include <Oled.h>
#include <OledDriver.h>
#include <QEI.h>
#include <stdio.h>
#include <pwm.h>

int main(void) {
    BOARD_Init();
    QEI_Init();
    OledInit();
    PWM_Init();
    PWM_AddPins(PWM_PORTY12); // Pin 5 R
    PWM_AddPins(PWM_PORTY10); // Pin 6 G
    PWM_AddPins(PWM_PORTY04); // Pin 9 B
    char test[100];
    int counter = 0;
    int R;
    int G;
    int B;
    while (1) {
        // Counter resets every 96 ticks (1 turn gives 100)
        // 96 / 6 is 16
        counter = QEI_GetPosition();
        if (counter >= 0 && counter < 16) {
            R = 0;
            G = 1000;
            B = counter * 62;
        } else if (counter > 16 && counter < 32) {
            R = 0;
            G = 1000 - ((counter - 16) * 62);
            B = 1000;
        } else if (counter > 32 && counter < 48) {
            R = 0 + ((counter - 32)*62);
            G = 0;
            B = 1000;
        } else if (counter > 48 && counter < 64) {
            R = 1000;
            G = 0;
            B = 1000 - ((counter - 48) * 62);
        } else if (counter > 64 && counter < 80) {
            R = 1000;
            G = (counter - 64) * 62;
            B = 0;
        } else if (counter > 80 && counter <= 96) {
            R = 1000 - ((counter - 80) * 62);
            G = 1000;
            B = 0;
        }

        PWM_SetDutyCycle(PWM_PORTY12, R);
        PWM_SetDutyCycle(PWM_PORTY10, G);
        PWM_SetDutyCycle(PWM_PORTY04, B);



        sprintf(test, "%6d", counter);
        OledDrawString(test);
        OledUpdate();


    }
}
