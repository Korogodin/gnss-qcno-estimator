%/**
% Plot InSync vector
% @param handles - указатель на всея структуру
%*/
function plot_InSync(handles)
globals;

if isstruct(handles)
    hA = handles.axes_InSync;
else
    hF = figure;
    hA = gca;
end

plot(hA, 1:K, InSync);
set(hA, 'XTickLabel', []);
set(hA, 'YTick', [0 1]);
set(hA, 'YTickLabel', {'Off', 'On'});
ylabel(hA, 'SymSync');
grid(hA, 'on');
ylim(hA, [-0.2 1.2]);

end

