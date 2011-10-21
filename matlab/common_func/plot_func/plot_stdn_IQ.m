%/**
% Plot stdn vectors
% @param handles - указатель на всея структуру
%*/
function plot_stdn_IQ(handles)
globals;


if isstruct(handles)
    hA = handles.axes_stdn_IQ;
else
    hF = figure;
    hA = gca;
end

    plot(hA, 1:K, stdn2_IQ_est, 1:K, stdn_IQ.^2);
    ylabel(hA, '\sigma^2_{IQ}');
    grid(hA, 'on');
%     ylim(hA, [5 60]);
    xlim(hA, [0 K])

if isstruct(handles)
    set(hA, 'XTickLabel', []);
else
    xlabel(hA, 'k');    
end

end

