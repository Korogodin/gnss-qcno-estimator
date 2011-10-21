function out_pb_Generate_Callback(hObject, eventdata, handles)

globals;

clc
set(hObject, 'Enable', 'off');
drawnow;

% // Запись данных в файл: K, I, Q
fid = fopen('file_in.csv', 'w');
fprintf(fid, '%d\n', K);
for k = 1:K
    fprintf(fid, '%d\n', I(k));
    if ~mod(k, fix(K/10))
        fprintf('Generate progress %.0f%%\n', round(k/K*50));
    end
end
for k = 1:K
    fprintf(fid, '%d\n', Q(k));
    if ~mod(k, fix(K/10))
        fprintf('Generate progress %.0f%%\n', round(k/K*50) + 50);
    end
end

fclose(fid);

set(hObject, 'Enable', 'on');
drawnow; 

end
