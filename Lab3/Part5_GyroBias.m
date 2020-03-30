clear all
close all
clc

GYRO_1HR = csvread('GYRO_1HR.csv');
GYRO_PRE = csvread('GYRO_DATA.csv');
sumx = 0;
sumy = 0;
sumz = 0;

xbias = mean(GYRO_PRE(:,1));
ybias = mean(GYRO_PRE(:,2));
zbias = mean(GYRO_PRE(:,3));

anglerateX = (GYRO_1HR(:,1)-xbias)/131;
anglerateY = (GYRO_1HR(:,2)-ybias)/131;
anglerateZ = (GYRO_1HR(:,3)-zbias)/131;

for i = 1:length(GYRO_1HR)
    sumx = sumx + anglerateX(i)*0.01;
    sumy = sumy + anglerateY(i)*0.01;
    sumz = sumz + anglerateZ(i)*0.01;
    anglexdr(i) = sumx;
    angleydr(i) = sumy;
    anglezdr(i) = sumz;
end

t = 1: 1: length(GYRO_1HR);
t = t.*(0.02/60);

figure (1)
plot(t, anglexdr);
hold on;
plot(t, angleydr);
plot(t, anglezdr);
xlabel('minutes');
ylabel('degree/s');
legend('drift x','drift y', 'drift z');
title('Angle Drift rate of each axis');

figure (2)
% plot the data point for every minute
plot(1:3000:length(GYRO_1HR), anglerateX(1:3000:length(GYRO_1HR)));
hold on;
plot(1:3000:length(GYRO_1HR), anglerateY(1:3000:length(GYRO_1HR)));
plot(1:3000:length(GYRO_1HR), anglerateZ(1:3000:length(GYRO_1HR)));
title('Angle Rate of each axis')

