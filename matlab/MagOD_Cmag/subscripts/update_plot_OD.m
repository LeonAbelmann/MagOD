function update_plot_OD(handles, newplot)

% If button was pressed, create a new graph
if newplot
    handles.settings.fig_OD = figure();
    guidata(handles.figure1, handles);
% If no plot exists, return without doing anything
elseif isempty(handles.settings.fig_OD) || ~ishandle(handles.settings.fig_OD)
    return;
end

% Obtain and clear figure
fig_handle = handles.settings.fig_OD;
figure(fig_handle);
clf;

ODplot(handles.data,handles.settings);

title('Raw data');

message_append(handles, 'Updated figure raw data.')