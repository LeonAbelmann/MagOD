function try_save_figure(fig_handle, dir_name, fig_name)

% Check if the handle points to a valid figure
if ~isempty(fig_handle) && ishandle(fig_handle)

    % Saves as a PDF
    print(fig_handle, [dir_name '/' fig_name], '-dpdf', '-bestfit');
end
