%/**
% Plot acum_counter vector
% @param handles - указатель на всея структуру
%*/
function plot_acum_counter(handles)
globals;

if isstruct(handles)
    hA = handles.axes_acum_counter;
else
    hF = figure;
    hA = gca;
end

plot(hA, 1:K, acum_counter);

ylabel(hA, 'acum\_counter');
grid(hA, 'on');

if isstruct(handles) 
    set(hA, 'XTickLabel', []);
else
    xlabel(hA, 'k');
end

end

