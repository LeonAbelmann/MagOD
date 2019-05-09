function data = load_and_break_data(bac_data, data)

% Load data
data.all.OD = bac_data.OD;
data.all.ODoriginal = bac_data.ODoriginal;
data.all.Vup = bac_data.Vup; 
data.all.Vdwn = bac_data.Vdwn;
data.all.Vav  = bac_data.Vav; 
data.all.Vref = bac_data.Vref;
data.all.Vscat = bac_data.Vscat; 
data.all.Temp = bac_data.Temp;
data.all.LED = bac_data.LED;
data.all.time_sec = bac_data.t_sec;
data.all.time_min = data.all.time_sec/60;
data.all.time_hr = data.all.time_min/60;
data.all.time_day = data.all.time_hr/24;
data.all.prog = bac_data.prog; % program number (0,1,2,etc)
data.all.maxprog = max(bac_data.prog)+1;

% Break into cycles, find the first occurance of program "0"
is_cycle_start =    data.all.prog(2:end) - data.all.prog(1:end-1) ~= 0 & ...
                    data.all.prog(2:end) == 0;
is_cycle_start =    [0; is_cycle_start];

% Throw away lead and trail
% TODO. If the measurement is too short, this leads to an error since you throw away everything. Leon
% first_cycle_ind = find(is_cycle_start, 1, 'first');
% last_cycle_ind = find(is_cycle_start, 1, 'last') - 1;
first_cycle_ind = 1;
last_cycle_ind = find(is_cycle_start, 1, 'last');

ind_cut = first_cycle_ind:last_cycle_ind;
data.all.OD = data.all.OD(ind_cut);
data.all.ODoriginal = data.all.ODoriginal(ind_cut);
data.all.Vup = data.all.Vup(ind_cut); 
data.all.Vdwn = data.all.Vdwn(ind_cut);
data.all.Vav  = data.all.Vav(ind_cut); 
data.all.Vref = data.all.Vref(ind_cut);
data.all.Vscat = data.all.Vscat(ind_cut); 
data.all.Temp = data.all.Temp(ind_cut);
data.all.LED = data.all.LED(ind_cut);
data.all.time_sec = data.all.time_sec(ind_cut);
data.all.time_min = data.all.time_sec / 60;
data.all.time_hr = data.all.time_min / 60;
data.all.time_day = data.all.time_hr / 24;
data.all.prog = data.all.prog(ind_cut);
is_cycle_start = is_cycle_start(ind_cut);

% Save cycle variables
data.cycles.ind_start = vertcat(1,find(is_cycle_start));% Also use the first measurement
data.cycles.ind_end = [data.cycles.ind_start(2:end)-1; length(data.all.OD)];
data.cycles.number = length(data.cycles.ind_start);
data.cycles.duration_sec = data.all.time_sec(data.cycles.ind_start(2))- data.all.time_sec(1);%Take duration of first cycle

% Divide into segments
is_prog_change = vertcat(0,data.all.prog(2:end) - data.all.prog(1:end-1) ~= 0);
data.segs.ind_start = find(is_prog_change);
data.segs.ind_start = [1; data.segs.ind_start];
data.segs.ind_end = [data.segs.ind_start(2:end)-1; length(data.all.OD)];
data.segs.prog = data.all.prog(data.segs.ind_start);
data.segs.number = length(data.segs.ind_start);
