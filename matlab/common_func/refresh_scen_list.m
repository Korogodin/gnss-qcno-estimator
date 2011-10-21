%/**
%Добавление в popup-список доступных сценариев
%@param hObject - указатель на popup-menu на форме
%*/
function erro = refresh_scen_list(hObject)

globals;

scen_list = what([pwd '/matlab/scenarios']); % // Смотрим структуру каталога сценариев

if size(scen_list.mat, 1) > 0 % // Если в нем есть mat-файлы
    set(hObject, 'String', scen_list.mat);
    are_Scenarios = 1; % // Флаг наличия сценариев
else
    set(hObject, 'String', 'No scenarios');
    are_Scenarios = 0;
end

erro = 0;

end

