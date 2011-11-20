%/**
% Plot R4_acum vector
% @param handles - указатель на всея структуру
%*/
function plot_R4_acum(handles)
globals;


if isstruct(handles)
    hA = handles.axes_R4_acum;
else
    hF = figure;
    hA = gca;
end

maxR4_acum = 2^64 - 1;
if (2*max(R4_acum) >= maxR4_acum)
    redline = ones(1,K)*maxR4_acum;
else
    redline = nan(1,K);
end
plot(hA, 1:K, R4_acum, 1:K, redline, 'r');

ylabel(hA, 'R4\_acum');
grid(hA, 'on');

xlabel(hA, 'k');

end

