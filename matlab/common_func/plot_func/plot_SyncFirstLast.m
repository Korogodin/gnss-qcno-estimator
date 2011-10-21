%/**
% Plot SyncFirst and SyncLast vectors
% @param handles - указатель на всея структуру
%*/
function plot_SyncFirstLast(handles)
globals;

if isstruct(handles)
    hA = handles.axes_SyncFirstLast;
else
    hF = figure;
    hA = gca;
end

plot(hA, 1:K, SyncFirst, 1:K, SyncLast);
xlabel(hA, 'k');
set(hA, 'YTick', [0 1]);
set(hA, 'YTickLabel', {'Off', 'On'});
ylabel(hA, 'Sync flags');
grid(hA, 'on');
ylim(hA, [-0.2 1.2]);

end

