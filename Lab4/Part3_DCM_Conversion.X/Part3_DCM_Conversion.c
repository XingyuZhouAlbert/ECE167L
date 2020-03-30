/*
 * File:   Part3_DCM_Conversion.c
 * Author: xzhou45
 *
 * Created on March 3, 2020, 4:09 PM
 */


#include "xc.h"
#include <BOARD.h>
#include <Oled.h>
#include <OledDriver.h>
#include <MPU9250.h>
#include <Ascii.h>
#include <math.h>
#include <stdio.h>
#include <I2C.h>
#include <MatrixMath.h>




#define SCALE 131
int xgyro;
int ygyro;
int zgyro;
int i = 0;
int SumP = 0;
int SumQ = 0;
int SumR = 0;
float biasP = 0;
float biasQ = 0;
float biasR = 0;
float scaledP = 0;
float scaledQ = 0;
float scaledR = 0;
unsigned int timer = 0;
unsigned int timer1 = 0;
float pitch = 0;
float yaw = 0;
float roll = 0;

void Print1DArray(float mat[3]) {
    for (i = 0; i < 3; i++) {
        printf(" |%0.3f| ", (double) mat[i]);
    }
    printf("\n");
}

float mySinc(float x) {
    if (x == 0) {
        return 1;
    } else if (fabsf(x) < 1) { //get rid of the error when no rotation happens
        return 1 - (x * x) / 6 + (x * x * x * x) / 120 - (x * x * x * x * x * x) / 5040;
    } else {
        return sin(x) / x;
    }
}

void getEulerAngle(float mat[3][3]) {
    // Extract
    pitch = asin(-mat[0][2])*1.1;
    roll = atan2((mat[1][2]), (mat[2][2]));
    yaw = atan2((mat[0][1]), (mat[0][0]));
    // Convert
    pitch = (pitch * 180 / M_PI);
    roll = (roll * 180 / M_PI);
    yaw = (yaw * 180 / M_PI);
    // Display on OLED
    char Angle[1000];
    sprintf(Angle, "pitch:%.2f\nroll:%.2f\nyaw:%.2f\n", pitch, roll, yaw);
    OledDrawString(Angle);
    OledUpdate();
    OledClear(OLED_COLOR_BLACK);
}

/**
 * function R_exp = Rexp(w) returns the exponential Rodrigues parameter form of the integration that 
 * keeps R on SO(3).
 */
