% Interpolation of Soup Can Roll Data 

data = xlsread('cream2v2A', 'B2:K196'); % Get data from excel sheet
t = xlsread('cream2v2A', 'A2:A196');


sdata = zeros(size(data)); % matrix to hold interpolated data

for run = 1:10 % for each run
    inds = ~isnan(data(:,run)); % Find indices of non-NaN values
    y = data(inds, run); % find non-Nan values
    x = t(inds); % Find non-NaN timepoints
    sdata(:,run) = interp1(x,y,t, 'linear'); % Perform interpolation
end

% Calculate offset of position graphs
offset = mean(sdata(1,:),2);
sdata = sdata - offset;

% datamean = mean(data,2,'omitnan'); 
s = mean(sdata,2,'omitnan'); % get mean values 
ssigma = std(sdata,0,2,'omitnan');

% t1 = [];
% data1 = [];
% 
% t2 = [];
% data2 = [];
% 
% for run = 1:10
%     data1 = [data1 ; sdata(sdata(:,run) < 0.8844,run)];
%     t1 = [t1 ; t(sdata(:,run) < 0.8844)];
%     
%     data2 = [data2 ; sdata(sdata(:,run) >= 0.8844 & t < 7.8 ,run)];
%     t2 = [t2 ; t(sdata(:,run) >= 0.8844 & t < 7.8)];
% end
% 
% xlswrite('RegCream2-1', [t1 t1.^2 data1]);
% xlswrite('RegCream2-2', [t2 t2.^2 data2]);
% 
% xlswrite('InterpCream2', [t s], 'A2:A201');

figure % Plot original data and mean
hold on;

a1 = 0.026650691;
b1 = 0.06518716;

a2 = -0.041114787;
b2 = 0.680703461;
c2 = -1.035239235;

mid = 81;
nd = 165;

fit = a1.*t(1:nd).^2 + b1.*t(1:nd);
fit(mid:nd) = a2.*t(mid:nd).^2 + b2.*t(mid:nd) + c2;

figure % Plot interpolated data and mean
title('Position versus Time of Cream on Shallow Slope', 'FontWeight', 'normal');
hold on
plot(t(1:nd),fit,'k--');
plot(t,sdata,'Color', [.80 .80 .80]);
plot(t(1:nd),fit,'k--');
hold off

xlabel('t (s) -->');
ylabel('s (m) -->');
set(gca, 'fontsize', 30, 'fontname', 'Times');
box on;
legend('Piece-wise Quadratic Fit', 'Cleaned, Time-Aligned Data', 'Location', 'east');
% 
% 
% %% Velocity Analysis
% % Velocity
% vt = t(1:end-1) + diff(t)/2;
% dt = diff(t);
% vdata = diff(sdata)./dt(1);
% 
% vmean = mean(vdata, 2, 'omitnan');
% 
% % v = interp1(vt,vmean,t);
% v = vmean;
% vsigma = std(vdata,0,2,'omitnan');
% 
% figure % Plot interpolated data and mean
% title('v-t Graph');
% hold on
% plot(vt,vdata,'y');
% % plot(vt,v, 'k');
% % plot(vt,v+vsigma, 'b');
% % plot(vt,v-vsigma,'b');
% % plot(vt, 
% hold off
% 
% %% Energy analysis
% 
% % Gravitational Potential Energy
% d = (min(s)+0.8844) - s;
% d(d < 0) = 0;
% h = d.*sin(4.1/180*pi);
% m = 0.346;
% g = 9.8;
% gpe = m*g.*h;
% 
% % Translational Kinetic Energy
% Tk = 0.5*m.*v.^2;
% 
% % Rotational Kinetic Energy of Can
% mlid = 5.13 / 1000;     % Mass of Can Lid (g --> kg)
% mlidsigma = .01 / 1000; % Uncertainty of above (g --> kg)
% r = 66.14/2 / 1000;     % Radius of can (mm --> m)
% rsigma = .02/2 / 1000;  % Unc. of above (mm --> m)
% 
% Ilid = 0.5*mlid*r^2;                                            % Rotational Inertia of Can Lid
% Ilidsigma = Ilid*sqrt((mlidsigma/mlid)^2 + 2*(rsigma/r)^2);     % Unc. of above
% 
% mwall = 31.08 / 1000;       % Mass of can wall (g --> kg)
% mwallsigma = .01 / 1000;    % Unc. of above (g --> kg)
% L = 95.42 / 1000;           % Length of can (mm --> m)
% Lsigma = .02 / 1000;        % Unc. of above (mm --> m)
% 
% Iwall = mwall*r^2;                                          % Rot'l Inertia of Can Wall
% Iwallsigma = Iwall*sqrt((mwallsigma/mwall)^2+2*(rsigma/r)^2);   % Unc. of above
% 
% I = Iwall + 2*Ilid;                             % Total I
% Isigma = sqrt(Iwallsigma^2 + (2*Ilidsigma)^2);  % Unc. of I
% 
% w = v./r;        % Rotational Velocity
% %%%% wsigma
% 
% Trc = 0.5*I.*w.^2; % Rotational Kinetic Energy
% 
% 
% % Rotational Kinetic Energy of Soup
% mtotal = 343.6 / 1000;
% mtotalsigma = .2 / 1000;
% 
% msoup = mtotal - 2*mlid - mwall;
% 
% Isoup = 0.5*msoup*r^2;
% 
% Trs = 0.5.*Isoup.*w.^2;
% 
% % plot(vt,Trs);
% 
% % Total Energy
% 
% figure 
% title('Energy Values for Cream of Celery on Shallow Slope', 'FontWeight', 'normal');
% hold on
% plot(t,gpe, '--k');
% plot(vt,Tk, '-.k');
% plot(vt,Trc, ':k');
% plot(vt,Tk+gpe(1:end-1)+Trc, 'k');
% % plot(t,s./10);
% hold off 
% 
% xlabel('t (s) -->');
% ylabel('E (J) -->');
% set(gca, 'fontsize', 30, 'fontname', 'Times');
% box on;
% legend('Gravitational Potential Energy', 'Translational Kinetic Energy', 'Rotational Kinetic Energy of Can', 'Total Energy of Above');
% 
