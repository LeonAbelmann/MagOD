function update_plot_gamma(handles, newplot)

% If button was pressed, create a new graph
if newplot
    handles.settings.fig_gamma = figure();
    guidata(handles.figure1, handles);
% If no plot exists, return without doing anything
elseif isempty(handles.settings.fig_gamma) || ~ishandle(handles.settings.fig_gamma)
    return;
end

% Obtain and clear figure
fig_handle = handles.settings.fig_gamma;
figure(fig_handle);
clf;

% Decide what to plot on the x-axis
if strcmp(handles.settings.time_scale, 'days')
    x_data = handles.data.cycles.time_gamma_day;
    xlab = 'Time [days]';
elseif strcmp(handles.settings.time_scale, 'hours')
    x_data = handles.data.cycles.time_gamma_hr;
    xlab = 'Time [hrs]';
elseif strcmp(handles.settings.time_scale, 'minutes')
    x_data = handles.data.cycles.time_gamma_min;
    xlab = 'Time [min]';
elseif strcmp(handles.settings.time_scale, 'seconds')
    x_data = handles.data.cycles.time_gamma_sec;
    xlab = 'Time [sec]';
else
    error('Wrong time...');
end

% If plotted with filter
if handles.settings.gamma_plot_filter_enable
    
    % Determine plot colors
    color_BG = [0.8, 0.8, 0.8];
    
    % Plot tau
    
    % Plot unfiltered data
    subplot(1,2,1);
    plot(x_data, handles.data.cycles.tau, 'Color', color_BG);
    hold on;
    
    % Plot filtered data
    plot(x_data, handles.data.cycles.tauF, 'k');
       
    xlabel(xlab);
    ylabel('\tau [s]');
    xlim([0, max(x_data)]);
    
    % Plot gamma

    % Plot unfiltered data
    subplot(1,2,2);
    plot(x_data, handles.data.cycles.gamma, 'Color', color_BG);
    hold on;
    
    % Plot filtered data
    plot(x_data, handles.data.cycles.gammaF, 'k');
       
    xlabel(xlab);
    ylabel('\gamma [rad/mT/sec]');
    xlim([0, max(x_data)]);
    
    legend('Raw', 'Filtered (gaussian)');
    
else
    
    % Plot tau
    subplot(1,2,1);
    plot(x_data, handles.data.cycles.tau);  
    xlabel(xlab);
    ylabel('\tau [s]');
    xlim([0, max(x_data)]);
    
    % Plot gamma
    subplot(1,2,2);
    plot(x_data, handles.data.cycles.gamma);
    hold on;
    xlabel(xlab);
    ylabel('\gamma [rad/mT/sec]');
    xlim([0, max(x_data)]);
    
end
    
message_append(handles, 'Updated figure gamma.')