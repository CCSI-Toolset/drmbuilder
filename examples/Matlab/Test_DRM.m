% Driver to call DABNet D-RM generated by D-RM Builder
% 
% Code written and developed by:
% Priyadarshi Mahapatra, PhD
% Jinliang Ma, PhD
% URS Corporation / National Energy Technology Laboratory
% 24 Sept 2013
% 
% The following files are required
% *.m:		D-RM model exported by D-RM Builder
% *.csv:	training or validation data file exported by D-RM Builder

clear all; clc;     %close all;

% choose D-RM model file
[filename, pathname] = uigetfile('*.m','Choose D-RM Model Parameter File');
if isequal(filename,0)
    disp('User selected cancel')
	return;
end
run(fullfile(pathname,filename)); % Load parameters related to D-RM object into workspace
if strcmp(DRM_type,'DABNet')==1
    myDRM = DRM_DABNet(dt,A,B,NN,u_mean,y_mean,u_sigma,y_sigma);
else
    myDRM = DRM_NARMA(dt,nu,ny,nhistory,u_mean,y_mean,u_sigma,y_sigma,NN);
end
[filename, pathname] = uigetfile('*.csv','Choose Training or Validation Data File');
if isequal(filename,0)
    disp('User selected cancel')
    return;
end
u_csv = csvread(fullfile(pathname,filename),1,1);	%read training input sequence from training data exported by D-RM Builder
%prepare sequence of input data u
u = [];
j = 1;
for i=1:nu
   u = [u u_csv(:,input_indices(i))];
end
u = u';
npoint = size(u,2); %number of time steps in the sequence
if strcmp(DRM_type,'DABNet')==1
    clear u_mean y_mean u_sigma y_sigma A B NN; % Remove loaded model parameters
    % Define initial conditions for the D-RM (initial states are defined @SS)
    myDRM.initialize(u(:,1));
else
    y_ss = zeros(ny,1);
    for i=1:ny
        y_ss(i) = u_csv(1,ninput+i);
    end
    clear nhistory u_mean y_mean u_sigma y_sigma NN;
    % Define initial conditions for the D-RM (initial states are defined @SS)
    myDRM.initialize(u(:,1),y_ss);
end
y(:,1) = myDRM.y;
clear u_csv;    %remove loaded training/validation data


%% Define Simulation Time
tbeg = 0;
tend = myDRM.dt*(npoint-1);
t = tbeg:myDRM.dt:tend;


%% Run Simulation
for k = 1:npoint-1
    myDRM.evalNextStep(u(:,k));
    y(:,k+1) = myDRM.y;
end

%% Save Result to a file
[filename, pathname] = uiputfile('*.csv','Save D-RM Predicted Results As');
if isequal(filename,0) || isequal(pathname,0)
    disp('User selected Cancel. Predicted results will not be saved.');
else
    csvwrite(fullfile(pathname,filename),y');
end

%% Plotting the responses
figure;
sp_no = 0;
for i = 1:ny
    sp_no = sp_no + 1;
    subplot(2,max(ny,nu),sp_no), plot(t,y(i,:),'r','Linewidth',1.2);
    ylabel(['y' num2str(i)]);
    xlim([t(1) t(npoint)]); grid on;
end
sp_no = max(ny,nu);
for j = 1:nu
    sp_no = sp_no + 1;
    subplot(2,max(ny,nu),sp_no), stairs(t,u(j,:),'Linewidth',1.2);
    xlabel('Time [hr]'); ylabel(['u' num2str(j)]);
    y_range = max(u(j,:)) - min(u(j,:));
    if y_range == 0
        y_range = abs(0.5*max(u(j,:)));
    end
    xlim([t(1) t(npoint)]); grid on;
    ylim([min(u(j,:))-0.05*y_range max(u(j,:))+0.05*y_range]);
end
