% FILTER_RECT_PADDING filters a signal in the fourier domain based on a
% supplied cutoff frequency. The filter consists of a rectangular transfer
% function which is either '0' or '1', and can be defined as lowpass of
% highpass. The time-signal is padded (leading and trailing) and later
% removed to buffer the upswing and downswing at the signal ends and this
% way reduce this effect in the signal.
% 
% Inputs:   signal:     The Nx1 or 1xN signal in the time domain.
%           time_sec:   The Nx1 or 1xN evenly-spaced time variable in
%                       seconds.
%           f_cutoff:   Scalar, the desired cutoff frequency in Hertz. Is 2
%                       elements in case of f_type='bandpass' or
%                       'bandstop', in the format [f_low, f_high]
%           f_type:     The filter type ('highpass', 'lowpass', 'bandstop'
%                       or 'bandpass')
%           pad_part:   Scalar, he padding length as part of the length of
%                       the signal. Example: 0.5 will pad the signal with 
%                       0.5*N at the leading end with the value signal(1), 
%                       and 0.5*N at the trailing end with the value
%                       signal(end).
%           plotted:    Boolean. If FALSE, will end the script withough 
%                       plotting. If TRUE, will plot the FFT signal along 
%                       with the filter, and the filtering result.
% 
% Outputs:  signal_filt:    The Nx1 filtered signal.
%           signal_fft:     The Nx1 signal in the frequency domain.
%           freq:           The Nx1 frequency values corresponding to
%                           signal_fft.
% 
% Copyright Tijmen Hageman 2017
% 

function [signal_filt, signal_fft, freq] = filter_rect_padding(signal, time_sec, f_cutoff, f_type, pad_part, plotted)

% Align values along first dimension
signal = signal(:);
time_sec = time_sec(:);

% Pads both sides with 20%
pad_len = round(length(signal)*pad_part);
signal_pad = padarray(signal, [pad_len, 0], 'replicate');

% Fourier transformation of OD
signal_fft = fft(signal_pad);

% Calculate frequency axis
f_max = 1/(time_sec(2)-time_sec(1));
f_step = f_max/length(signal_pad);
freq = 0:f_step:f_max-f_step;

% Defines filter for bottom half of spectrum
H = ones(size(signal_fft));
if strcmp(f_type, 'highpass')
    H(freq < f_cutoff(1)) = 0;
elseif strcmp(f_type, 'lowpass')
    H(freq > f_cutoff(1)) = 0;
elseif strcmp(f_type, 'bandstop')
    H(freq >= f_cutoff(1) & freq <= f_cutoff(2)) = 0;
elseif strcmp(f_type, 'bandpass')
    H(freq < f_cutoff(1)) = 0;
    H(freq > f_cutoff(2)) = 0;
else
    error('Invalid filter type specified. Either lowpass or highpass');    
end

% Mirrors filter to top half of spectrum
Hlen = length(H);
if rem(Hlen,2) % if uneven length
    H((Hlen+3)/2:end) = H((Hlen+1)/2:-1:2);
else % if even length
    H((Hlen/2+1):end) = H((Hlen/2+1):-1:2);
end

% Performs frequency-domain filtering
signal_fft = signal_fft.*H;
    
% Inverse fourier transform to end up at filtered OD
signal_filt = ifft(signal_fft);

% De-padding
signal_filt = signal_filt(pad_len+1:end-pad_len);

% Plot results (of padded signal!!!)
if plotted
    
    % Plot spectrum
    figure, semilogy(freq, abs(fft(signal_pad)));
    xlabel('Frequency [Hz]');
    ylabel('FFT(OD)');
    xlim([0, max(freq)]);
    
    % Plot filter
    hold on;
    filt = max(abs(signal_fft))*ones(1, length(signal_fft));
    filt(signal_fft==0) = 0.01;
    semilogy(freq, filt);
    legend('Unfiltered spectrum', 'Filter');
    
    % Plot filtered stuff
    figure, 
    plot(time_sec, signal); hold on;
    plot(time_sec, signal_filt, 'k');
    xlabel('Time [sec]');
    ylabel('signal');
    legend('Unfiltered', 'Filtered');
    xlim([0, max(time_sec)]);
    
end