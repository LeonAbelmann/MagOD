function segment_times = getsegmen_times(handles, cycle_nr)
% segment_times contains the timestamps of the beginning of each cycle

seg_nr=zeros(1, handles.data.all.maxprog);%sections in this cycle initialize
for i=1:length(seg_nr)
    seg_nr(i) = (cycle_nr - 1) * handles.data.all.maxprog + i;
end
seg_ind=handles.data.segs.ind_start(seg_nr);
segment_times=handles.data.all.time_sec(seg_ind);

