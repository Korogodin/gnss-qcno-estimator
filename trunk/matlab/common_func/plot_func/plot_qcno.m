%/**
% Plot qcno real vector
% @param handles - указатель на всея структуру
%*/
function plot_qcno(handles)
globals;


if isstruct(handles)
    hA = handles.axes_qcno;
else
    hF = figure;
    hA = gca;
end

    plot(hA, 1:K, qcno_est, 1:K, 10*log10(A_IQ_eff.^2/ 2 ./ stdn_IQ.^2 / Tc),...
        1:K, qcno_ist, 1:K, 10*log10(sqrtA_IQ2_est.^2/ 2 ./ stdn2_IQ_est / Tc), 1:K, 10*log10(sqrtA_IQ2_est.^2/ 2 ./ 8^2 / Tc));
    xlabel(hA, 'k');
    ylabel(hA, 'q_{c/no}, dBHz');
    grid(hA, 'on');
    ylim(hA, [5 60]);
    xlim(hA, [0 K])


end

