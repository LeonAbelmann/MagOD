function [OD_up, OD_down, time_up, time_down, cmag, dmag, time_cmagdmag] = ...
    calculate_cmag_dmag(handles, cycle_nrs)

% Pre-declaration
OD_up = zeros(1, length(cycle_nrs));
OD_down = zeros(1, length(cycle_nrs));
time_up = zeros(2, length(cycle_nrs));
time_down = zeros(2, length(cycle_nrs));

for cycle_ind = 1:length(cycle_nrs)
    
    cur_cycle = cycle_nrs(cycle_ind);
    
    % Get top OD
    seg_up_nr = (cur_cycle - 1) * handles.data.all.maxprog + handles.settings.segment_up;
    seg_up_nr = min(seg_up_nr,handles.data.segs.number);
    [seg_up_time, seg_up_OD] = get_segment(handles, seg_up_nr);
    seg_up_ind = seg_up_time >= seg_up_time(end) - handles.settings.duration_averaging_sec;
    OD_up(cycle_ind) = mean(seg_up_OD(seg_up_ind));
    
    % Get timing top OD
    time_up(1,cycle_ind) = min(seg_up_time(seg_up_ind));
    time_up(2,cycle_ind) = max(seg_up_time(seg_up_ind));
    
    % Get bottom OD
    seg_down_nr = (cur_cycle - 1) * handles.data.all.maxprog + handles.settings.segment_down;
    seg_down_nr = min(seg_down_nr,handles.data.segs.number);
    [seg_down_time, seg_down_OD] = get_segment(handles, seg_down_nr);
    seg_down_ind = seg_down_time >= seg_down_time(end) - handles.settings.duration_averaging_sec;
    OD_down(cycle_ind) = mean(seg_down_OD(seg_down_ind));
    
    % Calculate upper 
    time_down(1,cycle_ind) = min(seg_down_time(seg_down_ind));
    time_down(2,cycle_ind) = max(seg_down_time(seg_down_ind));
    
end

% Calculate cmag and dmag
cmag = OD_up./OD_down;
dmag = OD_up-OD_down;
    
% Time of the cmag and dmag is the mean of the first time indices
time_cmagdmag = time_up(1,:)/2 + time_down(1,:)/2;