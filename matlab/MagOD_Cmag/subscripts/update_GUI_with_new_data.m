function update_GUI_with_new_data(handles)

% Update name
filename=handles.settings.source_folder;
filename=filename(max(1,length(filename)-20): length(filename));% Limit to last 20 characters
set(handles.OD_text_name, 'String', filename);

% Update offset
set(handles.OD_check_offset, 'Value', handles.settings.offset_enable);
set(handles.OD_edit_offset_red, 'String', num2str(handles.settings.offsetred));
set(handles.OD_edit_offset_green, 'String', num2str(handles.settings.offsetgreen));
set(handles.OD_edit_offset_blue, 'String', num2str(handles.settings.offsetblue));

% Update radio buttons time
if strcmp(handles.settings.time_scale, 'days')
    set(handles.OD_radio_days, 'Value', 1);
elseif strcmp(handles.settings.time_scale, 'hours')
    set(handles.OD_radio_hours, 'Value', 1);
elseif strcmp(handles.settings.time_scale, 'minutes')
    set(handles.OD_radio_minutes, 'Value', 1);
elseif strcmp(handles.settings.time_scale, 'seconds')
    set(handles.OD_radio_seconds, 'Value', 1);
else
    error('Invalid value radio buttons x-axis!!!');
end    

% Update cycle bar
set(handles.cycle_slider, 'Min', handles.settings.cycle_min);
set(handles.cycle_slider, 'Max', handles.settings.cycle_max);
set(handles.cycle_slider, 'Value', handles.settings.cycle_nr_cur);
set(handles.cycle_slider, 'SliderStep', handles.settings.cycle_steps);
set(handles.cycle_text_slider, 'String', ...
    ['Cycle ' num2str(handles.settings.cycle_nr_cur) '/' num2str(handles.settings.cycle_max)]);

% Update segment details cycle
set(handles.cmag_popup_up, 'String', [1:handles.data.all.maxprog]');
set(handles.cmag_popup_down, 'String', [1:handles.data.all.maxprog]');
set(handles.cmag_popup_up, 'Value', handles.settings.segment_up);
set(handles.cmag_popup_down, 'Value', handles.settings.segment_down);

% Update edit fields cmag
set(handles.cmag_edit_time, 'String', num2str(handles.settings.duration_averaging_sec));
set(handles.cmag_edit_cyclefreq, 'String', num2str(handles.settings.cmag_calc_period));

% Update filter fields
set(handles.cmag_check_filter, 'Value', handles.settings.cmag_plot_filter_enable);
set(handles.cmag_edit_std, 'String', num2str(handles.settings.cmag_plot_filter_std));

% Update segment details gamma
set(handles.gamma_popup_segnr, 'String', [1:handles.data.all.maxprog]');
set(handles.gamma_popup_segnr, 'Value', handles.settings.gamma_segment);

% Update fitting function radio buttons
if strcmp(handles.settings.fitting_function, 'gexpcot')
    set(handles.gamma_radio_gexpcot, 'Value', 1);
elseif strcmp(handles.settings.fitting_function, 'expcot')
    set(handles.gamma_radio_expcot, 'Value', 1);
elseif strcmp(handles.settings.fitting_function, 'exp')
    set(handles.gamma_radio_exp, 'Value', 1);
else
    error('Invalid value radio buttons fitting function!!!');
end   

% Update slope up down radio buttons
if strcmp(handles.settings.slope_type, 'up')
    set(handles.gamma_radio_up, 'Value', 1);
elseif strcmp(handles.settings.slope_type, 'down')
    set(handles.gamma_radio_down, 'Value', 1);
end

% Update magnetic field settings
set(handles.gamma_edit_B, 'Value', str2double(handles.settings.B_mT));

% Update filter info stuff
set(handles.gamma_check_filter, 'Value', handles.settings.gamma_plot_filter_enable);
set(handles.gamma_edit_std, 'String', num2str(handles.settings.gamma_plot_filter_std));