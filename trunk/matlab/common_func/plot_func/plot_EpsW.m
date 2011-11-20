%/**
% Plot EpsW vector
% @param handles - указатель на всея структуру
%*/
function plot_EpsW(handles)
globals;

if isstruct(handles)
    hA = handles.axes_EpsW;
else
    hF = figure;
    hA = gca;
end


plot(hA, 1:K, EpsW/2/pi);
ylabel(hA, 'EpsW, Hz');
set(hA,'XGrid','on','YGrid','on')

if isstruct(handles)
    set(hA, 'XTickLabel', []);
else
    xlabel(hA, 'k');
end

end

