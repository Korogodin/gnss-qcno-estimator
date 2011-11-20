%/**
%Выбор сценария из списка
%@param hObject - указатель на popup-menu на форме
%*/
function out_popup_scenarios_Callback(hObject, eventdata, handles)

globals; 

select_string = get(hObject, 'Value'); 
all_strings = get(hObject, 'String');
select_scen = all_strings(select_string, 1); % // Scenario's filename

scenario_data =open([pwd '/matlab/scenarios/' select_scen{1}]); % // Open scenario's file

% // Read scenario's data
I = scenario_data.I;
Q = scenario_data.Q;
K = scenario_data.K;
InSync = scenario_data.InSync;
SyncFirst = scenario_data.SyncFirst;
SyncLast = scenario_data.SyncLast;
SyncTemp = scenario_data.SyncTemp;
qcno_ist = scenario_data.qcno_ist;
Description = scenario_data.Description;
A_IQ = scenario_data.A_IQ;
A_IQ_eff = scenario_data.A_IQ_eff;
Tc = scenario_data.Tc;
EpsW = scenario_data.EpsW;
stdn_IQ = scenario_data.stdn_IQ;

qcno_est = nan(1,K);
A_IQ_est = nan(1,K);
sqrtA_IQ2_est = nan(1, K);
stdn2_IQ_est = nan(1,K);

sum_counter = nan(1, K);
acum_counter = nan(1, K);
fail_counter = nan(1, K);
allow_stdn2_est = nan(1, K);

R2 = nan(1, K);
R4 = nan(1, K);
R2_acum = nan(1, K);
R4_acum = nan(1, K);

start_counter = nan(1, K);

% // Description area
set(handles.ed_Description, 'String', Description); 

% // Plot I and Q
plot_IQ(handles);

% // Plot error of freq estimation
plot_EpsW(handles);

% // Plot InSync
plot_InSync(handles);

% // Plot SyncFirst & SyncLast
plot_SyncFirstLast(handles);

% // Plot qcno real vector
plot_qcno(handles);

% // Plot amp-vectors
plot_A_IQ(handles);

% // Plot stdn_IQ-vectors
plot_stdn_IQ(handles);

% // Plot sum_counter
plot_sum_counter(handles);

% // Plot acum_counter
plot_acum_counter(handles);

% // Plot fail_counter
plot_fail_counter(handles);

% // Plot allow_stdn2_est
plot_allow_stdn2_est(handles);

plot_R2(handles);
plot_R4(handles);
plot_R2_acum(handles);
plot_R4_acum(handles);

end

