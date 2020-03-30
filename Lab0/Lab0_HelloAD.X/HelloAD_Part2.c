/*
 * File:   HelloAD_Part2.c
 * Author: xzhou
 *
 * Created on January 7, 2020, 10:17 PM
 */


#include "xc.h"
#include <BOARD.h> 
#include <Oled.h>
#include <OledDriver.h>
#include <AD.h>
#include <Ascii.h>
#include <stdio.h>


uint16_t value;

int main(void) {
    BOARD_Init();
    OledInit();
    AD_Init();
    // Activate Pin_A0 since it's associated with the pot
    AD_AddPins(AD_A0);
    // Adding a char buffer since draw string can only draw string
    char Result[100];
    while (1) {
        OledDrawString("Potentiometer Value:\n");
        OledUpdate();
        value = AD_ReadADPin(AD_A0);
        // store the value into my char buffer
        sprintf(Result, "\n%4d", value);
        OledDrawString(Result);
        OledUpdate();
    }
    while(1);
}
