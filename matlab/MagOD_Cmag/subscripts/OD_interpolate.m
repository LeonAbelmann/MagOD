function [time_sec_int, OD_int, Vav_int, prog_int] = OD_interpolate(time_sec, OD, Vav, prog)

% Fixed time steps for interpolation
min_time = time_sec(1);
max_time = time_sec(end);
time_step = (max_time-min_time)/length(time_sec);
time_sec_int = [min_time:time_step:max_time]';

% Interpolate OD
OD_int = interp1(time_sec, OD, time_sec_int);

% Interpolate Vav
Vav_int = interp1(time_sec, Vav, time_sec_int);

% Interpolate program
prog_int = interp1(time_sec, prog, time_sec_int, 'nearest');