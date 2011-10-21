function out_pb_Run_Callback(hObject, eventdata, handles)

globals;

clc
set(hObject, 'Enable', 'off');
drawnow;

[status_f, result_f] = system('./front-end-corr','-echo');
if status_f
    fprintf('Имитация завершена с ошибкой %d\n', status_f);
else
    disp('Имитация завершена нормально');
end
    
set(hObject, 'Enable', 'on');
drawnow; 


end
