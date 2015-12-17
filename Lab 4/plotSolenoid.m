%renders magnetic field from two Helmholtz coils
%using data produced by simulation
R = 0.07;           %radius
L = 0.022;          %length
w = 91392;          %angular freq
t = 0:0.000001:L;   %step
theta = -72;        %angle of first coil
theta2 = -120;      %angle of second coil
sizex = 0.24;       %size of 3-D grid (x)
sizey = 0.24;       %size (y)
sizez = 0.24;       %size (z)

%default coil parametric equations
x = R * cos(w * t);
y = t;
z = R * sin(w * t);

rot = zeros(length(x),3);   %rotation for first coil
rot2 = zeros(length(x),3);  %rotation for second coil
for i = 1:length(x)         %multiplying by rotation matrix about z-axis
    rot(i,:) = transpose(rotz(theta) * [x(i);y(i);z(i)]);
    rot2(i,:) = transpose(rotz(theta2) * [x(i);y(i);z(i)]);
end

%first coil
x1 = transpose(rot(:, 1)) + sizex/2 + 0.04;
y1 = transpose(rot(:, 2)) + sizey/2 - L/2;
z1 = transpose(rot(:, 3)) + sizez/2;
%second coil
x2 = transpose(rot2(:, 1)) + sizex/2 - 0.04;
y2 = transpose(rot2(:, 2)) + sizey/2 - L/2 + 0.01;
z2 = transpose(rot2(:, 3)) + sizez/2;

data = csvread('simData.csv', 1, 0); %import csv file
%extract data
X = data(:, 1); %position (x,y,z)
Y = data(:, 2);
Z = data(:, 3);
U = data(:, 4); %magnetic field at position (Bx, By, Bz)
V = data(:, 5);
W = data(:, 6);

plot3(x1, y1, z1); %plot first coil
hold on;
plot3(x2, y2, z2);  %plot second coil
quiver3(X, Y, Z, U, V, W, 100); %plot magnetic field using vectors
hold off;

axis([0 0.24 0 0.24 0 0.24]); %set plot boundaries
pbaspect([1 1 1]); %aspect ratio of axes
daspect([1 1 1]); %aspect ratio of data