%/**
% Plot R4 vector
% @param handles - указатель на всея структуру
%*/
function plot_R4(handles)
globals;


if isstruct(handles)
    hA = handles.axes_R4;
else
    hF = figure;
    hA = gca;
end

maxR4 = 2^64 - 1;
if (2*max(R4) >= maxR4)
    redline = ones(1,K)*maxR4;
else
    redline = nan(1,K);
end
plot(hA, 1:K, R4, 1:K, redline, 'r');

ylabel(hA, 'R4');
grid(hA, 'on');

if isstruct(handles) 
    set(hA, 'XTickLabel', []);
else
    xlabel(hA, 'k');
end

end

