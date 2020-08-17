% MagOD_Cmag GUI for analysis of Magnetic OD measurements
% Used in combination with MagOD instrumentation 
% http://koreumtb.manucodiata.org/index.php/intranet/20-magod
% Tijmen Hageman, Leon Abelmann
% April 2017 - Aug 2019


function varargout = MagOD_Cmag(varargin)
% MagOD_Cmag MATLAB code for MagOD_Cmag.fig
%      MagOD_Cmag, by itself, creates a new MagOD_Cmag or raises the existing
%      singleton*.
%
%      H = MagOD_Cmag returns the handle to a new MagOD_Cmag or the handle to
%      the existing singleton*.
%
%      MagOD_Cmag('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in MagOD_Cmag.M with the given input arguments.
%
%      MagOD_Cmag('Property','Value',...) creates a new MagOD_Cmag or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before MagOD_Cmag_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to MagOD_Cmag_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help MagOD_Cmag
% Last Modified by GUIDE v2.5 09-May-2019 15:02:32

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @MagOD_Cmag_OpeningFcn, ...
                   'gui_OutputFcn',  @MagOD_Cmag_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before MagOD_Cmag is made visible.
function MagOD_Cmag_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA). So handles
% itself is not a handle, only the elements in it!
% varargin   command line arguments to MagOD_Cmag (see VARARGIN)

% Choose default command line output for MagOD_Cmag
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% Set version and date
Version='1.0, May 2019';

% Create a data and settings object in the handles
handles.data = [];
handles.dataF = [];
handles.settings = [];

% Initialize variables
handles.settings.LEDcolor = 0; % Whether or not to show lines in the graph with the color of the LED
% Graph colors, rgb definitions
handles.settings.colors.green = [0,0.5,0];
handles.settings.colors.red = [1,0,0];
handles.settings.colors.blue = [0,0,1];
handles.test = 0;


% UIWAIT makes MagOD_Cmag wait for user response (see UIRESUME)
% uiwait(handles.figure1);

% Logo
% Read color logo image.
% Prepare the full file name.
folder = pwd;
baseFileName = 'magod.jpg';
% Get the full filename, with path prepended.
fullFileName = fullfile(folder, baseFileName);
% Check for existence. Warn user if not found.
if ~exist(fullFileName, 'file')
    % Didn't find it there. Check the search path for it.
    fullFileName = baseFileName; % No path this time.
    if ~exist(fullFileName, 'file')
        % Still didn't find it. Alert user.
        errorMessage = sprintf('Error: %s does not exist.', fullFileName);
        uiwait(warndlg(errorMessage));
    end
else
     % Read it in from disk.
     rgbImage = imread(fullFileName);
    % Display the original color image.
     axes(handles.axes2);
     imshow(rgbImage, []);
     title('MagOD Logo Image', 'FontSize', 20);   
end

guidata(handles.figure1, handles);

addpath('subscripts');
addpath('subscripts/fitting_functions');
init_GUI(handles);
message_append(handles, strcat('MagOD Cmag Analyzer Version: ', Version));


