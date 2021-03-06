function adjust_cycle_data(handles, cycle_nr)

% Get cycle data
get_cycle(handles, cycle_nr);
handles = guidata(handles.figure1);

% Calculate Cmag and Dmag
[   handles.data.cycle.OD_up, ...
    handles.data.cycle.OD_down, ...
    handles.data.cycle.time_up, ...
    handles.data.cycle.time_down, ...
    handles.data.cycle.cmag, ...
    handles.data.cycle.dmag] = ...
        calculate_cmag_dmag(handles, cycle_nr);
guidata(handles.figure1, handles);

% Calculate fit
[   handles.dataF.cycle.tau, ...
    handles.dataF.cycle.gamma, ...
    ~, ...
    handles.dataF.cycle.fit_X, ...
    handles.dataF.cycle.fit_Y] = fit_gamma(handles, cycle_nr);
guidata(handles.figure1, handles);

% Plot results
update_plot_cycle(handles, 0);