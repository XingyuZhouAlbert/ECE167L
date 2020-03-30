
close all
clear all
clc

GscaleFactor = (1/250)*(2^15-1); 
deltaT = 1/50;
[Acc,Mag,wGyro,Eul] = CreateTrajectoryData(deltaT,true);

swGyro = wGyro/131;

Rminus = eye(3);


gyroSecond = swGyro(1:50,1:3); % first second data
biasP = mean(gyroSecond(:,1));
biasQ = mean(gyroSecond(:,2));
biasR = mean(gyroSecond(:,3));

Binitial = [0;0;0];


He = [22770;5329;41510.2]/1000;     % Earth's magnetic field in uT (NED)
Hu = He/norm(He);                   % magnetic field unit vector
Ge = [0;0;1];                       % Earth's gravitational field in g (NED)


% magInertial = [22825.7/1000;5393.9/1000;41690.9/1000];
% accelInertial = [0;0;-1];


for i = 51 : length(wGyro)        
    nwGyrox = deg2rad((swGyro(i,1)));
    nwGyroy = deg2rad((swGyro(i,2)));
    nwGyroz = deg2rad((swGyro(i,3)));
    nwGyro=[nwGyrox;nwGyroy;nwGyroz];
    if i == 51
        [Rplus, Bplus] = IntegrateClosedLoop(Rminus, Binitial, nwGyro, [Mag(i,1);Mag(i,2);Mag(i,3)], [Acc(i,1);Acc(i,2);Acc(i,3)], Hu, Ge, deltaT)
    else
        [Rplus, Bplus] = IntegrateClosedLoop(Rplus, Bplus, nwGyro, [Mag(i,1);Mag(i,2);Mag(i,3)], [Acc(i,1);Acc(i,2);Acc(i,3)], Hu, Ge, deltaT)
    end

    NEul(i,1) = atan2d(Rplus(1,2),Rplus(1,1)); %yaw
    NEul(i,2) = asind(-1*Rplus(1,3));           %pitch
    NEul(i,3) = atan2d(Rplus(2,3),Rplus(3,3)); %roll
end

figure(1)
subplot(3,1,1);
histfit(NEul(:,1)-Eul(:,1)); 
subplot(3,1,2);
histfit(NEul(:,2)-Eul(:,2)); 
subplot(3,1,3);
histfit(NEul(:,3)-Eul(:,3)); 
title('Error between NEul and Eul');


figure(4)
plot(abs(NEul)-abs(Eul));
legend("yaw","pitch","roll");
title('Error between NEul and Eul');

