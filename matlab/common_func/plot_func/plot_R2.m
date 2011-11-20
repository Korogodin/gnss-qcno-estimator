%/**
% Plot R2 vector
% @param handles - указатель на всея структуру
%*/
function plot_R2(handles)
globals;


if isstruct(handles)
    hA = handles.axes_R2;
else
    hF = figure;
    hA = gca;
end

maxR2 = 2^32 - 1;
if (2*max(R2) >= maxR2)
    redline = ones(1,K)*maxR2;
else
    redline = nan(1,K);
end
plot(hA, 1:K, R2, 1:K, redline, 'r');
ylabel(hA, 'R2');
grid(hA, 'on');

if isstruct(handles) 
    set(hA, 'XTickLabel', []);
else
    xlabel(hA, 'k');
end

end

