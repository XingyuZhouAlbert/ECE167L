

tumbleaccel = csvread('part8_tumble_accel_3min.csv');
tumblemag = csvread('part8_tumble_mag_2min.csv');


[Atilde Btilde] = CalibrateEllipsoidData3D(tumbleaccel(:,1),tumbleaccel(:,2),tumbleaccel(:,3),10,0);
[Xc Yc Zc] = CorrectEllipsoidData3D(tumbleaccel(:,1),tumbleaccel(:,2),tumbleaccel(:,3),Atilde,Btilde);



accelX = tumbleaccel(:,1);
accelY = tumbleaccel(:,2);
accelZ = tumbleaccel(:,3);

figure (1)
plot3(accelX(:,1),accelY(:,1),accelZ(:,1),'.');
title('Raw accel data from sensor');

calibratedAX = (accelX(:,1) - offsetx_accl)*xscale_accl_total;
calibratedAY = (accelY(:,1) - offsety_accl)*yscale_accl_total;
calibratedAZ = (accelZ(:,1) - offsetz_accl)*zscale_accl_total;

figure (3)
subplot(3,1,1)
plot(1:length(calibratedAX(:,1)), calibratedAX(:,1));
xlabel('manual calibrated X data using part4');
subplot(3,1,2)
plot(1:length(Xc(1,:)), Xc(1,:));
xlabel('post calibrated X data using the provided file');
subplot(3,1,3)
plot(1:length(accelX(:,1)), accelX(:,1));
xlabel('raw X data from the sensor');

figure (4)
subplot(3,1,1)
plot(1:length(calibratedAY(:,1)), calibratedAY(:,1));
xlabel('manual calibrated Y data using part4');
subplot(3,1,2)
plot(1:length(Yc(1,:)), Yc(1,:));
xlabel('post calibrated Y data using the provided file');
subplot(3,1,3)
plot(1:length(accelY(:,1)), accelY(:,1));
xlabel('raw Y data from the sensor');

figure (5)
subplot(3,1,1)
plot(1:length(calibratedAZ(:,1)), calibratedAZ(:,1));
xlabel('manual calibrated Z data using part4');
subplot(3,1,2)
plot(1:length(Zc(1,:)), Zc(1,:));
xlabel('post calibrated Z data using the provided file');
subplot(3,1,3)
plot(1:length(accelZ(:,1)), accelZ(:,1));
xlabel('raw Z data from the sensor');




magX = tumblemag(:,1);
magY = tumblemag(:,2);
magZ = tumblemag(:,3);

figure (6)
plot3(magX(:,1),magY(:,1),magZ(:,1),'.');
title('Raw mag data from sensor');
[AM BM] = CalibrateEllipsoidData3D(tumblemag(:,1),tumblemag(:,2),tumblemag(:,3),10,0);
[XMc YMc ZMc] = CorrectEllipsoidData3D(tumblemag(:,1),tumblemag(:,2),tumblemag(:,3),AM,BM);

calibratedMX = (magX(:,1) - offsetX_mag)*scaleX_mag;
calibratedMY = (magY(:,1) - offsetY_mag)*scaleY_mag;
calibratedMZ = (magZ(:,1) - offsetZ_mag)*scaleZ_mag;


figure (7)
subplot(3,1,1)
plot(1:length(calibratedMX(:,1)), calibratedMX(:,1));
xlabel('manual calibrated mag X data using part4');
subplot(3,1,2)
plot(1:length(XMc(1,:)), XMc(1,:));
xlabel('post calibrated mag X data using the provided file');
subplot(3,1,3)
plot(1:length(magX(:,1)), magX(:,1));
xlabel('raw mag X data from the sensor');

figure (8)
subplot(3,1,1)
plot(1:length(calibratedMY(:,1)), calibratedMY(:,1));
xlabel('manual calibrated mag Y data using part4');
subplot(3,1,2)
plot(1:length(YMc(1,:)), YMc(1,:));
xlabel('post calibrated mag Y data using the provided file');
subplot(3,1,3)
plot(1:length(magY(:,1)), magY(:,1));
xlabel('raw mag Y data from the sensor');

figure (9)
subplot(3,1,1)
plot(1:length(calibratedMZ(:,1)), calibratedMZ(:,1));
xlabel('manual calibrated mag Z data using part4');
subplot(3,1,2)
plot(1:length(ZMc(1,:)), ZMc(1,:));
xlabel('post calibrated mag Z data using the provided file');
subplot(3,1,3)
plot(1:length(magZ(:,1)), magZ(:,1));
xlabel('raw mag Z data from the sensor');








