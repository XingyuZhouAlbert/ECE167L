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
int xaccl, yaccl, zaccl, xmag, ymag, zmag;
int i = 0;
int SumP = 0;
int SumQ = 0;
int SumR = 0;
float biasP = 0;
float biasQ = 0;
float biasR = 0;
unsigned int timer = 0;
unsigned int timer1 = 0;
float pitch = 0;
float yaw = 0;
float roll = 0;

float Rplus[3][3] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
};
float Bplus[3] = {0, 0, 0};

void Print1DArray(float mat[3]) {
    for (i = 0; i < 3; i++) {
        printf(" |%0.3f| ", (double) mat[i]);
    }
    printf("\n");
}

void Copy1DMatrix(float mat1[3], float mat2[3]) {
    for (i = 0; i < 3; i++) {
        mat2[i] = mat1[i];
    }
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
    //    printf("pitch:%.2f\nroll:%.2f\nyaw:%.2f\n", pitch, roll, yaw);
    // Display on OLED
    char Angle[100];
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

void IntegrateClosedLoop(float Rminus[3][3], float Bminus[3], float gyros[3], float mags[3], float accels[3], float magInertial[3], float accelInertial[3], float Rplus[3][3], float Bplus[3]) {
    // declare some variables
    float bdot[3] = {0, 0, 0};
    float bdot1[3] = {0, 0, 0};
    float accels_temp[3] = {0, 0, 0};
    float mag_temp[3] = {0, 0, 0};
    float magInertial_temp[3] = {0, 0, 0};
    float accelInertial_temp[3] = {0, 0, 0};
    float biasedGyro[3] = {0, 0, 0};
    float gyroInputWithFeedback[3] = {0, 0, 0};
    float pqr[3] = {0, 0, 0};
    float wmeas_a[3] = {0, 0, 0};
    float wmeas_m[3] = {0, 0, 0};
    float wmeas_a1[3] = {0, 0, 0};
    float wmeas_m1[3] = {0, 0, 0};
    float wmeas_a2[3] = {0, 0, 0};
    float wmeas_m2[3] = {0, 0, 0};
    float accrcross[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    float Rminustrans[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    float magrcross[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    float feedbackgyro[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    float R_temp[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };

    float accelnorm, magnorm, accInorm, magInorm;
    float Kp_a = 40;
    float Ki_a = Kp_a * 0.1;
    float Kp_m = 20;
    float Ki_m = Kp_m * 0.1;

    accelnorm = sqrt(accels[0] * accels[0] + accels[1] * accels[1] + accels[2] * accels[2]);
    magnorm = sqrt(mags[0] * mags[0] + mags[1] * mags[1] + mags[2] * mags[2]);
    accInorm = sqrt(accelInertial[0] * accelInertial[0] + accelInertial[1] * accelInertial[1] + accelInertial[2] * accelInertial[2]);
    magInorm = sqrt(magInertial[0] * magInertial[0] + magInertial[1] * magInertial[1] + magInertial[2] * magInertial[2]);

    for (i = 0; i < 3; i++) {
        accels_temp[i] = accels[i] / accelnorm;
        mag_temp[i] = mags[i] / magnorm;
        magInertial_temp[i] = magInertial[i] / magInorm;
        accelInertial_temp[i] = accelInertial[i] / accInorm;
        biasedGyro[i] = gyros[i] - Bminus[i];
    }

    r_cross(accels_temp, accrcross);
    MatrixTranspose(Rminus, Rminustrans);
    Matrix3by1(Rminustrans, accelInertial_temp, wmeas_a1);
    Matrix3by1(accrcross, wmeas_a1, wmeas_a2);
    Copy1DMatrix(wmeas_a2, wmeas_a);

    r_cross(mag_temp, magrcross);
    Matrix3by1(Rminustrans, magInertial_temp, wmeas_m1);
    Matrix3by1(magrcross, wmeas_m1, wmeas_m2);
    Copy1DMatrix(wmeas_m2, wmeas_m);

    for (i = 0; i < 3; i++) {
        gyroInputWithFeedback[i] = biasedGyro[i] + (Kp_a * wmeas_a[i]) + (Kp_m * wmeas_m[i]);
        bdot[i] = (-Ki_a * wmeas_a[i]) - (Ki_m * wmeas_m[i]);
        pqr[i] = gyroInputWithFeedback[i] * 0.02;
        bdot1[i] = bdot[i] * 0.02;
    }
    Rexp(pqr, feedbackgyro);
    MatrixMultiply(Rminus, feedbackgyro, R_temp);
    MatrixCopy(R_temp, Rplus);
    for (i = 0; i < 3; i++) {
        Bplus[i] = Bminus[i] + bdot1[i];
    }
}

void DCMfromTraid(float mags[3], float accels[3], float magInertial[3], float accelInertial[3], float eDCM[3][3]) {
    float accelnorm, magnorm, accInorm, magInorm, mnorm, Mnorm;
    float accels_temp[3] = {0, 0, 0};
    float mag_temp[3] = {0, 0, 0};
    float magInertial_temp[3] = {0, 0, 0};
    float accelInertial_temp[3] = {0, 0, 0};
    float m_temp[3] = {0, 0, 0};
    float m[3] = {0, 0, 0};
    float M_temp[3] = {0, 0, 0};
    float M[3] = {0, 0, 0};
    float MMcross[3] = {0, 0, 0};
    float mmcross[3] = {0, 0, 0};
    float magCross[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    float magInertialCross[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };

    float A1[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    float A2[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    float A3[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    float A4[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    float A[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };


    accelnorm = sqrt(accels[0] * accels[0] + accels[1] * accels[1] + accels[2] * accels[2]);
    magnorm = sqrt(mags[0] * mags[0] + mags[1] * mags[1] + mags[2] * mags[2]);
    accInorm = sqrt(accelInertial[0] * accelInertial[0] + accelInertial[1] * accelInertial[1] + accelInertial[2] * accelInertial[2]);
    magInorm = sqrt(magInertial[0] * magInertial[0] + magInertial[1] * magInertial[1] + magInertial[2] * magInertial[2]);
    for (i = 0; i < 3; i++) {
        accels_temp[i] = accels[i] / accelnorm;
        mag_temp[i] = mags[i] / magnorm;
        magInertial_temp[i] = magInertial[i] / magInorm;
        accelInertial_temp[i] = accelInertial[i] / accInorm;
    }
    Copy1DMatrix(mag_temp, mags);
    Copy1DMatrix(accels_temp, accels);
    Copy1DMatrix(magInertial_temp, magInertial);
    Copy1DMatrix(accelInertial_temp, accelInertial);


    r_cross(mags, magCross);
    Matrix3by1(magCross, accels, m_temp);
    mnorm = sqrt(m_temp[0] * m_temp[0] + m_temp[1] * m_temp[1] + m_temp[2] * m_temp[2]);

    r_cross(magInertial, magInertialCross);
    Matrix3by1(magInertialCross, accelInertial, M_temp);
    Mnorm = sqrt(M_temp[0] * M_temp[0] + M_temp[1] * M_temp[1] + M_temp[2] * M_temp[2]);
    for (i = 0; i < 3; i++) {
        m[i] = m_temp[i] / mnorm;
        M[i] = M_temp[i] / Mnorm;
    };

    Matrix3by1(magInertialCross, M, MMcross);
    Matrix3by1(magCross, m, mmcross);
    A1[0][0] = magInertial[0];
    A1[0][1] = M[0];
    A1[0][2] = MMcross[0];
    A1[1][0] = magInertial[1];
    A1[1][1] = M[1];
    A1[1][2] = MMcross[1];
    A1[2][0] = magInertial[2];
    A1[2][1] = M[2];
    A1[2][2] = MMcross[2];

    A2[0][0] = mags[0];
    A2[0][1] = m[0];
    A2[0][2] = mmcross[0];
    A2[1][0] = mags[1];
    A2[1][1] = m[1];
    A2[1][2] = mmcross[1];
    A2[2][0] = mags[2];
    A2[2][1] = m[2];
    A2[2][2] = mmcross[2];

    MatrixTranspose(A2,A4);
    MatrixMultiply(A1, A4, A3);
    MatrixCopy(A3, A);
    MatrixTranspose(A, eDCM);

}

int main(void) {
    BOARD_Init();
    MPU9250_Init();
    TIMERS_Init();
    OledInit();
    xaccl = 0;
    yaccl = 0;
    zaccl = 0;
    xmag = 0;
    ymag = 0;
    zmag = 0;
    float mInertial[3] = {22825.7 * 0.001, 5393.9 * 0.001, 41690.9 * 0.001};
    float accInertial[3] = {0, 0, -1};
    float accels[3] = {0, 0, 0};
    float mags[3] = {0, 0, 0};
    float correctaccels[3] = {0, 0, 0};
    float correctmags[3] = {0, 0, 0};
    float Atilde[3][3] = {
        {5.74049155546029e-05, -4.75362391940228e-07, 2.77568877586301e-07},
        {-4.84997646829463e-07, 5.75925379638571e-05, -1.30519109144118e-06},
        {3.02137309248433e-08, -4.40750202817198e-07, 5.46561524607610e-05}
    };
    float Btilde[3] = {-0.0437927497335771, -0.0176776806461205, 0.0324664233327244};
    float AMtilde[3][3] = {
        {0.00488708502559458, 0.000136773537330645, -4.37601058178839e-06},
        {0.000154922534381618, 0.00487579459447044, -9.61571890602184e-07},
        {-0.000284581064184032, 0.000121176152113117, 0.00446089195821704}
    };
    float BMtilde[3] = {-0.640796348104602, -0.586925736122492, 1.04455958137841};
    float correct1[3] = {0, 0, 0};
    float correct2[3] = {0, 0, 0};
    float newmag[3] = {0, 0, 0};
    float correct1_temp[3] = {0, 0, 0};
    float correct2_temp[3] = {0, 0, 0};
    float newmag_temp[3] = {0, 0, 0};
    float Rmis[3][3] = {
        {-0.473095701358529, 0.393962508636576, 0.788019034760492},
        {0.880926572469336, 0.199148635550817, 0.429311302990819},
        {0.0121996423516768, 0.897292239321741, -0.441268405825148}
    };
    float RmisTrans[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    float eDCM[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };

    // delay for 10s.
    printf("\n\rstart initializing\n\r");
    while (TIMERS_GetMilliSeconds()<(timer1 + 10000));
    MatrixTranspose(Rmis, RmisTrans);
    int z = 0;
    while (1) {
        while (TIMERS_GetMilliSeconds()<(timer + 20));
        timer = TIMERS_GetMilliSeconds();
        xaccl = MPU9250_ReadAccelX();
        yaccl = MPU9250_ReadAccelY();
        zaccl = MPU9250_ReadAccelZ();
        xmag = MPU9250_ReadMagX();
        ymag = MPU9250_ReadMagY();
        zmag = MPU9250_ReadMagZ();
        accels[0] = xaccl;
        accels[1] = yaccl;
        accels[2] = zaccl;
        mags[0] = xmag;
        mags[1] = ymag;
        mags[2] = zmag;
        // calibrate accel and mags reading
        Matrix3by1(Atilde, accels, correct1_temp);
        Matrix3by1(AMtilde, mags, correct2_temp);
        Copy1DMatrix(correct1_temp, correct1);
        Copy1DMatrix(correct2_temp, correct2);
        Matrix3by1(RmisTrans, correct2, newmag_temp);
        Copy1DMatrix(newmag_temp, newmag);
        for (i = 0; i < 3; i++) {
            correctaccels[i] = Btilde[i] + correct1[i];
            correctmags[i] = BMtilde[i] + correct2[i];
        }
        DCMfromTraid(correctmags, correctaccels, mInertial, accInertial, eDCM);
        getEulerAngle(eDCM);
    }
}
