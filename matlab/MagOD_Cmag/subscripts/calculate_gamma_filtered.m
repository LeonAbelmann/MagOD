function calculate_gamma_filtered(handles)

% Create filter
std = handles.settings.gamma_plot_filter_std;
x_filt = round(-4*std):1:round(4*std);
fgauss = normpdf(x_filt, 0, std);
fgauss = fgauss/sum(fgauss);

% Filter data
handles.data.cycles.tauF = imfilter(handles.data.cycles.tau(:), fgauss(:), 'replicate', 'same');
handles.data.cycles.gammaF = imfilter(handles.data.cycles.gamma(:), fgauss(:), 'replicate', 'same');

% Save data
guidata(handles.figure1, handles);