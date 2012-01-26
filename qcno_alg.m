function main()

clc
close all

S2 = 30:1:100;
A2 = 1:200;

Q = nan(length(A2), length(S2));
Err = nan(length(A2), length(S2));

for i = 1:length(A2)
    for j = 1:length(S2)
        Q(i, j) = qcno(A2(i), S2(j));
        Err(i, j) = Q(i, j) - 10*log10(A2(i) /S2(j) / 2 / 0.001);
    end
end

fprintf('Max Err = %f; p2p Err = %f; Std Err = %f \n', max(max(abs(Err))), max(max((Err))) - min(min((Err))), sqrt(mean(mean(Err.^2))));

figure(1)
mesh(Q)
zlabel('qcno');

figure(2)
mesh(Err)
zlabel('Err qcno');

end

function Q = qcno(A2, S2)

KA = fix(log2(A2)); % 31 - clz(A2)
KS = fix(log2(S2)); % 31 - clz(S2)

dA = 4;
dC = 6;

if ((6 + KA) < KS)
    Q = 7;
else
    A2 = fix(A2 * 2^(dA - KA + dC));
    S2 = fix(S2 * 2^(- KS + dC));

    sum_S = S2 * 2^(dA-1) + S2;
    quotient_fix = 2^(dA-1);
    
    while (sum_S <= A2)
        sum_S = sum_S + S2;
        quotient_fix = quotient_fix + 1;
    end
    
    log_arr = round(10*log10((2^(dA-1):2^(dA+1)) + 0.5));
    
    Q = 27 + log_arr(quotient_fix - 2^(dA-1) + 1) + 3*(KA-KS-dA);
%     Q = 27 + round(10*log10(fix(A2/S2)+0.5)) + 3*(KA-KS-dA);
end

end
