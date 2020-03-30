/*
 * File:   MPUMain.c
 * Author: xzhou45
 *
 * Created on February 13, 2020, 4:30 PM
 */


#include "xc.h"
#include "Oled.h"
#include <BOARD.h>
#include <MPU9250.h>
#include <stdio.h>
#include <I2C.h>
#include <timers.h>

int xaccl;
int yaccl;
int zaccl;
int xgyro;
int ygyro;
int zgyro;
int xmag;
int ymag;
int zmag;
int i = 0;
unsigned int timer = 0;
unsigned int timer1 = 0;

int main(void) {
    BOARD_Init();
    MPU9250_Init();
    TIMERS_Init();
    OledInit();
    I2C_Init(I2C_DEFUALT_RATE);
    xaccl = 0;
    yaccl = 0;
    zaccl = 0;
    xgyro = 0;
    ygyro = 0;
    zgyro = 0;
    xmag = 0;
    ymag = 0;
    zmag = 0;
    char data [1000];
    //delay for 5s.
//    while (TIMERS_GetMilliSeconds()<(timer1 + 5000));
    while (1) {
        while (TIMERS_GetMilliSeconds()<(timer + 20));
        timer = TIMERS_GetMilliSeconds();
        xaccl = MPU9250_ReadAccelX();
        yaccl = MPU9250_ReadAccelY();
        zaccl = MPU9250_ReadAccelZ();
        xgyro = MPU9250_ReadGyroX();
        ygyro = MPU9250_ReadGyroY();
        zgyro = MPU9250_ReadGyroZ();
        xmag = MPU9250_ReadMagX();
        ymag = MPU9250_ReadMagY();
        zmag = MPU9250_ReadMagZ();
        sprintf(data,"Xmag:%10d\nYmag:%10d\nZmag:%10d",xmag,ymag,zmag);
        OledDrawString(data);
        OledUpdate();
        printf("\r\n%d, %d, %d", xmag, ymag, zmag);
//        if (TIMERS_GetMilliSeconds()>(timer1 + 15000)) {
//            break;
//        }

    }
}
