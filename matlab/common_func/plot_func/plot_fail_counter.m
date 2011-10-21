%/**
% Plot fail_counter vector
% @param handles - указатель на всея структуру
%*/
function plot_fail_counter(handles)
globals;

if isstruct(handles)
    hA = handles.axes_fail_counter;
else
    hF = figure;
    hA = gca;
end

plot(hA, 1:K, fail_counter);

ylabel(hA, 'fail\_counter');
grid(hA, 'on');

if isstruct(handles) 
    set(hA, 'XTickLabel', []);
else
    xlabel(hA, 'k');
end

end

