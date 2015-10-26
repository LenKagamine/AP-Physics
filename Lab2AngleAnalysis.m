% Script to Calculate Angle from a set of Time data
dates = {'Oct20sample', 'Oct21sample', 'Oct22sample', 'Oct23sample'}; %'Oct19sample', 'Oct20sample', 'Oct21sample', 'Oct22sample', 'Oct23sample'
samples = [6 6 6 4]; %3 6 6 6 4
names = cell(1, sum(samples));

% Create file names
i = 1;
for d = 1:numel(samples)
    for s = 1:samples(d)
        names(i) = strcat(dates(d), int2str(s));
        i = i+1;
    end
end

% filename = struct;

type = zeros(size(names));
lastline = zeros(size(names)); %[1567 1568 1564 1564 1558 1568];
wid = 0.0277;
m = 1;
L =  zeros(size(names)) + 0.1997; %[0.4087 0.4087 0.4087 0.4087 0.4087 0.4087];

% Set up figures
angvst = figure;
title('Angle vs Time');
xlabel('Time (s)');
ylabel('Length');
zlabel('Angle (deg)');
view(3);
set(gca, 'FontSize', 30)
hold on

pervsang = figure;
title('Period vs Angle');
xlabel('Angle (deg)');
ylabel('Length');
zlabel('Period (s)');
view(3);
set(gca, 'FontSize', 30)
hold on

pervstime = figure;
title('Period vs Time');
xlabel('Time (s)');
ylabel('Length');
zlabel('Period (s)');
view(3);
set(gca, 'FontSize', 30)
hold on

% Create line formats
linespec = {'r--', 'g-.', 'b:'};

% Create output structures
theta = zeros(1117, sum(samples));
leng = zeros(1117, sum(samples));
t = zeros(1117, sum(samples));
T = zeros(558, sum(samples));

% excels = cell(4,1); % 4 lengths
xls = [];

for i = 1:numel(lastline);
    fi = ['f' int2str(i)];
    filename = strcat('Data/', names(i), '.csv');
    
    % Read info
    type(i) = csvread(filename{1}, 1, 3, [1 3 1 3]);
    L(i) = csvread(filename{1}, 1, 4, [1 4 1 4]);
    lastline(i) = csvread(filename{1}, 1, 5, [1 5 1 5]);
    
    ti = csvread(filename{1}, 1, 0, [1 0 lastline(i) 0]); % Time
    state = csvread(filename{1}, 1, 1, [1 1 lastline(i) 1]); % State
    Ti = csvread(filename{1}, 1, 2, [1 2 lastline(i) 2]); % Period

    % Calculate v
    dt = diff(ti);
    v = wid./ dt(1:2:end);

    % Calculate w
    w = v./L(i);
    
    % Calculate m
    if     type == 0; m = 0.0757;
    elseif type == 1; m = 0.1303;
    elseif type == 2; m = 0.2860;
    end
    
    % Calculate I
    I = 1/4*m*(wid/2)^2 + 1/12*m*0.0525^2 + m*L(i).^2;
    
    % Calculate E
%     E = 0.5*m.*v.^2;
    E = 0.5*I.*w.^2;

    
    % Calculate h
    h = E./m./9.8;

    % Calculate angle
    thetai = acos(1 - h./L(i))/pi*180;

    % Store Output
    ti = ti(1:2:end-1); % Time 
    t(1:numel(ti), i) = ti;
    t(t(:,i)==0,i) = ti(end);
    
    theta(1:numel(thetai), i) = thetai; % Angle
    theta(theta(:,i)==0,i) = thetai(end);
    
    Ti = Ti(5:4:end-1); % Period
    T(1:numel(Ti), i) = Ti;
    T(T(:,i)==0,i) = Ti(end);
    
    leng(:,i) = zeros(1117,1) + L(i); % Length of pendulum
    
    % Plot output
    lengi = zeros(size(ti)) + L(i);
    
    figure(angvst); % Angle versus time and length
    plot3(ti, lengi, thetai, linespec{type(i)+1}, 'LineWidth', 2);

    figure(pervsang); % period versus angle and length (this is the important one!)
    plot3(thetai(3:2:end),lengi(3:2:end),Ti, linespec{type(i)+1}, 'LineWidth', 2);
    
    figure(pervstime); % Period versus time and length
    plot3(ti(3:2:end),lengi(3:2:end),Ti, linespec{type(i)+1}, 'LineWidth', 2);
    
    % Add to excel files 
    % Create length index
    if L(i) == 0.2037 
        lind = 1;
    elseif L(i) == 0.3044 
        lind = 2;
    elseif L(i) == 0.4072
        lind = 3;
    elseif L(i) == 0.5095
        lind = 4;
    end
        
    mi = zeros(size(Ti)) + m;
    pert = Ti-2*pi*sqrt(lind/9.80665);
    
    xls = [xls; [thetai(3:2:end) thetai(3:2:end).^2 zeros(size(Ti))+lind Ti pert]];
%     excels{lind,1} = [excels{lind,1}; [thetai(3:2:end) thetai(3:2:end).^2 Ti]];
end

xlswrite('Tfit2', xls);

% output excels
% for l = 1:4
% %     for m = 1:3
%         filename = ['length' num2str(l)];% 'mass' num2str(m)];
%         xlswrite(filename, excels{l,1});
% %     end
% end

% Format Graphs 
figure(angvst);
legend('wood', 'copper',  'alum');

figure(pervsang);
plot3(theta, leng, 2*pi.*sqrt(leng./9.8), 'k');
legend('wood', 'copper',  'alum');

figure(pervstime);
plot3(t, leng, 2*pi.*sqrt(leng./9.8), 'k');
legend('wood', 'copper',  'alum');
