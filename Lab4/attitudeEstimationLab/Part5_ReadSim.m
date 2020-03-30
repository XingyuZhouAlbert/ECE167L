clear;
clear clc;
close all;

GYRO_10S = csvread('Part5_BIAS.csv');
GYRO = csvread('Part5_GyroData.csv');

dT = 1/50;
R = eye(3);

biasP = mean(GYRO_10S(:,1));
biasQ = mean(GYRO_10S(:,2));
biasR = mean(GYRO_10S(:,3));


for i = 1 : length(GYRO)
    nwGyro(i,1) = GYRO(i,1) - biasP;
    nwGyro(i,2) = GYRO(i,2) - biasQ;
    nwGyro(i,3) = GYRO(i,3) - biasR;
end

for i = 1: length(nwGyro)
    p(i) = deg2rad(nwGyro(i,1)/131);
    q(i) = deg2rad(nwGyro(i,2)/131);
    r(i) = deg2rad(nwGyro(i,3)/131);
    gyroInput = [p(i);q(i);r(i)];
    if i == 1
        Rplus = IntegrateOpenLoop(R,gyroInput,dT);
    else
        Rplus = IntegrateOpenLoop(Rplus,gyroInput,dT);
    end
    
    NEul(i,1) = real((atan2(Rplus(1,2),Rplus(1,1)))*180/pi); %yaw
    NEul(i,2) = real((asin(-1*Rplus(1,3)))*180/pi);          %pitch
    NEul(i,3) = real((atan2(Rplus(2,3),Rplus(3,3)))*180/pi); %roll
end



