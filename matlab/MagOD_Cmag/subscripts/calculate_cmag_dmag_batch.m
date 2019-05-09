function calculate_cmag_dmag_batch(handles)

cycle_nrs = 1:handles.settings.cmag_calc_period:handles.settings.cycle_max;

message_new(handles, 'Calculating Cmag and Dmag.');

% Calculate Cmag and Dmag
[~, ~, ~, ~, ...
    handles.data.cycles.cmag, ...
    handles.data.cycles.dmag, ...
    handles.data.cycles.time_cmagdmag_sec] = ...
        calculate_cmag_dmag(handles, cycle_nrs);
    
% Calculate time axes
handles.data.cycles.time_cmagdmag_min = handles.data.cycles.time_cmagdmag_sec/60;
handles.data.cycles.time_cmagdmag_hr = handles.data.cycles.time_cmagdmag_min/60;
handles.data.cycles.time_cmagdmag_day = handles.data.cycles.time_cmagdmag_hr/24;

% Save data
guidata(handles.figure1, handles);

% Calculate filtered versions
calculate_cmag_dmag_filtered(handles);

% Update data
handles = guidata(handles.figure1);
    
% Update message to indicate finished
message_append(handles, 'Done.');

% Updat Cmag plot
update_plot_cmag(handles, 0);