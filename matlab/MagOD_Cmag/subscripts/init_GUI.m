function init_GUI(handles)

% Deactivate all GUI elements
deactivate_GUI(handles);

% Activate only the 'Load dataset' button
set(handles.OD_push_load, 'Enable', 'on');

% Create settings file
handles.settings.offset = 0;
handles.settings.offset_enable = 0;
handles.settings.time_scale = 'days';
handles.settings.cycle_min = 1;
handles.settings.cycle_max = 1;
handles.settings.cycle_steps = [1, 1];
handles.settings.cycle_nr_cur = 1;
handles.settings.segment_up = 1;
handles.settings.segment_down = 2;
handles.settings.duration_averaging_sec = 1;
handles.settings.cmag_calc_period = 1;
handles.settings.cmag_plot_filter_enable = 0;
handles.settings.cmag_plot_filter_std = 1;
handles.settings.gamma_segment = 1;
handles.settings.gamma_calc_period = 1;
handles.settings.B_mT = 1;
handles.settings.fitting_function = 'gexpcot';
handles.settings.slope_type = 'down';
handles.settings.gamma_plot_filter_enable = 0;
handles.settings.gamma_plot_filter_std = 1;

% Save settings in handles
guidata(handles.figure1, handles);