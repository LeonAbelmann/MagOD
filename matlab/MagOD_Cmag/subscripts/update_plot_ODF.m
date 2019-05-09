function update_plot_ODF(handles, newplot)

% If button was pressed, create a new graph
if newplot
    handles.settings.fig_ODF = figure();
    guidata(handles.figure1, handles);
% If no plot exists, return without doing anything
elseif isempty(handles.settings.fig_ODF) || ~ishandle(handles.settings.fig_ODF)
    return;
end

% Obtain and clear figure
fig_handle = handles.settings.fig_ODF;
figure(fig_handle);
clf;

ODplot(handles.dataF,handles.settings);

title('Filtered data');
message_append(handles, 'Updated figure filtered data.')