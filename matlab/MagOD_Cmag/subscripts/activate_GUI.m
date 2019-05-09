function activate_GUI(handles)

field_names = fieldnames(handles);

% Loop through all elements of the GUI
for i=1:length(field_names)

    cur_element = handles.(field_names{i});

    % Activate all UIControl items
    if isa(cur_element, 'matlab.ui.control.UIControl')
        set(cur_element, 'Enable', 'on');
    end

end