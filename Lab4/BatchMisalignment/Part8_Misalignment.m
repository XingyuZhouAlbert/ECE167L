close all
clear all
clc

Part8_Tumble_Mag_Accel

mi = [0;0;1];
si = [22770;5329;41510.2]/1000;


Mmeas(1,:) = Xc;
Mmeas(2,:) = Yc;
Mmeas(3,:) = Zc;
Smeas(1,:) = XMc;
Smeas(2,:) = YMc;
Smeas(3,:) = ZMc;

[Rge,Pge] = AlignMasterSlave(Mmeas,Smeas,mi,si);




