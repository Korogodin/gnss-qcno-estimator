%/**
% Plot R2_acum vector
% @param handles - указатель на всея структуру
%*/
function plot_R2_acum(handles)
globals;


if isstruct(handles)
    hA = handles.axes_R2_acum;
else
    hF = figure;
    hA = gca;
end

maxR2_acum = 2^32 - 1;
if (2*max(R2_acum) >= maxR2_acum)
    redline = ones(1,K)*maxR2_acum;
else
    redline = nan(1,K);
end
plot(hA, 1:K, R2_acum, 1:K, redline, 'r');

ylabel(hA, 'R2\_acum');
grid(hA, 'on');

if isstruct(handles) 
    set(hA, 'XTickLabel', []);
else
    xlabel(hA, 'k');
end

end

