function struct = filter_data(handles)
% Rewritten by Leon. We now filter on the data that is in handles. So with
% lead and trail removed

% Copy all settings from data to dataF(iltered)
struct = handles;
%struct.dataF = handles.data; % Debug: don't copy. If ok, rename struct
% into handles later

% Make local references
bac_data=handles.data.all;
%bac_dataF=struct.dataF.all;

% Interpolate data (akward, can be done better I suppose)
[bac_dataF.time_sec, bac_dataF.OD, bac_dataF.Vav, bac_dataF.Vref, ...
    bac_dataF.Vscat, bac_dataF.Temp, bac_dataF.LED, bac_dataF.prog] = ...
    OD_interpolate(bac_data.time_sec, bac_data.OD, bac_data.Vav, bac_data.Vref, ...
    bac_data.Vscat, bac_data.Temp, bac_data.LED, bac_data.prog);
bac_dataF.time_min=bac_dataF.time_sec/60;
bac_dataF.time_hr =bac_dataF.time_min/60;
bac_dataF.time_day=bac_dataF.time_hr/24;

% Filter data (filter out frequencies lower than the cycle period)
freq_cutoff = 1/handles.data.cycles.duration_sec * 0.95;
[bac_dataF.OD, ~, ~]  = filter_rect_padding(bac_dataF.OD, bac_dataF.time_sec, freq_cutoff, 'highpass', 1, 0);
[bac_dataF.Vav, ~, ~] = filter_rect_padding(bac_dataF.Vav, bac_dataF.time_sec, freq_cutoff, 'highpass', 1, 0);
bac_dataF.Vup = 0* bac_dataF.Vav;%For backward compatiblity
bac_dataF.Vdwn = 0* bac_dataF.Vav;%For backward compatiblity


% Write filtered data into handles
struct.dataF.all.OD        =bac_dataF.OD;
struct.dataF.all.Vav       =bac_dataF.Vav;
struct.dataF.all.time_sec = bac_dataF.time_sec;
struct.dataF.all.time_min = bac_dataF.time_min;
struct.dataF.all.time_hr  = bac_dataF.time_hr;
struct.dataF.all.time_day = bac_dataF.time_day;
struct.dataF.all.prog     = bac_dataF.prog;

% Break filtered data into cycles and segments
bac_dataF.t_sec = bac_dataF.time_sec;
bac_dataF.ODoriginal=bac_dataF.OD; % no idea why we need ODoriginal...
struct.dataF = load_and_break_data(bac_dataF, handles.data);
