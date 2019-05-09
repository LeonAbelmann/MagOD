function data2csv(header, data, filename_csv)

% Write the header
fid = fopen(filename_csv,'w');
fprintf(fid, '%s\n', header);
fclose(fid);

% Save the data
dlmwrite(filename_csv, data,'-append','delimiter',',')  ;