function calculate_cmag_dmag_filtered(handles)

% Create filter
std = handles.settings.cmag_plot_filter_std;
x_filt = round(-4*std):1:round(4*std);
fgauss = normpdf(x_filt, 0, std);
fgauss = fgauss/sum(fgauss);

% Filter data
handles.data.cycles.cmagF = imfilter(handles.data.cycles.cmag(:), fgauss(:), 'replicate', 'same');
handles.data.cycles.dmagF = imfilter(handles.data.cycles.dmag(:), fgauss(:), 'replicate', 'same');

% Save data
guidata(handles.figure1, handles);