function get_cycle(handles, cycle_nr)

% For raw data
ind_start = handles.data.cycles.ind_start(cycle_nr);
ind_stop =  handles.data.cycles.ind_end(cycle_nr);
handles.data.cycle.time_start = handles.data.all.time_sec(ind_start);
handles.data.cycle.OD =        handles.data.all.OD(ind_start:ind_stop);
handles.data.cycle.time_sec =  handles.data.all.time_sec(ind_start:ind_stop);
handles.data.cycle.prog =      handles.data.all.prog(ind_start:ind_stop);
handles.data.cycle.LED =       handles.data.all.LED(ind_start:ind_stop);

% For filtered data
ind_start = handles.dataF.cycles.ind_start(cycle_nr);
ind_stop =  handles.dataF.cycles.ind_end(cycle_nr);
handles.dataF.cycle.OD =        handles.dataF.all.OD(ind_start:ind_stop);
handles.dataF.cycle.time_sec =  handles.dataF.all.time_sec(ind_start:ind_stop);
handles.dataF.cycle.prog =      handles.dataF.all.prog(ind_start:ind_stop);
handles.dataF.cycle.LED =       handles.dataF.all.LED(ind_start:ind_stop);

% Save data
guidata(handles.figure1, handles);