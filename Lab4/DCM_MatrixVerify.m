close all
clear all
clc

syms theta psi phi

theta = 10;
psi = 5;
phi = 15;

a11 = cos(theta)*cos(psi);
a12 = cos(theta)* sin(psi);
a13 = -sin(theta);


a21 = (sin(phi)*sin(theta)*cos(psi)) - (cos(phi)*sin(psi));
a22 = (sin(phi)*sin(theta)*sin(psi)) + (cos(phi)*cos(psi));
a23 = sin(phi)*cos(theta);

a31 = (cos(phi)*sin(theta)*cos(psi)) + (sin(phi)*sin(psi));
a32 = (cos(phi)*sin(theta)*sin(psi)) - (sin(phi)*cos(psi));
a33 = cos(phi)*cos(theta);

matrix = [a11,a12,a13;a21,a22,a23;a31,a32,a33]
matrixT = transpose(matrix)
matrixV = inv(matrix)

