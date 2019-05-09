function [tau, gamma, time_sec, fit_X, fit_Y] = fit_gamma(handles, cycle_nrs)

% Pre-declaration
tau = zeros(1, length(cycle_nrs));
time_sec = zeros(1, length(cycle_nrs));
fit_X = cell(1, length(cycle_nrs));
fit_Y = cell(1, length(cycle_nrs));

% Suppress output of fitting
opts = optimset('Display', 'off');

for cycle_ind = 1:length(cycle_nrs)
    
    if rem(cycle_ind, 10)==0
        message_new(handles, ['Fitting ' num2str(cycle_ind) '/' num2str(length(cycle_nrs)), '.']);
    end
    
    cur_cycle = cycle_nrs(cycle_ind);
    
    % Get the desired segment
    seg_nr = (cur_cycle - 1) * handles.data.all.maxprog + handles.settings.gamma_segment;
    seg_nr = min(seg_nr,handles.data.segs.number-1);%Don't attempt to fit the last segment
    [seg_time, seg_OD] = get_segmentF(handles, seg_nr);
    
    % Determine fitting function
    if strcmp(handles.settings.fitting_function, 'gexpcot')
        if strcmp(handles.settings.slope_type, 'up')
            fit_func = @ODstep_cot_rise;
        else
            fit_func = @ODstep_cot_fall;
        end
    elseif strcmp(handles.settings.fitting_function, 'expcot')
        if strcmp(handles.settings.slope_type, 'up')
            fit_func = @ODstep_cot_rise_noG;
        else
            fit_func = @ODstep_cot_fall_noG;
        end
    elseif strcmp(handles.settings.fitting_function, 'exp')
        if strcmp(handles.settings.slope_type, 'up')
            fit_func = @ODstep_rise;
        else
            fit_func = @ODstep_fall;
        end
    else
        error('Invalid fitting function specified');
    end
    
    % Determine starting condition
    delay = seg_time(1);
    est_tau = 1;
    amplitude = max(seg_OD)-min(seg_OD);
    offset = min(seg_OD);
    p0 = [delay, est_tau, amplitude, offset];
    lowerbound = [delay, 0, -Inf, -Inf]; upperbound = [delay, Inf, Inf, Inf];
    
    % Perform fitting
    pfit = lsqcurvefit(fit_func, p0 , seg_time(:), seg_OD(:), lowerbound, upperbound, opts);
    tau(cycle_ind) = pfit(2);
    
    % Store fitted result
    fit_X{cycle_ind} = seg_time;
    fit_Y{cycle_ind} = fit_func(pfit, seg_time);
    
    % Time of cycle start is recorded as the tau-moment
    time_sec(cycle_ind) = seg_time(1);
    
end

gamma = 1./(pi*tau*handles.settings.B_mT);