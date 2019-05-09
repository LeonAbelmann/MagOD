function OD = ODstep_cot_rise(param, t)

delay = param(1);
tau = param(2);
A = param(3);
B = param(4);

%Cosine sequence to the angular response of MTB. Converts angles in
%range [0,pi/2] radians to OD values.
p_norm = [0.559, 0.495, -0.057];

% Get angle in normalised range [0,1], corresponding to [0,90] degrees
% Theta is the angle with respect to the light beam
% Theta will DEcrease during the 'rising slope', as the absorbance
% INcreases with DEcreasing angle
theta = 4/pi * acot(exp((t-delay)./tau));
theta = theta*pi/2;

% Convert to OD
OD = A * sum_of_cosine(p_norm, theta) + B;

% Take care of initial condition
OD(t<=delay) = B;