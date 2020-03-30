close all
clear all
clc


rawgyro = csvread('part6_rawgyro.csv');
xRotate = csvread('part6_gyroX90.csv');
yRotate = csvread('part6_gyroY90.csv');
zRotate = csvread('part6_gyroZ90.csv');

xbias = mean(rawgyro(:,1));
ybias = mean(rawgyro(:,2));
zbias = mean(rawgyro(:,3));

scaleX = nansum(abs(xRotate(:,1)-xbias))*0.02/400;
scaleY = nansum(abs(yRotate(:,2)-ybias))*0.02/1000;
scaleZ = nansum(abs(zRotate(:,3)-zbias))*0.02/500;
figure (1)
axis equal
plot(xRotate(:,1)/scaleX);
hold on

plot(yRotate(:,2)/scaleY);

plot(zRotate(:,3)/scaleZ);
xlabel('sample counts');
ylabel('turning degree');
title('Gyro turning angles on each axis');
legend('X axis', 'Y axis', 'Z axis');



