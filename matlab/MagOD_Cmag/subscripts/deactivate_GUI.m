function deactivate_GUI(handles)

field_names = fieldnames(handles);

% Loop through all elements of the GUI
for i=1:length(field_names)

    cur_element = handles.(field_names{i});

    % Deactivate all UIControl items
    if isa(cur_element, 'matlab.ui.control.UIControl')
        set(cur_element, 'Enable', 'off');
    end

end