% --- Outputs from this function are returned to the command line.
function varargout = MagOD_Cmag_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in save_push_save.
function save_push_save_Callback(hObject, eventdata, handles)
% hObject    handle to save_push_save (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

save_data(handles);


% --- Executes on selection change in gamma_popup_segnr.
function gamma_popup_segnr_Callback(hObject, eventdata, handles)
% hObject    handle to gamma_popup_segnr (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns gamma_popup_segnr contents as cell array
%        contents{get(hObject,'Value')} returns selected item from gammaSe_popup_segnr

% Store data
handles.settings.gamma_segment = get(hObject, 'Value');
guidata(handles.figure1, handles);

message_new(handles, ['Selected segment ' get(hObject, 'Value') ' as fitting segment.']);

% Adjust cycle data and plot
adjust_cycle_data(handles, handles.settings.cycle_nr_cur);


% --- Executes during object creation, after setting all properties.
function gamma_popup_segnr_CreateFcn(hObject, eventdata, handles)
% hObject    handle to gamma_popup_segnr (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


function gamma_edit_cyclefreq_Callback(hObject, eventdata, handles)
% hObject    handle to gamma_edit_cyclefreq (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of gamma_edit_cyclefreq as text
%        str2double(get(hObject,'String')) returns contents of gamma_edit_cyclefreq as a double

value = get(hObject, 'String');
valid = check_input(value, 'int', [1, Inf]);

% If valid, process data and update graph
if valid
    message_new(handles, 'New value for gamma period.');
    handles.settings.gamma_calc_period = str2double(value);
    guidata(handles.figure1, handles);
    
% If invalid, keep original value
else
    message_new(handles, 'Invalid value for gamma period supplied.');
    set(hObject, 'String', num2str(handles.settings.gamma_calc_period));
end


% --- Executes during object creation, after setting all properties.
function gamma_edit_cyclefreq_CreateFcn(hObject, eventdata, handles)
% hObject    handle to gamma_edit_cyclefreq (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in gamma_push_calc.
function gamma_push_calc_Callback(hObject, eventdata, handles)
% hObject    handle to gamma_push_calc (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Calculate and plot gamma
calculate_gamma_batch(handles);
handles = guidata(handles.figure1);

% Activate plot button
set(handles.gamma_push_plot, 'Enable', 'on');


% --- Executes on button press in gamma_push_plot.
function gamma_push_plot_Callback(hObject, eventdata, handles)
% hObject    handle to gamma_push_plot (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

message_new(handles, 'Creating new figure gamma.');
update_plot_gamma(handles, 1);


% --- Executes on button press in gamma_check_filter.
function gamma_check_filter_Callback(hObject, eventdata, handles)
% hObject    handle to gamma_check_filter (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of gamma_check_filter

% Save data
handles.settings.gamma_plot_filter_enable = get(hObject, 'Value');
guidata(handles.figure1, handles);

if get(hObject, 'Value')
    message_new(handles, 'Enabled plotting filter for gamma.');
else
    message_new(handles, 'Disabled plotting filter for gamma.');
end

% Perform filtering action
calculate_gamma_filtered(handles);
handles = guidata(handles.figure1);

% Update plot
update_plot_gamma(handles, 0);



function gamma_edit_std_Callback(hObject, eventdata, handles)
% hObject    handle to gamma_edit_std (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of gamma_edit_std as text
%        str2double(get(hObject,'String')) returns contents of gamma_edit_std as a double

value = get(hObject, 'String');
valid = check_input(value, 'double', [0.01, Inf]);

% If valid, process data and update graph
if valid
    
    % Save value
    message_new(handles, 'New value for gamma filter std.');
    handles.settings.gamma_plot_filter_std = str2double(value);
    guidata(handles.figure1, handles);
    
    % Re-calculate the filtered data
    calculate_gamma_filtered(handles);
    
    % Re-plot data
    handles = guidata(handles.figure1);
    update_plot_gamma(handles, 0);
    
% If invalid, keep original value
else
    message_new(handles, 'Invalid value for gamma filter std supplied.');
    set(hObject, 'String', num2str(handles.settings.gamma_plot_filter_std));
end


% --- Executes during object creation, after setting all properties.
function gamma_edit_std_CreateFcn(hObject, eventdata, handles)
% hObject    handle to gamma_edit_std (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in cmag_popup_up.
function cmag_popup_up_Callback(hObject, eventdata, handles)
% hObject    handle to cmag_popup_up (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns cmag_popup_up contents as cell array
%        contents{get(hObject,'Value')} returns selected item from cmag_popup_up

% Save data
handles.settings.segment_up = get(hObject, 'Value');
guidata(handles.figure1, handles);

message_new(handles, ['Selected segment ' get(hObject, 'Value') ' as upper segment.']);

% Update figure
adjust_cycle_data(handles, handles.settings.cycle_nr_cur);


% --- Executes during object creation, after setting all properties.
function cmag_popup_up_CreateFcn(hObject, eventdata, handles)
% hObject    handle to cmag_popup_up (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on selection change in cmag_popup_down.
function cmag_popup_down_Callback(hObject, eventdata, handles)
% hObject    handle to cmag_popup_down (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns cmag_popup_down contents as cell array
%        contents{get(hObject,'Value')} returns selected item from cmag_popup_down

% Save data
handles.settings.segment_down = get(hObject, 'Value');
guidata(handles.figure1, handles);

message_new(handles, ['Selected segment ' get(hObject, 'Value') ' as lower segment.']);

% Update figure
adjust_cycle_data(handles, handles.settings.cycle_nr_cur);


% --- Executes during object creation, after setting all properties.
function cmag_popup_down_CreateFcn(hObject, eventdata, handles)
% hObject    handle to cmag_popup_down (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function cmag_edit_cyclefreq_Callback(hObject, eventdata, handles)
% hObject    handle to cmag_edit_cyclefreq (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of cmag_edit_cyclefreq as text
%        str2double(get(hObject,'String')) returns contents of cmag_edit_cyclefreq as a double

value = get(hObject, 'String');
valid = check_input(value, 'int', [1, Inf]);

% If valid, process data and update graph
if valid
    message_new(handles, 'New value for cmag period.');
    handles.settings.cmag_calc_period = str2double(value);
    guidata(handles.figure1, handles);
    
% If invalid, keep original value
else
    message_new(handles, 'Invalid value for cmag period supplied.');
    set(hObject, 'String', num2str(handles.settings.cmag_calc_period));
end


% --- Executes during object creation, after setting all properties.
function cmag_edit_cyclefreq_CreateFcn(hObject, eventdata, handles)
% hObject    handle to cmag_edit_cyclefreq (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in cmag_push_calc.
function cmag_push_calc_Callback(hObject, eventdata, handles)
% hObject    handle to cmag_push_calc (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Perform calculation and plot
calculate_cmag_dmag_batch(handles);
handles = guidata(handles.figure1);

% Activate plot button
set(handles.cmag_push_plot, 'Enable', 'on');


% --- Executes on button press in cmag_push_plot.
function cmag_push_plot_Callback(hObject, eventdata, handles)
% hObject    handle to cmag_push_plot (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

message_new(handles, 'Creating new plot Cmag and Dmag.');
update_plot_cmag(handles, 1);


% --- Executes on button press in cmag_check_filter.
function cmag_check_filter_Callback(hObject, eventdata, handles)
% hObject    handle to cmag_check_filter (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of cmag_check_filter

% Save data
handles.settings.cmag_plot_filter_enable = get(hObject, 'Value');
guidata(handles.figure1, handles);

if get(hObject, 'Value')
    message_new(handles, 'Enabled plotting filter for cmag.');
else
    message_new(handles, 'Disabled plotting filter for cmag.');
end

% Perform filtering action
calculate_cmag_dmag_filtered(handles);
handles = guidata(handles.figure1);

% Update plot
update_plot_cmag(handles, 0);



function cmag_edit_std_Callback(hObject, eventdata, handles)
% hObject    handle to cmag_edit_std (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of cmag_edit_std as text
%        str2double(get(hObject,'String')) returns contents of cmag_edit_std as a double

value = get(hObject, 'String');
valid = check_input(value, 'double', [0.01, Inf]);

% If valid, process data and update graph
if valid
    
    % Save value
    message_new(handles, 'New value for Cmag filter std.');
    handles.settings.cmag_plot_filter_std = str2double(value);
    guidata(handles.figure1, handles);
    
    % Re-calculate the filtered data
    calculate_cmag_dmag_filtered(handles);
    
    % Re-plot data
    handles = guidata(handles.figure1);
    update_plot_cmag(handles, 0);
    
% If invalid, keep original value
else
    message_new(handles, 'Invalid value for Cmag filter std supplied.');
    set(hObject, 'String', num2str(handles.settings.cmag_plot_filter_std));
end


% --- Executes during object creation, after setting all properties.
function cmag_edit_std_CreateFcn(hObject, eventdata, handles)
% hObject    handle to cmag_edit_std (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function cmag_edit_time_Callback(hObject, eventdata, handles)
% hObject    handle to cmag_edit_time (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of cmag_edit_time as text
%        str2double(get(hObject,'String')) returns contents of cmag_edit_time as a double

value = get(hObject, 'String');
valid = check_input(value, 'double', [1, Inf]);

% If valid, process data and update graph
if valid
    message_new(handles, 'New value for integration period.');
    handles.settings.duration_averaging_sec = str2double(value);
    guidata(handles.figure1, handles);
    adjust_cycle_data(handles, handles.settings.cycle_nr_cur);
    
% If invalid, keep original value
else
    message_new(handles, 'Invalid value for integration period supplied.');
    set(hObject, 'String', num2str(handles.settings.duration_averaging_sec));
end



% --- Executes during object creation, after setting all properties.
function cmag_edit_time_CreateFcn(hObject, eventdata, handles)
% hObject    handle to cmag_edit_time (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on slider movement.
function cycle_slider_Callback(hObject, eventdata, handles)
% hObject    handle to cycle_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'Value') returns position of slider
%        get(hObject,'Min') and get(hObject,'Max') to determine range of slider

% Get value and perform proper rounding
value = get(hObject, 'Value');
value = round(value);
set(hObject, 'Value', value);

% Save in settings
handles.settings.cycle_nr_cur = value;

% Update the corresponding label
set(handles.cycle_text_slider, 'String', ['Cycle ' num2str(value), '/' num2str(handles.settings.cycle_max)]);
guidata(handles.figure1, handles);

% Update message
message_new(handles, ['Changed current cycle to ' num2str(value) '.']);

% Update cycle and image
adjust_cycle_data(handles, value);



% --- Executes during object creation, after setting all properties.
function cycle_slider_CreateFcn(hObject, eventdata, handles)
% hObject    handle to cycle_slider (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: slider controls usually have a light gray background.
if isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor',[.9 .9 .9]);
end


% --- Executes on button press in cycle_push_plot.
function cycle_push_plot_Callback(hObject, eventdata, handles)
% hObject    handle to cycle_push_plot (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

message_new(handles, 'Creating new figure cycle data.')

% Avoid problems at first run
if isempty(handles.data.cycle.OD)
    adjust_cycle_data(handles, handles.settings.cycle_nr_cur);
end
handles = guidata(handles.figure1);
update_plot_cycle(handles, 1);


% --- Executes on button press in OD_push_load.
function OD_push_load_Callback(hObject, eventdata, handles)
% hObject    handle to OD_push_load (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

load_data(handles);

% --- Executes on button press in OD_check_offset.
function OD_check_offset_Callback(hObject, eventdata, handles)
% hObject    handle to OD_check_offset (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of OD_check_offset

% Process new value
value = get(hObject, 'Value');
handles.settings.offset_enable = value;
guidata(handles.figure1, handles);

% Message
if value==0
    handles.data.all.OD = handles.data.all.ODoriginal;
    message_new(handles, 'Disabled offset, restored to original OD from datafile.')
else
    if handles.settings.offsetred == 0
         message_new(handles, 'Invalid value for offset supplied.');
    else
        handles.data.all.OD = change_offset(handles,handles.data.all.OD);
        message_new(handles, 'Offset adjusted.');
    end
end

% Update figure
update_plot_OD(handles, 0);

function OD_edit_offset_red_Callback(hObject, eventdata, handles)
% hObject    handle to OD_edit_offset_red (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of OD_edit_offset_red as text
%        str2double(get(hObject,'String')) returns contents of OD_edit_offset_red as a double

% Check whether entry is valid
value = get(hObject, 'String');
valid = check_input(value, 'double', [-Inf, Inf]);

% If valid, process data and update graph
if valid
    message_new(handles, 'New value for offset red supplied.');
    handles.settings.offsetred = str2double(value);
    % correct offset with new value
    fprintf('before: %f', handles.data.all.OD(1))
    handles.data.all.OD = change_offset(handles,handles.data.all.OD);;
    fprintf('after: %f',handles.data.all.OD(1))
    guidata(handles.figure1, handles);
    update_plot_OD(handles, 0);
% If invalid, keep original value
else
    message_new(handles, 'Invalid value for offset red/general supplied.');
    set(hObject, 'String', num2str(handles.settings.offsetred));
end

% --- Executes during object creation, after setting all properties.
function OD_edit_offset_red_CreateFcn(hObject, eventdata, handles)
% hObject    handle to OD_edit_offset_red (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end

function OD_edit_offset_green_Callback(hObject, eventdata, handles)
% hObject    handle to OD_edit_offset_green (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of OD_edit_offset_green as text
%        str2double(get(hObject,'String')) returns contents of OD_edit_offset_green as a double
% Check whether entry is valid
value = get(hObject, 'String');
valid = check_input(value, 'double', [-Inf, Inf]);

% If valid, process data and update graph
if valid
    message_new(handles, 'New value for offset green supplied.');
    handles.settings.offsetgreen = str2double(value);
    % correct offset with new value
    handles.data.all.OD = change_offset(handles,handles.data.all.OD);;
    guidata(handles.figure1, handles);
    update_plot_OD(handles, 0);
% If invalid, keep original value
else
    message_new(handles, 'Invalid value for offset supplied.');
    set(hObject, 'String', num2str(handles.settings.offsetgreen));
end


% --- Executes during object creation, after setting all properties.
function OD_edit_offset_green_CreateFcn(hObject, eventdata, handles)
% hObject    handle to OD_edit_offset_green (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end



function OD_edit_offset_blue_Callback(hObject, eventdata, handles)
% hObject    handle to OD_edit_offset_blue (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of OD_edit_offset_blue as text
%        str2double(get(hObject,'String')) returns contents of OD_edit_offset_blue as a double
value = get(hObject, 'String');
valid = check_input(value, 'double', [-Inf, Inf]);

% If valid, process data and update graph
if valid
    message_new(handles, 'New value for offset blue supplied.');
    handles.settings.offsetblue = str2double(value);
    % correct offset with new value
    handles.data.all.OD = change_offset(handles,handles.data.all.OD);
    guidata(handles.figure1, handles);
    update_plot_OD(handles, 0);
% If invalid, keep original value
else
    message_new(handles, 'Invalid value for offset blue supplied.');
    set(hObject, 'String', num2str(handles.settings.offsetblue));
end


% --- Executes during object creation, after setting all properties.
function OD_edit_offset_blue_CreateFcn(hObject, eventdata, handles)
% hObject    handle to OD_edit_offset_blue (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes on button press in OD_push_plot.
function OD_push_plot_Callback(hObject, eventdata, handles)
% hObject    handle to OD_push_plot (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with hbac_dataFandles and user data (see GUIDATA)

message_new(handles, 'Creating new figure raw data.');
update_plot_OD(handles, 1);


% --- Executes on button press in OD_push_plot_filtered.
function OD_push_plot_filtered_Callback(hObject, eventdata, handles)
% hObject    handle to OD_push_plot_filtered (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

message_new(handles, 'Creating new figure filtered data.');
handles = filter_data(handles);
update_plot_ODF(handles, 1);
    

function initialize_data(handles)

data = handles.data;
dataF = handles.dataF;

% Data (raw)
data.all.OD = [];
data.all.time_sec = [];
data.all.time_min = [];
data.all.time_hr = [];
data.all.time_day = [];
data.all.prog = [];
data.all.maxprog = [];

% Data (filtered)
dataF.all.OD = [];
dataF.all.time_sec = [];
dataF.all.time_min = [];
dataF.all.time_hr = [];
dataF.all.time_day = [];
dataF.all.prog = [];
dataF.all.maxprog = [];

% Single cycle (raw)
data.cycle.OD = [];
data.cycle.time_sec = [];
data.cycle.prog = [];
data.cycle.time_up = [];
data.cycle.time_down = [];
data.cycle.OD_up = [];
data.cycle.OD_down = [];

% Single cycle (filtered)
dataF.cycle.OD = [];
dataF.cycle.time_sec = [];
dataF.cycle.prog = [];
dataF.cycle.OD_fit = [];
data.cycle.time_fit = [];

% Single segment (raw)
data.seg.OD = [];
data.seg.time_sec = [];
data.seg.prog = [];
data.seg.OD_filt = [];

% Single segment (filtered)
dataF.seg.OD = [];
dataF.seg.time_sec = [];
dataF.seg.prog = [];
dataF.seg.OD_filt = [];

% Cycle synchronisation data (raw)
data.cycles.ind_start = [];
data.cycles.ind_end = [];
data.cycles.number = [];
data.cycles.duration_sec = [];
data.cycles.cmag = [];
data.cycles.dmag = [];
data.cycles.time_cmagdmag_sec = [];
data.cycles.time_cmagdmag_min = [];
data.cycles.time_cmagdmag_hr = [];
data.cycles.time_cmagdmag_day = [];
data.cycles.cmagF = [];
data.cycles.dmagF = [];
data.cycles.tau = [];
data.cycles.gamma = [];
data.cycles.time_gamma_sec = [];
data.cycles.time_gamma_min = [];
data.cycles.time_gamma_hr = [];
data.cycles.time_gamma_day = [];
data.cycles.tauF = [];
data.cycles.gammaF = [];

% Cycle synchronisation data (filtered)
dataF.cycles.ind_start = [];
dataF.cycles.ind_end = [];
dataF.cycles.number = [];
dataF.cycles.duration_sec = [];

% Segment synchronisation data (raw)
data.segs.ind_start = [];
data.segs.ind_end = [];
data.segs.prog = [];
data.segs.number = [];
% data.segs.duration_sec = [];

% Segment synchronisation data (filtered)
dataF.segs.ind_start = [];
dataF.segs.ind_end = [];
dataF.segs.prog = [];
dataF.segs.number = [];
% dataF.segs.duration_sec = [];

% Saves the data
handles.data = data;
guidata(handles.figure1, handles);


    
function load_data(handles)

% Get folder name
pathName = uigetdir;
if pathName
    handles.settings.source_folder = pathName;
else
    newMessage(handles.messages_text, 'Error: no valid path name selected');
    return;
end

message_new(handles, 'Initializing data structure.');

% Initialize data
initialize_data(handles);
handles = guidata(handles.figure1);

% Buffer variables
data = handles.data;
dataF = handles.dataF;

message_append(handles, 'Loading and pre-processing data.');

% Load data
bac_data = load_data_growth(handles,pathName, 0, 'sec');

% Break data into cycles and segments
handles.data = load_and_break_data(bac_data, data);

% Save in handles
%handles.data = data;

guidata(handles.figure1, handles);

message_append(handles, 'Updating GUI.');

% Initialize GUI settings
init_GUI_with_new_data(handles);
handles = guidata(handles.figure1);

% Update the GUI accordingly
update_GUI_with_new_data(handles);

% Make GUI active
activate_GUI(handles);
set(handles.cmag_push_plot, 'Enable', 'off');
set(handles.gamma_push_plot, 'Enable', 'off');

message_append(handles, 'Ready.');


% --- Executes when selected object is changed in OD_panel_xaxis.
function OD_panel_xaxis_SelectionChangedFcn(hObject, eventdata, handles)
% hObject    handle to the selected object in OD_panel_xaxis 
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Process radio buttons
if get(handles.OD_radio_days, 'Value')
    handles.settings.time_scale = 'days';
elseif get(handles.OD_radio_hours, 'Value')
    handles.settings.time_scale = 'hours';
elseif get(handles.OD_radio_minutes, 'Value')
    handles.settings.time_scale = 'minutes';
elseif get(handles.OD_radio_seconds, 'Value')
    handles.settings.time_scale = 'seconds';
else
    error('Invalid time scale selected')
end

% Save in handles
guidata(handles.figure1, handles);

message_new(handles, ['Switched time axis to ' handles.settings.time_scale, '.']);

% Plot result
update_plot_OD(handles, 0);
update_plot_ODF(handles, 0);
update_plot_cmag(handles, 0);



function gamma_edit_B_Callback(hObject, eventdata, handles)
% hObject    handle to gamma_edit_B (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of gamma_edit_B as text
%        str2double(get(hObject,'String')) returns contents of gamma_edit_B as a double

value = get(hObject, 'String');
valid = check_input(value, 'double', [0.001, Inf]);

% If valid, process data and update graph
if valid
    
    % Store new value for B
    message_new(handles, 'New value for B-field strength.');
    handles.settings.B_mT = str2double(value);
    guidata(handles.figure1, handles);
    
    % Adjust cycle plot
    adjust_cycle_data(handles, handles.settings.cycle_nr_cur);
    
    % Re-calculate data for gamma
    handles.data.cycles.gamma = 1./(pi*handles.data.cycles.tau*str2double(value));
    guidata(handles.figure1, handles);
    
    % Re-do filtering action
    calculate_gamma_filtered(handles);
    handles = guidata(handles.figure1);
    
    % Update plot
    update_plot_gamma(handles, 0);
    
% If invalid, keep original value
else
    message_new(handles, 'Invalid value for B-field strength supplied.');
    set(hObject, 'String', num2str(handles.settings.B_mT));
end



% --- Executes during object creation, after setting all properties.
function gamma_edit_B_CreateFcn(hObject, eventdata, handles)
% hObject    handle to gamma_edit_B (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes when selected object is changed in gamma_panel_function.
function gamma_panel_function_SelectionChangedFcn(hObject, eventdata, handles)
% hObject    handle to the selected object in gamma_panel_function 
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Process radio buttons
if get(handles.gamma_radio_gexpcot, 'Value')
    handles.settings.fitting_function = 'gexpcot';
elseif get(handles.gamma_radio_expcot, 'Value')
    handles.settings.fitting_function = 'expcot';
elseif get(handles.gamma_radio_exp, 'Value')
    handles.settings.fitting_function = 'exp';
else
    error('Invalid time scale selected')
end

message_new(handles, ['Fitting function set to ' handles.settings.fitting_function '.']);

% Save in handles
guidata(handles.figure1, handles);

% Plot result
adjust_cycle_data(handles, handles.settings.cycle_nr_cur);


% --- Executes when selected object is changed in gamma_panel_slope.
function gamma_panel_slope_SelectionChangedFcn(hObject, eventdata, handles)
% hObject    handle to the selected object in gamma_panel_slope 
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Process radio buttons
if get(handles.gamma_radio_up, 'Value')
    handles.settings.slope_type = 'up';
elseif get(handles.gamma_radio_down, 'Value')
    handles.settings.slope_type= 'down';
else
    error('Invalid time scale selected')
end

message_new(handles, ['Slope direction set to ' handles.settings.slope_type '.']);

% Save in handles
guidata(handles.figure1, handles);

% Plot result
adjust_cycle_data(handles, handles.settings.cycle_nr_cur);


% --- Executes on button press in Color.
% Show Colors
function Color_Callback(hObject, eventdata, handles)
% hObject    handle to Color (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% Process new value
value = get(hObject, 'Value');
handles.settings.LEDcolor = value;
guidata(handles.figure1, handles);

% Message
if value==0
    message_new(handles, 'Colors off')
else
    message_new(handles, 'Colors on')
end

% Update figure
update_plot_OD(handles, 0);
update_plot_ODF(handles, 0);


% Hint: get(hObject,'Value') returns toggle state of Color


% --- Executes during object creation, after setting all properties.
function axes2_CreateFcn(hObject, eventdata, handles)
% hObject    handle to axes2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: place code in OpeningFcn to populate axes2


% --- Executes during object creation, after setting all properties.
function messages_text_CreateFcn(hObject, eventdata, handles)
% hObject    handle to messages_text (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called


% --- Executes during object creation, after setting all properties.
function OD_text_name_CreateFcn(hObject, eventdata, handles)
% hObject    handle to OD_text_name (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called


% --- If Enable == 'on', executes on mouse press in 5 pixel border.
% --- Otherwise, executes on mouse press in 5 pixel border or over OD_push_load.
function OD_push_load_ButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to OD_push_load (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on key press with focus on OD_push_load and none of its controls.
function OD_push_load_KeyPressFcn(hObject, eventdata, handles)
% hObject    handle to OD_push_load (see GCBO)
% eventdata  structure with the following fields (see MATLAB.UI.CONTROL.UICONTROL)
%	Key: name of the key that was pressed, in lower case
%	Character: character interpretation of the key(s) that was pressed
%	Modifier: name(s) of the modifier key(s) (i.e., control, shift) pressed
% handles    structure with handles and user data (see GUIDATA)
