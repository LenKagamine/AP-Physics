% Script to Calculate Angle from a set of Time data
dates = {'Oct21sample'}; %'Oct19sample', 'Oct20sample', 'Oct21sample', 'Oct22sample', 'Oct23sample'
samples = [6]; %3 6 6 6 4
names = cell(1, sum(samples));

% Create file names
i = 1;
for d = numel(samples):-1:1
    for s = samples(d):-2:1
        names(i) = strcat(dates(d), int2str(s));
        i = i+1;
    end
    for s = samples(d)-1:-2:1
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
title('Angle vs Time', 'FontWeight', 'normal');
xlabel('Time (s)');
ylabel('Length (m)');
zlabel('Angle (deg)');
view(3);
box on;
set(gca, 'FontSize', 30, 'FontName', 'Times')
hold on

pervsang = figure;
title('Period vs Angle', 'FontWeight', 'normal');
xlabel('Angle (deg) -->');
ylabel('Length');
zlabel('Period (s) -->');
view(3);
box on;
set(gca, 'FontSize', 30, 'FontName', 'Times')
hold on

pervstime = figure;
title('Period vs Time', 'FontWeight', 'normal');
xlabel('Time (s) -->');
ylabel('Length');
zlabel('Period (s) -->');
view(3);
box on;
set(gca, 'FontSize', 30, 'FontName', 'Times')
hold on

% Create line formats
linespec = {'k--', 'k-.', 'k-'};

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

    if L(i) == 0.2037 
        L(i) = 0.2011;
    elseif L(i) == 0.3044 
        L(i) = 0.3005;
    elseif L(i) == 0.4072
        L(i) = 0.4006;
    elseif L(i) == 0.5095
        L(i) = 0.5033;
    end
    
    % Calculate v
    dt = diff(ti);
    v = wid./ dt(1:2:end);

    % Calculate w
    w = v./L(i);
    
    % Calculate m
    if     type(i) == 0; m = 0.0757;
    elseif type(i) == 1; m = 0.1303;
    elseif type(i) == 2; m = 0.2860;
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
    plot3(thetai(3:2:end),lengi(3:2:end),Ti, linespec{type(i)+1}, 'LineWidth', 1.5);
    
    figure(pervstime); % Period versus time and length
    plot3(ti(3:2:end),lengi(3:2:end),Ti, linespec{type(i)+1}, 'LineWidth', 2);
    
    % Add to excel files 
    % Create length index
        
    usable = thetai < 14;
    
    mi = zeros(size(thetai)) + m;
    pert = Ti-2*pi*sqrt(L(i)/9.80665);
    theta0ind = find(usable);
    theta0 = zeros(size(thetai))+thetai(theta0ind(1));
    Li = zeros(size(thetai))+L(i);
    
    ti = ti - ti(theta0ind(1));
    fitfunc = log(thetai ./ theta0).*2.*m./L(i);
    xls = [xls; [ti(usable) thetai(usable) fitfunc(usable) theta0(usable) mi(usable) Li(usable)]];
%     xls = [xls; [thetai(3:2:end) thetai(3:2:end).^2 zeros(size(Ti))+L(i) Ti pert]];
%     excels{L(i),1} = [excels{L(i),1}; [thetai(3:2:end) thetai(3:2:end).^2 Ti]];
end

% xlswrite('kfitUsable', xls);

% output excels
% for l = 1:4
% %     for m = 1:3
%         filename = ['length' num2str(l)];% 'mass' num2str(m)];

%         xlswrite(filename, excels{l,1});
% %     end
% end

% Format Graphs 
figure(angvst);
legend('wood', 'aluminum', 'copper', 'Location', 'northeast');

figure(pervsang);
% plot3(theta, leng, 2*pi.*sqrt(leng./9.8), 'k:', 'LineWidth', 1.5);
legend('wood', 'aluminum', 'copper', 'Location', 'east');
view(0,0);
xlim([0 80]);
n = 0.8:0.2:1.6;
labels = cell(size(n));
for i = 1:numel(n)
    labels{i} = sprintf('%.1f ',n(i));
end
set(gca,'ZTickLabel',labels);

figure(pervstime);
% plot3(t, leng, 2*pi.*sqrt(leng./9.8), 'k:', 'LineWidth', 1.5);
legend('wood', 'aluminum', 'copper', 'Location', 'northeast');
n=get(gca,'ZTick');
n = 1.08:0.02:1.16;
labels = cell(size(n));
for i = 1:numel(n)
    labels{i} = sprintf('%.2f ',n(i));
end
set(gca,'ZTickLabel',labels);
% gca.YTickLabel = {'0.9', '1.0', '1.1', '1.2', '1.3', '1.4', '1.5', '1.6'};
view(0,0);
