function calculate_gamma_batch(handles)

cycle_nrs = 1:handles.settings.gamma_calc_period:handles.settings.cycle_max;

message_new(handles, 'Calculating gamma.');

% Calculate gamma
[   handles.data.cycles.tau, ...
    handles.data.cycles.gamma, ...
    handles.data.cycles.time_gamma_sec] ...
    = fit_gamma(handles, cycle_nrs);
    
% Calculate time axes
handles.data.cycles.time_gamma_min = handles.data.cycles.time_gamma_sec/60;
handles.data.cycles.time_gamma_hr = handles.data.cycles.time_gamma_min/60;
handles.data.cycles.time_gamma_day = handles.data.cycles.time_gamma_hr/24;

% Save data
guidata(handles.figure1, handles);

% Calculate filtered versions
calculate_gamma_filtered(handles);

% Update data
handles = guidata(handles.figure1);
    
% Update message to indicate finished
message_append(handles, 'Done.');

% Updat Cmag plot
update_plot_gamma(handles, 0);