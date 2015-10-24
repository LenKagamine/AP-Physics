% Script to Calculate Angle from a set of Time data
dates = {'Oct19sample', 'Oct20sample', 'Oct21sample', 'Oct22sample', 'Oct23sample'};
samples = [3 6 6 6 4];
names = cell(1, sum(samples));

% Create file names
i = 1;
for d = 1:numel(samples)
    for s = 1:samples(d)
        names(i) = strcat(dates(d), int2str(s));
        i = i+1;
    end
end

filename = struct;

type = zeros(size(names));
lastline = zeros(size(names)); %[1567 1568 1564 1564 1558 1568];
wid = 0.0277;
m = 1;
L =  zeros(size(names)); %[0.4087 0.4087 0.4087 0.4087 0.4087 0.4087];

% Set up figures
angvst = figure;
title('Angle vs Time');
xlabel('Time (s)');
ylabel('Length');
zlabel('Angle (deg)');
view(3);
hold on

pervsang = figure;
title('Period vs Angle');
xlabel('Angle (deg)');
ylabel('Length');
zlabel('Period (s)');
view(3);
hold on

pervstime = figure;
title('Period vs Time');
xlabel('Time (s)');
ylabel('Length');
zlabel('Period (s)');
view(3);
hold on

% Create output structures
theta = zeros(1117, sum(samples));
leng = zeros(1117, sum(samples));
t = zeros(1117, sum(samples));
T = zeros(558, sum(samples));

for i = 1:numel(lastline);
    fi = ['f' int2str(i)];
    filename.(fi) = strcat('Data/', names(i), '.csv');
    
    % Read info
    type(i) = csvread(filename.(fi){1}, 1, 3, [1 3 1 3]);
    L(i) = csvread(filename.(fi){1}, 1, 4, [1 4 1 4]);
    lastline(i) = csvread(filename.(fi){1}, 1, 5, [1 5 1 5]);
    
    ti = csvread(filename.(fi){1}, 1, 0, [1 0 lastline(i) 0]); % Time
    state = csvread(filename.(fi){1}, 1, 1, [1 1 lastline(i) 1]); % State
    Ti = csvread(filename.(fi){1}, 1, 2, [1 2 lastline(i) 2]); % Period

    % Calculate v
    dt = diff(ti);
    v = wid./ dt(1:2:end);

    % Calculate E
    E = 0.5*m.*v.^2;

    % Calculate h
    h = E./m./9.8;

    % Calculate angle
    thetai = acos(1 - h./L(i))/pi*180;

    % Store Output
    ti = ti(1:2:end-1);
    t(1:numel(ti), i) = ti;
    t(t(:,i)==0,i) = ti(end);
    
    theta(1:numel(thetai), i) = thetai;
    theta(theta(:,i)==0,i) = thetai(end);
    
    Ti = Ti(5:4:end-1);
    T(1:numel(Ti), i) = Ti;
    T(T(:,i)==0,i) = Ti(end);
    
    % Plot output
    leng(:,i) = zeros(1117,1) + L(i);
    
%     figure(angvst);
%     plot3(ti(1:2:end-1), leng, thetai);
% 
%     figure(pervsang);
%     plot3(thetai(3:2:end),leng(3:2:end),Ti(5:4:end-1));
%     
%     figure(pervstime);
%     plot3(ti(5:4:end-1),leng(3:2:end),Ti(5:4:end-1));
end

% Create Legend
leg = cell(size(names));
leg(type == 0) = {'Wood'};
leg(type == 1) = {'Aluminum'};
leg(type == 2) = {'Copper'};


% Format Graphs 
figure(angvst);
for ty = 0:2
    plot3(t(:,type==ty), leng(:,type==ty), theta(:,type==ty));
end
% legend(leg);
legend('wood', 'alum', 'copper');

figure(pervsang);
for ty = 0:2
    plot3(theta(3:2:end,type==ty), leng(3:2:end,type==ty), T(:,type==ty));
end
% legend(leg);
legend('wood', 'alum', 'copper');

figure(pervstime);
for ty = 0:2
    plot3(t(3:2:end,type==ty), leng(3:2:end,type==ty), T(:,type==ty));
end
% legend(leg);
legend('wood', 'alum', 'copper');
