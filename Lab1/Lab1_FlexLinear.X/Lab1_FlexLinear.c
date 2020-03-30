/*
 * File:   Lab1_FlexLinear.c
 * Author: xzhou45
 *
 * Created on January 15, 2020, 11:38 AM
 */


#include "xc.h"
#include <Oled.h>
#include <OledDriver.h>
#include <AD.h>
#include <Ascii.h>
#include <BOARD.h>
#include <stdio.h>



int main(void){
    BOARD_Init();
    int value = 0;
    OledInit();
    AD_Init();
    AD_AddPins(AD_A3);
    char result[100];
    while(1){
        OledUpdate();
        value = AD_ReadADPin(AD_A3);
        sprintf(result, "%4d", value);
        OledDrawString(result);
    }
    
}