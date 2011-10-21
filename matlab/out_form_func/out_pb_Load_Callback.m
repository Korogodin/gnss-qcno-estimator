function out_pb_Load_Callback(hObject, eventdata, handles)

globals;

clc
set(hObject, 'Enable', 'off');
drawnow;

read_file_and_plot( handles );

set(hObject, 'Enable', 'on');
drawnow; 


end
