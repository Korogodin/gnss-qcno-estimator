function out_pb_Generate_Callback(hObject, eventdata, handles)

globals;

clc
set(hObject, 'Enable', 'off');
drawnow;

% // Запись данных в файл: K, I, Q, SyncTemp, 
fid = fopen('file_in.csv', 'w');
fprintf(fid, '%d\n', K);
for k = 1:K
    fprintf(fid, '%d\n', I(k));
    if ~mod(k, fix(K/10))
        fprintf('Generate progress I,Q %.0f%%\n', round(k/K*50));
    end
end
for k = 1:K
    fprintf(fid, '%d\n', Q(k));
    if ~mod(k, fix(K/10))
        fprintf('Generate progress I,Q %.0f%%\n', round(k/K*50) + 50);
    end
end
% for k = 1:K
%     fprintf(fid, '%d\n', SyncTemp(k));
%     if ~mod(k, fix(K/10))
%         fprintf('Generate progress SyncTemp %.0f%%\n', round(k/K*100));
%     end
% end
% for k = 1:K
%     fprintf(fid, '%d\n', SyncLast(k));
%     if ~mod(k, fix(K/10))
%         fprintf('Generate progress SyncLast %.0f%%\n', round(k/K*100));
%     end
% end
for k = 1:K
    fprintf(fid, '%d\n', SyncFirst(k));
    if ~mod(k, fix(K/10))
        fprintf('Generate progress SyncFirst %.0f%%\n', round(k/K*100));
    end
end

fclose(fid);

set(hObject, 'Enable', 'on');
drawnow; 

end
