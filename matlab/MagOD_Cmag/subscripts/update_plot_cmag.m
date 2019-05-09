function update_plot_cmag(handles, newplot)

% If button was pressed, create a new graph
if newplot
    handles.settings.fig_cmag = figure();
    guidata(handles.figure1, handles);
% If no plot exists, return without doing anything
elseif isempty(handles.settings.fig_cmag) || ~ishandle(handles.settings.fig_cmag)
    return;
end

% Obtain and clear figure
fig_handle = handles.settings.fig_cmag;
figure(fig_handle);
clf;

% Decide what to plot on the x-axis
if strcmp(handles.settings.time_scale, 'days')
    x_data = handles.data.cycles.time_cmagdmag_day;
    xlab = 'Time [days]';
elseif strcmp(handles.settings.time_scale, 'hours')
    x_data = handles.data.cycles.time_cmagdmag_hr;
    xlab = 'Time [hrs]';
elseif strcmp(handles.settings.time_scale, 'minutes')
    x_data = handles.data.cycles.time_cmagdmag_min;
    xlab = 'Time [min]';
elseif strcmp(handles.settings.time_scale, 'seconds')
    x_data = handles.data.cycles.time_cmagdmag_sec;
    xlab = 'Time [sec]';
else
    error('Wrong time...');
end

% If plotted with filter
if handles.settings.cmag_plot_filter_enable
    
    % Determine plot colors
    color_BG = [0.8, 0.8, 0.8];
    
    % Plot Cmag
    
    % Plot unfiltered data
    subplot(1,2,1);
    plot(x_data, handles.data.cycles.cmag, 'Color', color_BG);
    hold on;
    
    % Plot filtered data
    plot(x_data, handles.data.cycles.cmagF, 'k');
       
    xlabel(xlab);
    ylabel('Cmag');
    xlim([0, max(x_data)]);
    
    % Plot Dmag

    % Plot unfiltered data
    subplot(1,2,2);
    plot(x_data, handles.data.cycles.dmag, 'Color', color_BG);
    hold on;
    
    % Plot filtered data
    plot(x_data, handles.data.cycles.dmagF, 'k');
       
    xlabel(xlab);
    ylabel('Dmag');
    xlim([0, max(x_data)]);
    
    legend('Raw', 'Filtered (gaussian)');
    
else
    
    % Plot Cmag
    subplot(1,2,1);
    plot(x_data, handles.data.cycles.cmag);  
    xlabel(xlab);
    ylabel('Cmag');
    xlim([0, max(x_data)]);
    
    % Plot Dmag
    subplot(1,2,2);
    plot(x_data, handles.data.cycles.dmag);
    hold on;
    xlabel(xlab);
    ylabel('Dmag');
    xlim([0, max(x_data)]);
    
end
    
message_append(handles, 'Updated figure for Cmag and Dmag.')