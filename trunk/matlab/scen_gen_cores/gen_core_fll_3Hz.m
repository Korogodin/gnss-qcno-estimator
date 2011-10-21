%/**
%Синтез сценария - ЧАП второго порядка
%*/

Description = ...
    'Сценарий работы системы частотной автоподстройки при qcno от 45 до 0 дБГц, а stdn_IQ=8, период накопления в корреляторах 1 мс, символьной синхронизации нет, полоса ЧАП 3Гц';
filename = 'FLL3Hz-45to0dBHz-stdnIQ_8-T_1ms-NoSync.mat';
disp('gen_core_fll_3Hz started');

global TauChip; 
TauChip = 300;

Tmod = 300; % Время моделирования
Tf = 0.001; % Период работы фильтров 
Tc = 0.001; % Период интегрирования в корреляторе
K = fix(Tmod/Tc); 

Fd = 44.2e6;
L = round(Fd*Tc);

% qcno_ist = 45*ones(1,K); % // SNR
qcno_ist = 45*(ones(1,K) - (1:K)/K); % // SNR

H = 3; % Hz, полоса

Xextr = [0; 0; 0]; % Вектор экстраполяций

Ff = [1 0  0
      0 1  Tf
      0 0  1]; % Переходная матрица для модели частоты (в темпе фильтра)
  
Fc = [1 Tc Tc^2/2
      0 1  Tc
      0 0  1]; % Переходная матрица для модели частоты (в темпе коррелятора)
  
Fincorr =  [1 Tc 0
            0 1  0
            0 0  1]; % Переходная матрица набега фазы в корреляторе

Ko = nan(3,1); % Вектор-столбец коэффициентов фильтра
Ko(3) = 2*16/9*H^2; % Коэффициенты непрерывной системы в установившемся режиме
Ko(2) = sqrt(2*Ko(3));
Ko(1) = 0;

Ko = Ko*Tf; % Переход к коэффициентам дискретной системы

Xist = [0; 100; 0]; % Истинный вектор состояния
stdIst = 0; nIst = randn(1,K); 

stdn_IQ = ones(1,K)*8; %*sqrt(L/2);

A_IQ = nan(1,K); % // Memory allocation
A_IQ_eff = nan(1,K);

I = nan(1,K); Iold = 1; % // Memory allocation
Q = nan(1,K); Qold = 1;

EpsPhi = nan(1, K);
EpsW = nan(1, K);
EpsTau = nan(1, K);
Ud = nan(1, K);

nI = stdn_IQ.*randn(1,K); % // I-comp noise
nQ = stdn_IQ.*randn(1,K); % // Q-comp noise

w = 0;
for k = 1:K
    
    EpsPhi(k) = mod(Xist(1) - Xextr(1),2*pi);
    EpsW(k) = Xist(2) - Xextr(2);
    EpsTau(k) = 0;
    
    [A_IQ(k) qcno] = qcno_change(qcno_ist(k), stdn_IQ(k), Tc);
    A_IQ_eff(k) = A_IQ(k)*sinc(EpsW(k)*Tc/2 /pi)*ro(EpsTau(k));
    mI = A_IQ_eff(k) * cos(EpsW(k)*Tc/2 + EpsPhi(k));
    mQ = - A_IQ_eff(k) * sin(EpsW(k)*Tc/2 + EpsPhi(k));
    I(k) = mI + nI(k);
    Q(k) = mQ + nQ(k);
    
    Xextr = Fincorr * Xextr; % Набег фазы в корреляторе к концу накопления    

    w = w + 1;
    if w == fix(Tf/Tc)  
    %     Ud = f(I(k), Q(k), I(k-1), Q(k-1), ...);      % Дискриминатор
        Ud(k) = (I(k)*Qold - Q(k)*Iold);
    %     Sd = f(A_IQ);             % Критизна дискриминационной характеристики
        Sd = Tc*A_IQ(k)^2;
        Xest = Xextr + Ko*Ud(k)/Sd;    % Вектор оценок на очередной интервал фильтра
        Xextr = Ff*Xest;            % Экстраполяция на следующий интервал
        w = 0;
    end

    Xist = Fc*Xist + [0; 0; 1]*nIst(k)*stdIst; % Здесь может быть любая другая модель изменения истинного вектора состояния
    if ~mod(k,fix(K/10))
        fprintf('Progress: %.0f%%\n', 100*k/K);
    end
    Iold = I(k);
    Qold = Q(k);
end
I = round(I);
Q = round(Q);

A_IQ_eff = abs(A_IQ_eff);

InSync = zeros(1,K);
SyncFirst = zeros(1,K);
SyncLast = zeros(1,K);
SyncTemp = zeros(1,K);

save([pwd  '/matlab/scenarios/' filename], 'K', 'I', 'Q', 'InSync', ...
    'SyncFirst', 'SyncLast', 'SyncTemp', 'qcno_ist', 'Description', ...
    'EpsW', 'A_IQ', 'A_IQ_eff', 'Tc', 'Tf', 'stdn_IQ', 'H');

hF = 100;

hF = figure(hF + 1);
plot(1:K, A_IQ, 1:K, A_IQ_eff);
xlabel('k');
ylabel('A_{IQ}, A_{IQ,eff}')

hF = figure(hF + 1);
plot(1:K, Ud);
xlabel('k');
ylabel('Ud')

hF = figure(hF + 1);
plot(1:K, EpsW);
xlabel('k');
ylabel('EpsW')
