function varargout = fig_main(varargin)
% FIG_MAIN M-file for fig_main.fig
%      FIG_MAIN, by itself, creates a new FIG_MAIN or raises the existing
%      singleton*.
%
%      H = FIG_MAIN returns the handle to a new FIG_MAIN or the handle to
%      the existing singleton*.
%
%      FIG_MAIN('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in FIG_MAIN.M with the given input arguments.
%
%      FIG_MAIN('Property','Value',...) creates a new FIG_MAIN or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before fig_main_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to fig_main_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help fig_main

% Last Modified by GUIDE v2.5 22-Nov-2011 14:39:17

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @fig_main_OpeningFcn, ...
                   'gui_OutputFcn',  @fig_main_OutputFcn, ...
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


% --- Executes just before fig_main is made visible.
function fig_main_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to fig_main (see VARARGIN)

% Choose default command line output for fig_main
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes fig_main wait for user response (see UIRESUME)
% uiwait(handles.fig_main);


% --- Outputs from this function are returned to the command line.
function varargout = fig_main_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in pb_Run.
function pb_Run_Callback(hObject, eventdata, handles)
out_pb_Run_Callback(hObject, eventdata, handles);


% --- Executes on selection change in popup_scenarios.
function popup_scenarios_Callback(hObject, eventdata, handles)
out_popup_scenarios_Callback(hObject, eventdata, handles)

% --- Executes during object creation, after setting all properties.
function popup_scenarios_CreateFcn(hObject, eventdata, handles)
% hObject    handle to popup_scenarios (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: popupmenu controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
out_popup_scenarios_CreateFcn(hObject, eventdata, handles);


% --- Executes on key press with focus on popup_scenarios and none of its controls.
function popup_scenarios_KeyPressFcn(hObject, eventdata, handles)
% hObject    handle to popup_scenarios (see GCBO)
% eventdata  structure with the following fields (see UICONTROL)
%	Key: name of the key that was pressed, in lower case
%	Character: character interpretation of the key(s) that was pressed
%	Modifier: name(s) of the modifier key(s) (i.e., control, shift) pressed
% handles    structure with handles and user data (see GUIDATA)



% --- If Enable == 'on', executes on mouse press in 5 pixel border.
% --- Otherwise, executes on mouse press in 5 pixel border or over popup_scenarios.
function popup_scenarios_ButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to popup_scenarios (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)


% --- Executes on button press in pb_refresh_scen_list.
function pb_refresh_scen_list_Callback(hObject, eventdata, handles)
out_pb_refresh_scen_list_Callback(hObject, eventdata, handles);



function ed_Description_Callback(hObject, eventdata, handles)
% hObject    handle to ed_Description (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of ed_Description as text
%        str2double(get(hObject,'String')) returns contents of ed_Description as a double


% --- Executes during object creation, after setting all properties.
function ed_Description_CreateFcn(hObject, eventdata, handles)
% hObject    handle to ed_Description (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --- Executes during object creation, after setting all properties.
function axes_IQ_CreateFcn(hObject, eventdata, handles)
% hObject    handle to axes_IQ (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: place code in OpeningFcn to populate axes_IQ
set(hObject, 'XTick', []);


% --- Executes during object creation, after setting all properties.
function axes_InSync_CreateFcn(hObject, eventdata, handles)
% hObject    handle to axes_InSync (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: place code in OpeningFcn to populate axes_InSync
set(hObject, 'XTick', []);


% --- Executes during object creation, after setting all properties.
function axes_SyncFirstLast_CreateFcn(hObject, eventdata, handles)
% hObject    handle to axes_SyncFirstLast (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: place code in OpeningFcn to populate axes_SyncFirstLast


% --- Executes during object creation, after setting all properties.
function axes_EpsW_CreateFcn(hObject, eventdata, handles)
% hObject    handle to axes_EpsW (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: place code in OpeningFcn to populate axes_EpsW
set(hObject, 'XTick', []);


% --- Executes on mouse press over axes background.
function axes_IQ_ButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to axes_IQ (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
disp('but');


% --- Executes during object creation, after setting all properties.
function axes_A_IQ_CreateFcn(hObject, eventdata, handles)
% hObject    handle to axes_A_IQ (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: place code in OpeningFcn to populate axes_A_IQ
set(hObject, 'XTickLabel', []);


% --- Executes during object creation, after setting all properties.
function axes_stdn_IQ_CreateFcn(hObject, eventdata, handles)
% hObject    handle to axes_stdn_IQ (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: place code in OpeningFcn to populate axes_stdn_IQ
set(hObject, 'XTickLabel', []);


% --- Executes on mouse press over axes background.
function axes_qcno_ButtonDownFcn(hObject, eventdata, handles)
% hObject    handle to axes_qcno (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)



% --- Executes on button press in pb_axes_qcno.
function pb_axes_qcno_Callback(hObject, eventdata, handles)
% hObject    handle to pb_axes_qcno (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
plot_qcno(0);


% --- Executes on button press in pb_axes_IQ.
function pb_axes_IQ_Callback(hObject, eventdata, handles)
% hObject    handle to pb_axes_IQ (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
plot_IQ(0);


% --- Executes on button press in pb_axes_EpsW.
function pb_axes_EpsW_Callback(hObject, eventdata, handles)
% hObject    handle to pb_axes_EpsW (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
plot_EpsW(0);


% --- Executes on button press in pb_axes_InSync.
function pb_axes_InSync_Callback(hObject, eventdata, handles)
% hObject    handle to pb_axes_InSync (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
plot_InSync(0);


% --- Executes on button press in pb_axes_SyncFirstLast.
function pb_axes_SyncFirstLast_Callback(hObject, eventdata, handles)
% hObject    handle to pb_axes_SyncFirstLast (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
plot_SyncFirstLast(0);


% --- Executes on button press in pb_axes_A_IQ.
function pb_axes_A_IQ_Callback(hObject, eventdata, handles)
% hObject    handle to pb_axes_A_IQ (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
plot_A_IQ(0);


% --- Executes on button press in pb_axes_stdn_IQ.
function pb_axes_stdn_IQ_Callback(hObject, eventdata, handles)
% hObject    handle to pb_axes_stdn_IQ (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
plot_stdn_IQ(0);


% --- Executes on button press in cb_exec.
function cb_exec_Callback(hObject, eventdata, handles)
% hObject    handle to cb_exec (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hint: get(hObject,'Value') returns toggle state of cb_exec


% --- Executes on button press in pb_Generate.
function pb_Generate_Callback(hObject, eventdata, handles)
% hObject    handle to pb_Generate (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
out_pb_Generate_Callback(hObject, eventdata, handles);

% --- Executes on button press in pb_Load.
function pb_Load_Callback(hObject, eventdata, handles)
% hObject    handle to pb_Load (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
out_pb_Load_Callback(hObject, eventdata, handles);


% --- Executes on button press in pb_axes_sum_counter.
function pb_axes_sum_counter_Callback(hObject, eventdata, handles)
% hObject    handle to pb_axes_sum_counter (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
plot_sum_counter(0);

% --- Executes on button press in pb_acum_counter.
function pb_acum_counter_Callback(hObject, eventdata, handles)
% hObject    handle to pb_acum_counter (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
plot_acum_counter(0);


% --- Executes on button press in pb_axes_allow_stdn2_est.
function pb_axes_allow_stdn2_est_Callback(hObject, eventdata, handles)
% hObject    handle to pb_axes_allow_stdn2_est (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
plot_allow_stdn2_est(0);

% --- Executes on button press in pb_axes_fail_counter.
function pb_axes_fail_counter_Callback(hObject, eventdata, handles)
% hObject    handle to pb_axes_fail_counter (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
plot_fail_counter(0);


% --- Executes on button press in pb_axes_R2.
function pb_axes_R2_Callback(hObject, eventdata, handles)
% hObject    handle to pb_axes_R2 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
plot_R2(0);

% --- Executes on button press in pb_axes_R4.
function pb_axes_R4_Callback(hObject, eventdata, handles)
% hObject    handle to pb_axes_R4 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
plot_R4(0);

% --- Executes on button press in pb_axes_R2_acum.
function pb_axes_R2_acum_Callback(hObject, eventdata, handles)
% hObject    handle to pb_axes_R2_acum (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
plot_R2_acum(0);

% --- Executes on button press in pb_axes_R4_acum.
function pb_axes_R4_acum_Callback(hObject, eventdata, handles)
% hObject    handle to pb_axes_R4_acum (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
plot_R4_acum(0);
