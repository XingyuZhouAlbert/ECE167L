/*
 * File:   Tuneout_Part3.c
 * Author: xzhou45
 *
 * Created on January 8, 2020, 11:47 AM
 */


#include "xc.h"
#include <BOARD.h>
#include <pwm.h>
#include <ToneGeneration.h>

int main(void) {
    BOARD_Init();
    ToneGeneration_Init();
    ToneGeneration_SetFrequency(TONE_440);
    ToneGeneration_ToneOn();
    while(1);

}
