close all
clear all
clc

[Accel,Mag,Adist,Bdist] = CreateTumbleData(1000);

% Naive test data for accel
offsetAx = (max(Accel(:,1))+ (min(Accel(:,1))))/2;
offsetAy = (max(Accel(:,2))+ (min(Accel(:,2))))/2;
offsetAz = (max(Accel(:,3))+ (min(Accel(:,3))))/2;

scaleAx = max(Accel(:,1))- offsetAx;
scaleAy = max(Accel(:,2))- offsetAy;
scaleAz = max(Accel(:,3))- offsetAz;

% Naive test data for mag
offsetMx = (max(Mag(:,1)) + (min(Mag(:,1))))/2;
offsetMy = (max(Mag(:,2)) + (min(Mag(:,2))))/2;
offsetMz = (max(Mag(:,3)) + (min(Mag(:,3))))/2;

scaleMx = max(Mag(:,1)) - offsetMx;
scaleMy = max(Mag(:,2)) - offsetMy;
scaleMz = max(Mag(:,3)) - offsetMz;

for i = 1 : length(Accel(:,1))
    calibratedX(i) = (Accel(i,1)-offsetAx)/ scaleAx;
    calibratedY(i) = (Accel(i,2)-offsetAy)/ scaleAy;
    calibratedZ(i) = (Accel(i,3)-offsetAz)/ scaleAz;
end

for i = 1 : length(Mag(:,1))
    calibratedXM(i) = (Mag(i,1)-offsetMx)/ scaleMx;
    calibratedYM(i) = (Mag(i,2)-offsetMy)/ scaleMy;
    calibratedZM(i) = (Mag(i,3)-offsetMz)/ scaleMz;
end

figure(5)
plot3(Accel(:,1),Accel(:,2),Accel(:,3),'.');
hold on
plot3(calibratedX(1,:),calibratedY(1,:),calibratedZ(1,:),'.');
axis equal
title('Naive Pre VS Post Accel Calibration');
legend('Before', 'After');

figure(6)
plot3(Mag(:,1),Mag(:,2),Mag(:,3),'.');
hold on
plot3(calibratedXM(1,:),calibratedYM(1,:),calibratedZM(1,:),'.');
axis equal
title('Naive Pre VS Post Mag Calibration');
legend('Before', 'After');








[Atilde Btilde] = CalibrateEllipsoidData3D(calibratedX(1,:)',calibratedY(1,:)',calibratedZ(1,:)',10,1);
[Xc Yc Zc] = CorrectEllipsoidData3D(calibratedX(1,:),calibratedY(1,:),calibratedZ(1,:),Atilde,Btilde);

figure(7)
plot3(Accel(:,1),Accel(:,2),Accel(:,3),'.');
hold on
axis equal;
plot3(Xc(1,:),Yc(1,:),Zc(1,:),'.');
legend('Before', 'After');
title('Pre VS Post Calibrated Accel Tumble Test');




[mAtilde mBtilde] = CalibrateEllipsoidData3D(Mag(:,1),Mag(:,2),Mag(:,3),10,0);
[MXc MYc MZc] = CorrectEllipsoidData3D(Mag(:,1),Mag(:,2),Mag(:,3),mAtilde,mBtilde);

figure(8)
plot3(Mag(:,1),Mag(:,2),Mag(:,3),'.');
hold on
axis equal;
plot3(MXc(1,:),MYc(1,:),MZc(1,:),'.');
legend('Before', 'After');
title('Pre VS Post Calibrated Mag Tumble Test');

preAXmean = mean(Accel(:,1));
preAYmean = mean(Accel(:,2));
preAZmean = mean(Accel(:,3));
preAXd = std(Accel(:,1));
preAYd = std(Accel(:,2));
preAZd = std(Accel(:,3));

postAXmean = mean(Xc(1,:));
postAYmean = mean(Yc(1,:));
postAZmean = mean(Zc(1,:));
postAXd = std(Xc(1,:));
postAYd = std(Yc(1,:));
postAZd = std(Zc(1,:));

preMXmean = mean(Mag(:,1));
preMYmean = mean(Mag(:,2));
preMZmean = mean(Mag(:,3));
preMXd = std(Mag(:,1));
preMYd = std(Mag(:,2));
preMZd = std(Mag(:,3));

postMXmean = mean(MXc(1,:));
postMYmean = mean(MYc(1,:));
postMZmean = mean(MYc(1,:));
postMXd = std(MXc(1,:));
postMYd = std(MYc(1,:));
postMZd = std(MZc(1,:));






