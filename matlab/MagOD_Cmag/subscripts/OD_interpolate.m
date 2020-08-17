function [time_sec_int, OD_int, Vav_int, Vref_int, Vscat_int, Temp_int, LED_int, prog_int] = ...
    OD_interpolate(time_sec, OD, Vav, Vref, Vscat, Temp, LED, prog)

% Fixed time steps for interpolation
min_time = time_sec(1);
max_time = time_sec(end);
time_step = (max_time-min_time)/length(time_sec);
time_sec_int = [min_time:time_step:max_time]'; % Prime is intentional

% Interpolate OD
OD_int = interp1(time_sec, OD, time_sec_int);

% Interpolate Vav
Vav_int = interp1(time_sec, Vav, time_sec_int);

% Interpolate Vref
Vref_int = interp1(time_sec, Vref, time_sec_int);

% Interpolate Vscat
Vscat_int = interp1(time_sec, Vscat, time_sec_int);

% Interpolate Temp
Temp_int = interp1(time_sec, Temp, time_sec_int);

% Interpolate LED
LED_int = interp1(time_sec, LED, time_sec_int, 'nearest'); % Only integers

% Interpolate program
prog_int = interp1(time_sec, prog, time_sec_int, 'nearest');
