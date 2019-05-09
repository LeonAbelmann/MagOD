function save_data(handles)

% Create new folder for saving
cur_timestamp = datestr(clock);
dirname = [handles.settings.source_folder '/results_' cur_timestamp];
mkdir(dirname);

message_new(handles, ['Saving data in ' dirname]);
message_append(handles, 'Saving images...');

% Save all plotted data in PDF's
try_save_figure(handles.settings.fig_OD, dirname, 'figure_OD');
try_save_figure(handles.settings.fig_ODF, dirname, 'figure_OD_filtered');
try_save_figure(handles.settings.fig_cycle, dirname, 'figure_cycle');
try_save_figure(handles.settings.fig_cmag, dirname, 'figure_cmag');
try_save_figure(handles.settings.fig_gamma, dirname, 'figure_gamma');

% Message
message_append(handles, 'Saving Cmag data...');

% Save Cmag data
header = ['Time[sec],Time[min],Time[hr],Time[day],Cmag,Cmag_filtered,Dmag,Dmag_filtered'];
data = [    handles.data.cycles.time_cmagdmag_sec(:), ...
            handles.data.cycles.time_cmagdmag_min(:), ...
            handles.data.cycles.time_cmagdmag_hr(:), ...
            handles.data.cycles.time_cmagdmag_day(:), ...
            handles.data.cycles.cmag(:), ...
            handles.data.cycles.cmagF(:), ...
            handles.data.cycles.dmag(:), ...
            handles.data.cycles.dmagF(:)];
data2csv(header, data, [dirname, '/cmag_dmag.csv']);

% Message
message_append(handles, 'Saving gamma data...');

% Save gamma data
header = ['Time[sec],Time[min],Time[hr],Time[day],tau[s],tau_filtered[s],gamma[rad/mT/s],gamma_filtered[rad/mT/s]'];
data = [    handles.data.cycles.time_gamma_sec(:), ...
            handles.data.cycles.time_gamma_min(:), ...
            handles.data.cycles.time_gamma_hr(:), ...
            handles.data.cycles.time_gamma_day(:), ...
            handles.data.cycles.tau(:), ...
            handles.data.cycles.tauF(:), ...
            handles.data.cycles.gamma(:), ...
            handles.data.cycles.gammaF(:)];
data2csv(header, data, [dirname, '/gamma.csv']);

% Message
message_append(handles, 'Done.');