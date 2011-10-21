%/**
% Plot allow_stdn2_est vector
% @param handles - указатель на всея структуру
%*/
function plot_allow_stdn2_est(handles)
globals;

if isstruct(handles)
    hA = handles.axes_allow_stdn2_est;
else
    hF = figure;
    hA = gca;
end

plot(hA, 1:K, allow_stdn2_est);

ylabel(hA, 'allow \sigma^2_{est}');
grid(hA, 'on');

set(hA, 'YTick', [0 1]);
set(hA, 'YTickLabel', {'Off', 'On'});
ylim(hA, [-0.2 1.2]);

if isstruct(handles) 
    set(hA, 'XTickLabel', []);
else
    xlabel(hA, 'k');
end

end

