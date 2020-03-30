close all
clear all
clc

EllipseXYData;
M = [];
M(:,1) = Xmeas;
M(:,2) = -(Ymeas.^2);
M(:,3) = Ymeas;
M(:,4) = ones(1);

k = M \ (Xmeas.^2)

% Calculating all variables
x0 = k(1) / 2
y0 = k(3) / (2*k(2))
a = sqrt((k(4)+x0^2+(k(2)*(y0^2)))/(R^2)) 
b = sqrt((a^2)/k(2))

%draw
for i = 1:150
    x1(i) = (Xmeas(i) - x0) / (a);
end

for j = 1:150
    y1(j) = (Ymeas(j) - y0) / (b);
end

figure(2)
plot(x1(1,:),y1(1,:),'.');
hold on
plot(Xmeas,Ymeas,'.');
axis equal
xlabel('x1');
ylabel('y1');
title('Plot of the circle using X,Y data');
legend('postcal', 'precal');

