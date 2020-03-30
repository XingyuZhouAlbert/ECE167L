/*
 * File:   MPUDataRecord.c
 * Author: xzhou45
 *
 * Created on February 20, 2020, 12:57 PM
 */


#include "xc.h"
#include <BOARD.h>
#include <MPU9250.h>
#include <stdio.h>
#include <I2C.h>
#include <timers.h>
int i = 0;
unsigned int timer = 0;
int xgyro = 0;
int main(void) {
    BOARD_Init();
    MPU9250_Init();
    TIMERS_Init();
    I2C_Init(I2C_DEFUALT_RATE);
    while(1){
        xgyro = MPU9250_ReadGyroX();
        printf("\r\n%d",xgyro);
    }
}
