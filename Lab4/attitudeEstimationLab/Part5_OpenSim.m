%% Open loop integration without noise
clear;
clear clc;
close all;

dT = 1/50;
GscaleFactor = (1/250)*(2^15-1); 
[Acc,Mag,wGyro,Eul] = CreateTrajectoryData(dT,false);

yaw = Eul(1,1);
pitch = Eul(1,2);
roll = Eul(1,3);

R = [cosd(pitch)*cosd(yaw) cosd(pitch)*sind(yaw) -1*sind(pitch), 
    (sind(roll)*sind(pitch)*cosd(yaw))-(cosd(roll)*sind(yaw)) (sind(roll)*sind(pitch)*sind(yaw))+(cosd(roll)*cosd(yaw)) sind(roll)*cosd(pitch),
    (cosd(roll)*sind(pitch)*cosd(yaw))+(sind(roll)*sind(yaw)) (cosd(roll)*sind(pitch)*sind(yaw))-(sind(roll)*cosd(yaw)) cosd(roll)*cosd(pitch)];

% [sX,sY,sZ]=sphere(30);
% surf(sX,sY,sZ,'FaceAlpha',.1,'EdgeColor','none');
% axis equal;
% xlabel('x')
% ylabel('y')
% zlabel('z')
% hold on
% 
% 
% quiver3([0,0,0],[0,0,0],[0,0,0],[R(1,1),R(1,2),R(1,3)],[R(2,1),R(2,2),R(2,3)],[R(3,1),R(3,2),R(3,3)],0);
% pause

for i = 1: length(wGyro)
    p(i) = deg2rad(wGyro(i,1)/GscaleFactor);
    q(i) = deg2rad(wGyro(i,2)/GscaleFactor);
    r(i) = deg2rad(wGyro(i,3)/GscaleFactor);
    gyroInput = [p(i);q(i);r(i)];
    if i == 1
        Rplus = IntegrateOpenLoop(R,gyroInput,dT);
    else
        Rplus = IntegrateOpenLoop(Rplus,gyroInput,dT);
    end
    
    NEul(i,1) = real((atan2(Rplus(1,2),Rplus(1,1)))*180/pi); %yaw
    NEul(i,2) = real((asin(-1*Rplus(1,3)))*180/pi);          %pitch
    NEul(i,3) = real((atan2(Rplus(2,3),Rplus(3,3)))*180/pi); %roll
    
%     quiver3([0,0,0],[0,0,0],[0,0,0],[Rplus(1,1),Rplus(1,2),Rplus(1,3)],[Rplus(2,1),Rplus(2,2),Rplus(2,3)],[Rplus(3,1),Rplus(3,2),Rplus(3,3)],0);
%     pause(0.001);
end

% hold off

% AnimateAttitude(dT,NEul)

figure(1)
subplot(3,1,1);
histfit(NEul(:,1)-Eul(:,1)); 
subplot(3,1,2);
histfit(NEul(:,2)-Eul(:,2)); 
subplot(3,1,3);
histfit(NEul(:,3)-Eul(:,3)); 
title('Error between NEul and Eul');

figure(2)
subplot(3,1,1);
plot(1:length(NEul),NEul(:,1)-Eul(:,1));
subplot(3,1,2);
plot(1:length(NEul),NEul(:,2)-Eul(:,2));
subplot(3,1,3);
plot(1:length(NEul),NEul(:,3)-Eul(:,3));
title('Error between NEul and Eul');
xlabel('Sample Count');
ylabel('Error in degree');

%% open loop integration with noise
clear;
clear clc;
close all;

dT = 1/50;
GscaleFactor = (1/250)*(2^15-1); 
[Acc,Mag,wGyro,Eul] = CreateTrajectoryData(dT,true);

yaw = Eul(1,1);
pitch = Eul(1,2);
roll = Eul(1,3);

R = [cosd(pitch)*cosd(yaw) cosd(pitch)*sind(yaw) -1*sind(pitch), 
    (sind(roll)*sind(pitch)*cosd(yaw))-(cosd(roll)*sind(yaw)) (sind(roll)*sind(pitch)*sind(yaw))+(cosd(roll)*cosd(yaw)) sind(roll)*cosd(pitch),
    (cosd(roll)*sind(pitch)*cosd(yaw))+(sind(roll)*sind(yaw)) (cosd(roll)*sind(pitch)*sind(yaw))-(sind(roll)*cosd(yaw)) cosd(roll)*cosd(pitch)];


gyroSecond = wGyro(1:50,1:3); % first second data
biasP = mean(gyroSecond(:,1));
biasQ = mean(gyroSecond(:,2));
biasR = mean(gyroSecond(:,3));

for i = 1 : length(wGyro)
    nwGyro(i,1) = wGyro(i,1) - biasP;
    nwGyro(i,2) = wGyro(i,2) - biasQ;
    nwGyro(i,3) = wGyro(i,3) - biasR;
end

for i = 1: length(nwGyro)
    p(i) = deg2rad(nwGyro(i,1)/GscaleFactor);
    q(i) = deg2rad(nwGyro(i,2)/GscaleFactor);
    r(i) = deg2rad(nwGyro(i,3)/GscaleFactor);
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


figure(1)
subplot(3,1,1);
histfit(NEul(:,1)-Eul(:,1)); 
subplot(3,1,2);
histfit(NEul(:,2)-Eul(:,2)); 
subplot(3,1,3);
histfit(NEul(:,3)-Eul(:,3)); 
title('Error between NEul and Eul');

figure(2)
subplot(3,1,1);
plot(1:length(NEul),NEul(:,1)-Eul(:,1));
subplot(3,1,2);
plot(1:length(NEul),NEul(:,2)-Eul(:,2));
subplot(3,1,3);
plot(1:length(NEul),NEul(:,3)-Eul(:,3));
title('Error between NEul and Eul');
xlabel('Sample Count');
ylabel('Error in degree');

