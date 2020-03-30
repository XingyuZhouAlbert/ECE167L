close all
clear all
clc



deltaT = 1/50
Rminus = [1 0 0;0 1 0;0 0 1];
Bminus = [1;2;3];

gyro = [-0.0410;-0.0873;-0.0025];
magIner = [22.770;5.329;41.5102];
AccIner = [0;0;1];

mag = [0.9134;0.4060;-0.0014];
accel = [0.9266;0.0493;-0.3354];


[Rplus, Bplus] = IntegrateClosedLoop(Rminus, Bminus, gyro, mag, accel, magIner, AccIner, deltaT)