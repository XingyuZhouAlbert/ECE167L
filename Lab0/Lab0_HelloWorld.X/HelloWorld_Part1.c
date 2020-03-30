/*
 * File:   HelloWorld_Part1.c
 * Author: xzhou
 *
 * Created on January 7, 2020, 8:39 PM
 */


#include "xc.h"
#include <BOARD.h>
#include <stdio.h>

int main(void) {
    BOARD_Init();

    printf("Hello World!");
    while (1) {
        ;
    }
}
