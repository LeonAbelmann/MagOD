function struc = load_data_growth(handles,map_name, plotted, xvar)

% List file namens
% Take all *.CSV and *.csv (you don't want to be dependent on the
% filesystem on the SD card
message_new(handles,['Loading files from' map_name]);

file_names_cap = dir(char(strcat(map_name, '/*.CSV')));
file_names_lc = dir(char(strcat(map_name, '/*.csv')));
file_names_comb = vertcat(file_names_cap, file_names_lc);
%filter out *_set.csv files
j=0;
for i=1:length(file_names_comb)
    if isempty(strfind(lower(file_names_comb(i).name),'_set'));
        j=j+1;
        file_names(j)=file_names_comb(i);
    end
end
    
nmb_filenames = length(file_names);

% Re-order file names and 
file_nr = zeros(nmb_filenames,1);
for i=1:nmb_filenames
    curName = file_names(i).name;
    loc_ = find(curName=='_', 1);   
    file_nr(i) = str2double(curName(loc_+1:end-4));
end
file_nr(isnan(file_nr)) = 0;
[~, sortInd] = sort(file_nr);

% Load each file one by one
for i = 1:(nmb_filenames)
    element_name = file_names(sortInd(i)).name;
    file_name = strcat(map_name, '/', element_name);
    %fprintf('%s, ',element_name);%for debug
    message_new(handles, ['Loading ' element_name]);
    file_table = readtable(file_name,'ReadVariableNames',false,'HeaderLines',1);%ReadVariableNames and Headerlines added by Leon, on my mac in also reads the first line, and that has more variable names than data columns
    if i == 1
        file_matrix = file_table;
    else
        file_matrix = vertcat(file_matrix, file_table);
    end
end

% Dissect all variables
struc.t_sec = table2array(file_matrix(:,1))/1000;
struc.t_min = struc.t_sec/60;
struc.t_hr = struc.t_sec/3600;
struc.Vup = table2array(file_matrix(:,2));
struc.Vdwn = table2array(file_matrix(:,3));
struc.Vav = table2array(file_matrix(:,4));
struc.Vref = table2array(file_matrix(:,5));
struc.Vscat = table2array(file_matrix(:,6));
struc.OD = table2array(file_matrix(:,7));
struc.Temp = table2array(file_matrix(:,8));
struc.LED = table2array(file_matrix(:,9));
struc.prog = table2array(file_matrix(:,10));

% Remember original OD data, we might modify it with offset
struc.ODoriginal = struc.OD;

% Determine what variable to plot on x-axis
if strcmp(xvar,'h') || strcmp(xvar,'hr') || strcmp(xvar,'hour') || strcmp(xvar,'hours')
    x = struc.t_hr;
    x_lab = 'Time [hr]';
elseif strcmp(xvar,'m') || strcmp(xvar,'min') || strcmp(xvar,'minute') || strcmp(xvar,'minutes')
    x = struc.t_min;
    x_lab = 'Time [min]';
elseif strcmp(xvar,'s') || strcmp(xvar,'sec') || strcmp(xvar,'second') || strcmp(xvar,'seconds')
    x = struc.t_sec;
    x_lab = 'Time [sec]';
else
    error('Nonvalid value for xvar given. Choose either ''sec'', ''min'' or ''hr''');
end

% Plot mode 1
if plotted==1
    figure;
    plot(x, struc.OD); xlabel(x_lab); ylabel('OD');
end

% Plot mode 2
if plotted==2
    figure;
    set(gca,'fontsize', 40);
    set(gca,'LineWidth',4);
    subplot(2,2,1); plot(x, struc.Vref); xlabel(x_lab); ylabel('V$_\mathrm{ref}$ [V]');
    subplot(2,2,2); plot(x, struc.Vsig); xlabel(x_lab); ylabel('V$_\mathrm{sig}$ [V]');
    subplot(2,2,3:4); plot(x, struc.OD); xlabel(x_lab); ylabel('OD');
end