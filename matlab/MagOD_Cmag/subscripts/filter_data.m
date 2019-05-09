function struct = filter_data(handles)
% Rewritten by Leon. We now filter on the data that is in handles. So with
% lead and trail removed

% Copy all settings from data to dataF(iltered)
struct = handles;
struct.dataF = handles.data;

% Make local references
bac_data=handles.data.all;
bac_dataF=struct.dataF.all;

% Interpolate data
[bac_dataF.time_sec, bac_dataF.OD, bac_dataF.Vav, bac_dataF.prog] = OD_interpolate(bac_data.time_sec, bac_data.OD, bac_data.Vav, bac_data.prog);

% Filter data (filter out frequencies lower than the cycle period)
freq_cutoff = 1/handles.data.cycles.duration_sec * 0.95;
[bac_dataF.OD, ~, ~]  = filter_rect_padding(bac_dataF.OD, bac_dataF.time_sec, freq_cutoff, 'highpass', 1, 0);
[bac_dataF.Vav, ~, ~] = filter_rect_padding(bac_dataF.Vav, bac_dataF.time_sec, freq_cutoff, 'highpass', 1, 0);

% Write filtered data into handles
%struct.dataF.all=bac_dataF;

% Break filtered data into cycles and segments
%handles.dataF = load_and_break_data(bac_dataF, handles.data);
