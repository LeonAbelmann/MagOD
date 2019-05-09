function OD = ODstep_rise(param, t)

delay = param(1);
tau = param(2);
A = param(3);
B = param(4);

% Get angle in normalised range [0,1], corresponding to [0,90] degrees
% Theta is the angle with respect to the light beam
% Theta will DEcrease during the 'rising slope', as the absorbance
% INcreases with DEcreasing angle
theta = 1-exp(-(t-delay)./tau);

% Convert to OD
OD = A * theta + B;

% Take care of initial condition
OD(t<=delay) = B;