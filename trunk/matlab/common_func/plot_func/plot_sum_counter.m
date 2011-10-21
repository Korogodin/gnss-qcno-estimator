%/**
% Plot sum_counter vector
% @param handles - указатель на всея структуру
%*/
function plot_sum_counter(handles)
globals;

if isstruct(handles)
    hA = handles.axes_sum_counter;
else
    hF = figure;
    hA = gca;
end

plot(hA, 1:K, sum_counter);

ylabel(hA, 'sum\_counter');
grid(hA, 'on');

if isstruct(handles) 
    set(hA, 'XTickLabel', []);
else
    xlabel(hA, 'k');
end

end

