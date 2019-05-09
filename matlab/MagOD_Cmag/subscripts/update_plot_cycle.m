function update_plot_cycle(handles, newplot)

% If button was pressed, create a new graph
if newplot
    handles.settings.fig_cycle = figure();
    guidata(handles.figure1, handles);
% If no plot exists, return without doing anything
elseif isempty(handles.settings.fig_cycle) || ~ishandle(handles.settings.fig_cycle)
    return;
end

% Obtain and clear figure
fig_handle = handles.settings.fig_cycle;
figure(fig_handle);
clf;

% Define time offset
TO = handles.data.cycle.time_sec(1);
TOF = handles.dataF.cycle.time_sec(1);

switch handles.data.cycle.LED(1) % for a cycle the color of the led must be all the same, so first one is ok
    case 1 
        linecolor = handles.settings.colors.red;
    case 2
        linecolor = handles.settings.colors.green;
    case 3
        linecolor = handles.settings.colors.blue;
end
% Plot unfiltered results
subplot(2,1,1);
t_up=handles.data.cycle.time_up-TO;
t_down=handles.data.cycle.time_down-TO;
plot(handles.data.cycle.time_sec-TO, handles.data.cycle.OD,'Color',linecolor); hold on;
ylimits=ylim; % Remember the y-scale (why is this the yscale of the second plot?)
plot(t_up, repmat(handles.data.cycle.OD_up, 2, 1), 'k', 'LineWidth', 2);
plot(t_down, repmat(handles.data.cycle.OD_down, 2, 1), '--k','LineWidth', 2);
% Get the start time of every segment
segment_starttimes=getsegmen_times(handles,handles.settings.cycle_nr_cur)-TO;
% Plot a  vertical line at every segment start (except the first, since it
% is not visible)
for i=2:length(segment_starttimes)
    starttime=segment_starttimes(i);
    plot([starttime starttime],[ylimits(1)  ylimits(2)],'k');
end
xlabel('Time [sec]');
ylabel('OD');
title(['Raw at ' num2str(handles.data.cycle.time_start,'%15.0f') ' s / Cmag=' num2str(handles.data.cycle.cmag) ' / Dmag=' num2str(handles.data.cycle.dmag)]);
legend('Data', 'Upper OD', 'Lower OD');
xlim([min(handles.data.cycle.time_sec-TO), max(handles.data.cycle.time_sec-TO)]);
ylim([ylimits(1)  ylimits(2)]);% force both plot to have the same y-axis


% Plot filtered results
subplot(2,1,2);
plot(handles.dataF.cycle.time_sec-TOF, handles.dataF.cycle.OD,'Color',linecolor); hold on;
plot(handles.dataF.cycle.fit_X{1}-TOF, handles.dataF.cycle.fit_Y{1}, 'k','LineWidth', 2);
xlabel('Time [sec]');
ylabel('OD');
title(['OD cycle filtered / \tau=' num2str(handles.dataF.cycle.tau) ' / \gamma=' num2str(handles.dataF.cycle.gamma)]);
legend('Data', 'Fitted function');
ylim([ylimits(1)  ylimits(2)]);% force both plot to have the same y-axis
xlim([min(handles.dataF.cycle.time_sec-TOF), max(handles.dataF.cycle.time_sec-TOF)])

message_append(handles, 'Updated figure cycle.')