void Rexp(float mat[3], float R_exp[3][3]) {
    float wnorm;
    float c;
    float s, p, q, r;
    p = mat[0];
    q = mat[1];
    r = mat[2];
    wnorm = sqrt((p * p)+(q * q)+(r * r));
    c = cos(wnorm * 0.5);
    s = mySinc(wnorm * 0.5);
    float rcross[3][3];
    rcross[0][0] = 0;
    rcross[0][1] = -r;
    rcross[0][2] = q;
    rcross[1][0] = r;
    rcross[1][1] = 0;
    rcross[1][2] = -p;
    rcross[2][0] = -q;
    rcross[2][1] = p;
    rcross[2][2] = 0;

    float mat1[3][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };

    float mat2[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    float mat3[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    float mat4[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    float mat5[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };

    MatrixScalarMultiply((s * c), rcross, mat2);
    MatrixScalarMultiply((s * s / 2), rcross, mat3);
    MatrixMultiply(mat3, rcross, mat4);
    MatrixAdd(mat4, mat2, mat5);
    MatrixAdd(mat5, mat1, R_exp);
}

/**
 * Function checks the orthonormality of a matrix passed to it and returns a
 * value that is the Frobinius norm of the residual. Smaller numbers
 * indicate better orthonormality. 
 */
int CheckOrthonormality(float mat[3][3]) {
    float invresult [3][3];
    float tranresult [3][3];
    MatrixInverse(mat, invresult);
    MatrixTranspose(mat, tranresult);
    if (MatrixEquals(invresult, tranresult) == 1) {
        return 1;
    } else {
        return 0;
    }
}

// Open loop integration using forward integration

void IntegrateOpenLoopFWD(float Rminus[3][3], float PQR[3], float Rplus[3][3]) {
    float gyro[3] = {0, 0, 0};
    float p, q, r;
    float result1 [3][3] = {
        {0}
    };
    gyro[0] = PQR[0] * (1 / 50);
    gyro[1] = PQR[1] * (1 / 50);
    gyro[2] = PQR[2] * (1 / 50);

    p = gyro[0];
    q = gyro[1];
    r = gyro[2];
    float rcross[3][3];
    rcross[0][0] = 0;
    rcross[0][1] = -r;
    rcross[0][2] = q;
    rcross[1][0] = r;
    rcross[1][1] = 0;
    rcross[1][2] = -p;
    rcross[2][0] = -q;
    rcross[2][1] = p;
    rcross[2][2] = 0;

    MatrixMultiply(rcross, Rminus, result1);
    MatrixAdd(Rminus, result1, Rplus);
}

// Open loop integration using matrix EXP

void IntegrateOpenLoopEXP(float Rminus[3][3], float PQR[3], float Rplus[3][3]) {
    float gyro[3] = {0, 0, 0};
    float R_exp[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    float R_temp[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    gyro[0] = PQR[0] * (0.02);
    gyro[1] = PQR[1] * (0.02);
    gyro[2] = PQR[2] * (0.02);
    Rexp(gyro, R_exp);
    MatrixMultiply(Rminus, R_exp, R_temp);
    MatrixCopy(R_temp, Rplus);
}

void r_cross(float mat1[3], float result[3][3]) {
    result[0][0] = 0;
    result[0][1] = -mat1[2];
    result[0][2] = mat1[1];
    result[1][0] = mat1[2];
    result[1][1] = 0;
    result[1][2] = -mat1[0];
    result[2][0] = -mat1[1];
    result[2][1] = mat1[0];
    result[2][2] = 0;
}

int main(void) {
    BOARD_Init();
    MPU9250_Init();
    TIMERS_Init();
    OledInit();
    I2C_Init(I2C_DEFUALT_RATE);
    printf("\n\rHelloWorld\n\r");
    xgyro = 0;
    ygyro = 0;
    zgyro = 0;
    float gyroInput[3] = {0, 0, 0};
    float R0 [3][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };
    float R_plus[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };

    // delay for 10s.
    while (TIMERS_GetMilliSeconds()<(timer1 + 10000));
    // Read 10s of data and compute the bias for each axis
    while (TIMERS_GetMilliSeconds()<(timer1 + 20000)) {
        SumP += MPU9250_ReadGyroX();
        SumQ += MPU9250_ReadGyroY();
        SumR += MPU9250_ReadGyroZ();
        i++;
    }
    // get the bias
    biasP = SumP / i;
    biasQ = SumQ / i;
    biasR = SumR / i;
    int z = 0;
    while (1) {
        while (TIMERS_GetMilliSeconds()<(timer + 20));
        timer = TIMERS_GetMilliSeconds();
        xgyro = MPU9250_ReadGyroX();
        ygyro = MPU9250_ReadGyroY();
        zgyro = MPU9250_ReadGyroZ();

        //scale my data
        scaledP = (xgyro - biasP) / SCALE;
        scaledQ = (ygyro - biasQ) / SCALE;
        scaledR = (zgyro - biasR) / SCALE;
        //convert the reading back to radian
        gyroInput[0] = scaledP * M_PI / 180;
        gyroInput[1] = scaledQ * M_PI / 180;
        gyroInput[2] = scaledR * M_PI / 180;
        if (z == 0) {
            IntegrateOpenLoopEXP(R0, gyroInput, R_plus);
            getEulerAngle(R_plus);
            z++;
        } else {
            IntegrateOpenLoopEXP(R_plus, gyroInput, R_plus);
            getEulerAngle(R_plus);
        }
    }
}

