clear all
close all
clc

%% zaccl calibration
Zdown = csvread('zaccl_facedown.csv');
Zup = csvread('zaccl_faceup.csv');

avgZdown = mean(Zdown)
avgZup = mean(Zup)

% the scale that i need to make them euqal to -1 or 1
zupscale = 1/avgZup
zdwnscale = 1/avgZdown

zscale_accl_total = (zupscale+zdwnscale)/2;

offsetz_accl = (avgZup + avgZdown)/2

for i = 1:200
    calibratedZup(i) = Zup(i) .* zupscale;
end

for j = 1:200
    calibratedZdwn(j) = Zdown(j) .* abs(zdwnscale);
end
% figure(1)
% scatter(1:length(Zup),calibratedZup);
% 
% figure(2)
% scatter(1:length(Zdown),calibratedZdwn);

figure (1)
scatter(1:length(Zup),calibratedZup);
hold on
scatter(1:length(Zdown),calibratedZdwn);
title('calibrated Z axis data');
legend('Zup', 'Zdown');
%% xaccel calibration
Xdown = csvread('xaccl_low.csv');
Xup = csvread('xaccl_high.csv');

avgXdown = mean(Xdown)
avgXup = mean(Xup)

% the scale that i need to make them euqal to -1 or 1
xupscale = 1/avgXup
xdwnscale = 1/avgXdown

xscale_accl_total = (xupscale+xdwnscale)/2;

offsetx_accl = (avgXup + avgXdown)/2

for i = 1:200
    calibratedXup(i) = Xup(i) .* abs(xupscale);
end

for j = 1:200
    calibratedXdwn(j) = Xdown(j) .* abs(xdwnscale);
end

% figure(3)
% scatter(1:length(Xup),calibratedXup);
% title('X axis put on high');
% figure(4)
% scatter(1:length(Xdown),calibratedXdwn);
% title('X axis put on low');

figure (2)
scatter(1:length(Xup),calibratedXup);
hold on
scatter(1:length(Xdown),calibratedXdwn);
title('calibrated X axis data');
legend('Xup', 'Xdown');



%% yaccel calibration
Yleft = csvread('yaccl_left.csv');
Yright = csvread('yaccl_right.csv');

avgYleft = mean(Yleft)
avgYright = mean(Yright)

% the scale that i need to make them euqal to -1 or 1
yupscale = 1/avgYleft
ydwnscale = 1/avgYright

yscale_accl_total = (yupscale+ydwnscale)/2;

offsety_accl = (avgYleft + avgYright)/2

for i = 1:200
    calibratedYleft(i) = Yleft(i) .* abs(yupscale);
end

for j = 1:200
    calibratedYright(j) = Yright(j) .* abs(ydwnscale);
end

% figure(5)
% scatter(1:length(Yleft),calibratedYleft);
% title('Y axis face left');
% figure(6)
% scatter(1:length(Yright),calibratedYright);
% title('Y axis face right');
figure (3)
scatter(1:length(Yleft),calibratedYleft);
hold on
scatter(1:length(Yright),calibratedYright);
title('calibrated Y axis data');
legend('Yleft', 'Yright');





%% MAG CALIBRATION
magdata = csvread('magdata.csv');
figure(7)
axis equal
scatter3(magdata(:,1),magdata(:,2),magdata(:,3));
title('Raw Mag data drawing');
% Take measurements from the raw data to get Xmax, Ymax, Zmax
Ymax = [385, 383, 376, 379,371]
Ymin = [-104,-95,-104,-106,-91]
Xmax = [392, 395, 389, 387,382]
Xmin = [-127,-124,-135,-126,-137]
Zmax = [28, 27, 30, 24, 23]
Zmin = [-533,-538,-528,-535,-536]


avgYmax = mean(Ymax);
avgYmin = mean(Ymin);
avgXmax = mean(Xmax);
avgXmin = mean(Xmin);
avgZmax = mean(Zmax);
avgZmin = mean(Zmin);

offsetY_mag = (avgYmax+avgYmin) / 2;
offsetX_mag = (avgXmax+avgXmin) / 2;
offsetZ_mag = (avgZmax+avgZmin) / 2;

scaleY_mag = 2/(avgYmax-avgYmin);
scaleX_mag = 2/(avgXmax-avgXmin);
scaleZ_mag = 2/(avgZmax-avgZmin);

for i = 1 : length(magdata(:,1))
    calibratedX(i) = (magdata(i,1)- offsetX_mag) / scaleX_mag;
end

for i = 1 : length(magdata(:,1))
    calibratedY(i) = (magdata(i,2)- offsetY_mag) / scaleY_mag;
end

for i = 1 : length(magdata(:,1))
    calibratedZ(i) = (magdata(i,3)- offsetZ_mag) / scaleZ_mag;
end

figure(8)
axis equal
scatter3(calibratedX(1,:),calibratedY(1,:),calibratedZ(1,:));
title('Calibrated Mag data drawing');

