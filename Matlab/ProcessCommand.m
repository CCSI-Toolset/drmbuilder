%function ProcessCommand
% CCSI DRM Training/Validation Simulator and Plot/Compare Tool
% 
% Code written and developed by:
% Priyadarshi Mahapatra and Jinliang Ma
% URS Corporation / National Energy Technology Laboratory
% August 8, 2013
% 
% Version Changes
% [2012-10-28] Initiated development for communication to/from files
% [2013-04-03] Merged 'Training' and 'Validation' into a single 'Simulation' section
% [2013-04-04] Coded separate sections for 'Simulation' and 'Plot'/'Compare'
% [2013-05-07] Modified code to include 'modeled' output using indices
% [2013-07-12] Major code modification for parsing 'Command_Parameters.txt' content
%              Incorporate 'ramped' step-changes for solver robustness within code
% [2013-07-22] Modified code for correct "cause-n-effect" b/w inputs and outputs
% [2013-08-08] Revised to interact with MATLAB engine, input/output arrays assigned through MATLAB engine
% [2014-07-18] Revised to include command for Kalman filter based UQ analysis             


dt = ID_Par{2}; % sampling/communication time step, in ACM's time unit

switch lower(ID_Par{12}) % command name
    case {'simulation'}
        dt_min = ID_Par{3}; % minimum time-step for numerical integration in ACM
        TY = []; Y = []; TU = []; U = [];
        disp([upper(ID_Par{13}) ' Data Selected']);
        
        nu = size(u,1);
        n = size(u,2);
        t = 0:dt:(n-1)*dt;
        
        u_ramprate = ID_Par{7};
        disp('Running High Fidelity Model...');
        for k = 1:n-1
            if k == 1
                up = u(:,k);
                sim(ID_Par{1},[t(k) t(k+1)]); tout = tout'; yp = yp';
                TY = [TY tout]; Y = [Y yp]; TU = [TU tout(end)]; U = [U up];
                y(:,1) = yp(:,1); % Alternatively this could be provided from ACM snapshot file
            else
                % Count how many max number of small-steps are required across all inputs
                for j = 1:nu
                    if u_ramprate(j) == -1
                        n_k(j) = 0;
                    else
                        n_k(j) = fix(min(dt/dt_min, abs(u(j,k)-up(j))/u_ramprate(j)/dt_min));
                    end
                end
                nk = max(n_k);
                if nk == 0 % When ramped u hits final-value u(:,k)
                    up = u(:,k);
                    sim(ID_Par{1},[t(k) t(k+1)]); tout = tout'; yp = yp';
                    TY = [TY tout]; Y = [Y yp]; TU = [TU tout(end)]; U = [U up];
                else
                    tk = t(k);
                    for kk = 1:nk
                        for j = 1:nu
                            if u_ramprate(j) == -1
                                up(j) = u(j,k);
                            else
                                up(j) = up(j) + dt_min*u_ramprate(j)*sign(u(j,k)-up(j));
                            end
                        end
                        SIMOPT = simset('FixedStep', dt_min);
                        sim(ID_Par{1},[tk tk+dt_min],SIMOPT); tout = tout'; yp = yp';
                        TY = [TY tout]; Y = [Y yp]; TU = [TU tout(end)]; U = [U up];
                        tk = tk + dt_min;
                    end
                    if tk < t(k+1)
                        up = u(:,k);
                        sim(ID_Par{1},[tk t(k+1)]); tout = tout'; yp = yp';
                        TY = [TY tout]; Y = [Y yp]; TU = [TU tout(end)]; U = [U up];
                    end
                end
            end
            y(:,k+1) = yp(:,end);
        end
        
        %y' is saved to memory through MATLAB engine
        
        % Shift inputs to the left for displaying lagging 'zero-order-hold' points in 'stairs' command         
        TU = [t(1) TU];
        U = [U U(:,end)];
    
        % Plot continuous variables (for debugging)
        ny = size(y,1);
        t_label = ID_Par{4};
        u_label = strsplit(ID_Par{5}, ' ');
        u_units = strsplit(ID_Par{6}, ' ');
        y_label = strsplit(ID_Par{9}, ' ');
        y_units = strsplit(ID_Par{10}, ' ');
        
        figure;
        sp_no = 0;
        for i = 1:ny
            sp_no = sp_no + 1;
            subplot(2,max(ny,nu),sp_no), plot(TY,Y(i,:),'k','Linewidth',1.2);
            ylabel([y_label{i} char(10) '[' y_units{i} ']']);
            xlim([t(1) t(n)]); grid on;
        end
        sp_no = max(ny,nu);
        for j = 1:nu
            sp_no = sp_no + 1;
            subplot(2,max(ny,nu),sp_no), stairs(t,u(j,:),'Linewidth',1.2); hold on;
            stairs(TU,U(j,:),'r','Linewidth',1.2);
            xlabel(['t [' t_label ']']); ylabel([u_label{j} char(10) '[' u_units{j} ']']);
            y_range = max(U(j,:)) - min(U(j,:));
            if y_range == 0
                y_range = abs(0.5*max(U(j,:)));
            end
            xlim([t(1) t(n)]); grid on;
            ylim([min(U(j,:))-0.05*y_range max(U(j,:))+0.05*y_range]);
        end

        
    case {'plot'}
        disp([upper(ID_Par{13}) ' Data Selected']);
        ym_idx = ID_Par{11};  % output index in Simulink output variable list, different from yplot_idx
        ym = y;  % y is from MATLAB engine as the ACM output
        ym(ym_idx,:) = ym_;  % ym_ is from MATLAB engine as the DRM output, replacing varied outputs only
        
        t_label = ID_Par{4};  % time unit
        u_label = strsplit(ID_Par{5}, ' ');  % input names
        u_units = strsplit(ID_Par{6}, ' ');  % input units
        y_label = strsplit(ID_Par{9}, ' ');  % output names
        y_units = strsplit(ID_Par{10}, ' ');  % output units
        
        n = size(u,2);  % u is input from MATLAB Engine
        t = 0:dt:(n-1)*dt;
        
        uplot_idx = ID_Par{14};  % user slected input variable indices
        yplot_idx = ID_Par{15};  % user selected output variable indices, subset of ym_idx
        nu = length(uplot_idx);
        ny = length(yplot_idx);
        
        figure;
        nrow_plot = 1 + bplot_error + bplot_step_change + bplot_correlation; %bplot_* are set by MATLAB engine
        %first row, always plot output response
        sp_no = 0;
        for i = 1:ny
            sp_no = sp_no + 1;
            subplot(nrow_plot,max(ny,nu),sp_no), plot(t,y(yplot_idx(i),:),'k','Linewidth',1.2); hold on; %ACM result in black
            if ~isempty(find(yplot_idx(i) == ym_idx))
                plot(t,ym(yplot_idx(i),:),'r','Linewidth',1.2);  %DRM result in red
            end
            xlabel(['t [' t_label ']']); ylabel([y_label{yplot_idx(i)} char(10) '[' y_units{yplot_idx(i)} ']']);
            xlim([t(1) t(n)]); grid on;
        end
        if (bplot_error)
          sp_no = max(ny,nu);
          for i = 1:ny
            sp_no = sp_no + 1;
            iy_drm = find(ym_idx == yplot_idx(i));
            if ~isempty(iy_drm)
                subplot(nrow_plot,max(ny,nu),sp_no), plot(t,1-ym(yplot_idx(i),:)./y(yplot_idx(i),:),'Linewidth',1.2);
                xlabel(['t [' t_label ']']); ylabel(['Normalized Error' char(10) y_label{yplot_idx(i)}]);
                xlim([t(1) t(n)]); grid on;
            end
          end
        end
        if (bplot_step_change)
          sp_no = (1+bplot_error)*max(ny,nu);
          for j = 1:nu
            sp_no = sp_no + 1;
            subplot(nrow_plot,max(ny,nu),sp_no), stairs(t,u(uplot_idx(j),:),'Linewidth',1.2);
            xlabel(['t [' t_label ']']); ylabel([u_label{uplot_idx(j)} char(10) '[' u_units{uplot_idx(j)} ']']);
            y_range = max(u(uplot_idx(j),:)) - min(u(uplot_idx(j),:));
            if y_range == 0
                y_range = abs(0.5*max(u(uplot_idx(j),:)));
            end
            xlim([t(1) t(n)]); grid on;
            ylim([min(u(uplot_idx(j),:))-0.05*y_range max(u(uplot_idx(j),:))+0.05*y_range]);
          end
        end
        if (bplot_correlation)
          sp_no = (1+bplot_error+bplot_step_change)*max(ny,nu);
          for i = 1: ny
            sp_no = sp_no + 1;
            iy_drm = find(ym_idx == yplot_idx(i));
            if ~isempty(iy_drm)
                subplot(nrow_plot,max(ny,nu),sp_no), plot(y(yplot_idx(i),:),ym(yplot_idx(i),:),'+');
                strlegend = sprintf('R^2=%g',R_squared(iy_drm));
                legend(strlegend);
                xlabel([y_label{yplot_idx(i)} ' (Plant)' char(10) '[' y_units{yplot_idx(i)} ']']);
                ylabel([y_label{yplot_idx(i)} ' (DRM)' char(10) '[' y_units{yplot_idx(i)} ']']);
            end
          end
        end

    case {'uq'}
        nk = size(u,2);  % u is from MATLAB engine
        uk = zeros(nu,1);  % nu is from DRMParameters.m script
        yk1 = zeros(ny,1);  % ny is from DRMParameters.m script
        %drm = DRM_DABNet(dt,input_names,output_names,input_indices,output_indices,nu,ny,u_mean,y_mean,u_sigma,y_sigma,A,B,NN);
        drm = DRM_DABNet(dt,A,B,NN,u_mean,y_mean,u_sigma,y_sigma,input_indices,output_indices,input_names,output_names);
        clear u_mean y_mean u_sigma y_sigma A B NN;
        % assign initial u as uk
        for i=1:nu
          uk(i) = u(input_indices(i),1);
        end
        drm.initialize(uk);
        ffcn = @funcNextState;
        hfcn = @funcStateToOutput;
        % iy includes all ny indices
        iy = zeros(ny,1);
        for i = 1:ny
          iy(i) = i;
        end
        nx = drm.getNumberOfStatesRelatedToOutputs(iy);
        icross = drm.getCrossIndices2(iy);
        x0 = drm.getAnnStatesRelatedToOutputs(iy);
        y0 = drm.evalRelatedOutputsFromAnnStates(iy);
        Q = diag((fq_state*x0).^2);	%fq_state is set by MATLAB engine
        R = diag((fr_output.*y0).^2);	%fr_output is a vector set by MATLAB engine
        sqrtR = chol(R);
        %allocate matrices
        M = zeros(nx,nk);
        P = zeros(nx,nx);  % does not save history of P
        MU = zeros(ny,nk);
        S = zeros(ny,ny,nk);
        y_ukf = zeros(ny,nk);
        y_obs = zeros(ny,nk);
        %assign initial values
        M(:,1) = drm.getAnnStatesRelatedToOutputs(iy);
        P0 = diag((0.001*M(:,1)).^2);  % hard-wired initial state covariance matrix
        P = P0;
        y_ukf(:,1) = y_drm(:,1);
        y_obs(:,1) = y_drm(:,1);
        S(:,:,1) = R;
        %iteration loop
        for k = 1:nk-1
            for i=1:nu
              uk(i) = u(input_indices(i),k);
            end
            Mk = M(:,k);
            Pk = P;
            %predict
            fparam = {iy, drm, uk};
            [M_, P_] = ukf_predict1(Mk, Pk, ffcn, Q, fparam);
            %update
            for i=1:ny
              yk1(i) = y_acm(output_indices(i),k+1);
            end
            yk1 = yk1 + sqrtR*randn(ny,1);
            y_obs(:,k+1) = yk1;
            hparam = {iy,drm};
            [Mk1, Pk1, K, MUk1, Sk1, LHk1] = ukf_update1(M_, P_, yk1, hfcn, R, hparam);
            M(:,k+1) = Mk1;
            %make Pk1 positive definite based on Brenda's method
            Pk1(find(icross==0)) = 0;
            P = Pk1;
            y_ukf(:,k+1) = MUk1;
            S(:,:,k+1) = Sk1;
        end
        y_std = zeros(ny,nk);
        for i = 1: ny
            for j = 1:nk
                y_std(i,j) = sqrt(S(i,i,j));
            end
        end
        y_lower = y_ukf - y_std;
        y_upper = y_ukf + y_std;
        %calculate R-squared of y_ukf
        r2_ukf = zeros(ny,1);
        for i=1:ny
            ybar = 0;
            for j=1:nk
              ybar = ybar + y_acm(output_indices(i),j);
            end
            ybar = ybar/nk;
            sse = 0;
            sst = 0;
            for j=1:nk
              delta_y = y_ukf(i,j) - y_acm(output_indices(i),j);
              sse = sse + delta_y*delta_y;
              delta_y = y_acm(output_indices(i),j) - ybar;
              sst = sst + delta_y*delta_y;
            end
            r2_ukf(i) = 1 - sse/sst;
        end
        
        %plot results with +/- sigma from S's diagonal terms
        %does not plot unvaried output variables
        t_label = ID_Par{4};  % time unit
        u_label = strsplit(ID_Par{5}, ' ');  % input names
        u_units = strsplit(ID_Par{6}, ' ');  % input units
        y_label = strsplit(ID_Par{9}, ' ');  % output names
        y_units = strsplit(ID_Par{10}, ' ');  % output units
        t = 0:dt:(nk-1)*dt;
        figure;
        ym_idx = ID_Par{11};  % output index in Simulink output variable list, different from yplot_idx
        uplot_idx = ID_Par{14};  % user slected input variable indices
        yplot_idx = ID_Par{15};  % user selected output variable indices, subset of ym_idx
        nu = length(uplot_idx);
        ny = length(yplot_idx);
        nrow_plot = 1 + bplot_error + bplot_step_change + bplot_correlation; %bplot_* are set by MATLAB engine
        %first row, always plot output response
        sp_no = 0;
        for i = 1:ny
            sp_no = sp_no + 1;
            iy_drm = find(ym_idx == yplot_idx(i));
            if ~isempty(iy_drm)
              subplot(nrow_plot,max(ny,nu),sp_no), plot(t,y_obs(iy_drm,:),'k','Linewidth',1.2); hold on;
              plot(t,y_ukf(iy_drm,:),'r','Linewidth',1.2);
              plot(t,y_lower(iy_drm,:),':b','Linewidth',1.2);
              plot(t,y_upper(iy_drm,:),':g','Linewidth',1.2);
              legend('Measured', 'UKF', 'UKF-STD', 'UKF+STD');
              xlabel(['t [' t_label ']']); ylabel([y_label{yplot_idx(i)} char(10) '[' y_units{yplot_idx(i)} ']']);
              xlim([t(1) t(nk)]); grid on;
            end
        end
        if (bplot_error)
          sp_no = max(ny,nu);
          for i = 1:ny
            sp_no = sp_no + 1;
            iy_drm = find(ym_idx == yplot_idx(i));
            if ~isempty(iy_drm)
              subplot(nrow_plot,max(ny,nu),sp_no), plot(t,1-y_drm(iy_drm,:)./y_acm(yplot_idx(i),:),'k','Linewidth',1.2); hold on;
              plot(t,1-y_ukf(iy_drm,:)./y_acm(yplot_idx(i),:),'r','Linewidth',1.2);
              legend('DRM', 'UKF');
              xlabel(['t [' t_label ']']); ylabel(['Normalized Error' char(10) y_label{yplot_idx(i)}]);
              xlim([t(1) t(nk)]); grid on;
            end
          end
        end
        if (bplot_step_change)
          sp_no = (1+bplot_error)*max(ny,nu);
          for j = 1:nu
            sp_no = sp_no + 1;
            subplot(nrow_plot,max(ny,nu),sp_no), stairs(t,u(uplot_idx(j),:),'Linewidth',1.2);
            xlabel(['t [' t_label ']']); ylabel([u_label{uplot_idx(j)} char(10) '[' u_units{uplot_idx(j)} ']']);
            y_range = max(u(uplot_idx(j),:)) - min(u(uplot_idx(j),:));
            if y_range == 0
                y_range = abs(0.5*max(u(uplot_idx(j),:)));
            end
            xlim([t(1) t(nk)]); grid on;
            ylim([min(u(uplot_idx(j),:))-0.05*y_range max(u(uplot_idx(j),:))+0.05*y_range]);
          end
        end
        if (bplot_correlation)
          sp_no = (1+bplot_error+bplot_step_change)*max(ny,nu);
          for i = 1: ny
            sp_no = sp_no + 1;
            iy_drm = find(ym_idx == yplot_idx(i));
            if ~isempty(iy_drm)
                subplot(nrow_plot,max(ny,nu),sp_no), plot(y_acm(yplot_idx(i),:),y_drm(iy_drm,:),'+k'); hold on;
                plot(y_acm(yplot_idx(i),:),y_ukf(iy_drm,:),'+r');
                strlegend_drm = sprintf('R^2=%g (DRM)',r2_drm(iy_drm));
                strlegend_ukf = sprintf('R^2=%g (UKF)',r2_ukf(iy_drm));
                legend(strlegend_drm, strlegend_ukf);
                xlabel([y_label{yplot_idx(i)} ' (Plant)' char(10) '[' y_units{yplot_idx(i)} ']']);
                ylabel([y_label{yplot_idx(i)} ' (DRM/UKF)' char(10) '[' y_units{yplot_idx(i)} ']']);
            end
          end
        end
        
    otherwise
        disp('Unknown scenario (Simulation/Plot) provided. Please check "Command_Parameters.txt" file');
        return
end
