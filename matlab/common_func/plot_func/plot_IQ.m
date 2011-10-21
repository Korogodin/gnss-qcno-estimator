%/**
% Plot I, Q vectors
% @param handles - указатель на всея структуру
%*/
function plot_IQ(handles)
globals;

if isstruct(handles)
    hA = handles.axes_IQ;
else
    hF = figure;
    hA = gca;
end

plot(hA, 1:K, I, 1:K, Q);

ylabel(hA, 'I, Q');
set(hA, 'XGrid','on','YGrid','on')

if isstruct(handles)
    set(hA, 'XTickLabel', []);
else
    xlabel(hA, 'k');
end

end

