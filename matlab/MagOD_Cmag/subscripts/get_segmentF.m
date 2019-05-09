function [seg_time, seg_OD] = get_segmentF(handles, seg_nr)

ind = handles.dataF.segs.ind_start(seg_nr):handles.dataF.segs.ind_end(seg_nr);
seg_time = handles.dataF.all.time_sec(ind);
seg_OD = handles.dataF.all.OD(ind);