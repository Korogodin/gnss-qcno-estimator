%/**
% Plot A_IQ, A_IQ_eff and other apm-vectors
% @param handles - указатель на всея структуру
%*/
function plot_A_IQ(handles)
globals;


if isstruct(handles)
    hA = handles.axes_A_IQ;
else
    hF = figure;
    hA = gca;
end

    plot(hA, 1:K, A_IQ_est, 1:K, A_IQ_eff, 1:K, A_IQ, 1:K, sqrtA_IQ2_est);
    ylabel(hA, 'A_{IQ}');
    grid(hA, 'on');
%     ylim(hA, [5 60]);
    xlim(hA, [0 K])

if isstruct(handles)
    set(hA, 'XTickLabel', []);
else
    xlabel(hA, 'k');    
end

end

