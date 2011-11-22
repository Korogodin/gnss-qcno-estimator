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
    
    U2_SHIFT = 0;
    if isnan(stdn2_IQ_est_shifted(1))
        plot(hA, 1:K, stdn2_IQ_est, 1, NaN, 1:K, stdn_IQ.^2);
    else
        plot(hA, 1:K, stdn2_IQ_est, 1:K, stdn2_IQ_est_shifted / 2^8 / 2^U2_SHIFT, 1:K, stdn_IQ.^2);
    end
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

