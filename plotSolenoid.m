t=0:0.000001:0.022;
R=0.07;
w=2*pi*320/0.022;
x1=R*cos(w*t)+0.1;
y1=R*sin(w*t)+0.1;
z1=t+0.03;

x2=-t+0.192;
y2=R*sin(w*t)+0.1;
z2=R*cos(w*t)+0.1;

data = csvread('data.csv', 1, 0);
X = data(:, 1);
Y = data(:, 2);
Z = data(:, 3);
U = data(:, 4);
V = data(:, 5);
W = data(:, 6);

plot3(x1, y1, z1);
hold on;
plot3(x2, y2, z2);
quiver3(X, Y, Z, U, V, W, 10);
hold off;
axis([0 0.24 0 0.24 0 0.24]);
pbaspect([1 1 1]);
daspect([1 1 1]);