%/**
%Расчет амплитуды квадратур для статистических эквивалентов
%@param qcno_dB - отношение qcno = Ps/No в дБГц
%@param stdn_IQ - СКО шума квадратур 
%@param Tc - интервал когерентного накопления
%*/
function [A_IQ qcno] =  qcno_change(qcno_dB, stdn_IQ, Tc)

qcno = 10.^(qcno_dB/10);
A_IQ = stdn_IQ .* sqrt(2 * qcno * Tc);

end

