function [seg_time, seg_OD] = get_segment(handles, seg_nr)

ind = handles.data.segs.ind_start(seg_nr):handles.data.segs.ind_end(seg_nr);
seg_time = handles.data.all.time_sec(ind);
seg_OD = handles.data.all.OD(ind);
