/*
 * File:   PING_Main.c
 * Author: xzhou45
 *
 * Created on February 3, 2020, 5:51 PM
 */


#include "xc.h"
#include <BOARD.h>
#include <timers.h>
#include <AD.h>
#include <Oled.h>
#include <pwm.h>
#include <OledDriver.h>
#include <PING.h>
#include <stdio.h>
#include <ToneGeneration.h>

int main(void) {
    BOARD_Init();
    TIMERS_Init();
    PING_Init();
    OledInit();
    ToneGeneration_Init();
    char dist [100];
    char timetest [100];
    uint16_t distance;
    uint16_t time = 0;
    uint16_t newdistance;
    while (1) {
        distance = PING_GetDistance();
        time = PING_GetTimeofFlight();
        sprintf(timetest, "\ntime: %6d", (int) time);
        OledDrawString(timetest);
        OledUpdate();
        newdistance = PING_GetDistance();
        // discard the sudden high pitch in my reading
        // if the next reading is too high then just keep it to the previous value
        if (newdistance > 90) {
            newdistance = distance;
        }
        // this is just to keep the tone in an acceptable range
        if(newdistance > 90){
            newdistance = 90;
        }
        sprintf(dist, "Distance: %6d\n", (int) newdistance);
        OledDrawString(dist);
        OledUpdate();
        ToneGeneration_SetFrequency(newdistance * 10);
        ToneGeneration_ToneOn();
        // another delay to further improve the stability of the reading
        uint16_t delay = TIMERS_GetMilliSeconds();
        while (delay + 60 > TIMERS_GetMilliSeconds());
    }
}
