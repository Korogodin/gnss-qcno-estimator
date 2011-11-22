%/**
% Чтение из file_out и отрисовка полученных данных
%@param handles - всея структура
%*/
function read_file_and_plot( handles )

globals;

fid = fopen('file_out.csv', 'r');

% K = fscanf(fid, '%u', 1);

sqrtA_IQ2_est = sqrt((fscanf(fid, '%u', K))');
A_IQ_est = (fscanf(fid, '%d', K))';
plot_A_IQ(handles);

stdn2_IQ_est = (fscanf(fid, '%d', K))';
stdn2_IQ_est_shifted = (fscanf(fid, '%d', K))';
plot_stdn_IQ(handles);

qcno_est = (fscanf(fid, '%d', K))';
plot_qcno(handles);

sum_counter = (fscanf(fid, '%d', K))';
plot_sum_counter(handles);

acum_counter = (fscanf(fid, '%d', K))';
plot_acum_counter(handles);

fail_counter = (fscanf(fid, '%d', K))';
plot_fail_counter(handles);

allow_stdn2_est = (fscanf(fid, '%d', K))';
plot_allow_stdn2_est(handles);

R2 = (fscanf(fid, '%u', K))';
plot_R2(handles);

R4 = (fscanf(fid, '%ul', K))';
plot_R4(handles);

R2_acum = (fscanf(fid, '%u', K))';
plot_R2_acum(handles);

R4_acum = (fscanf(fid, '%ul', K))';
plot_R4_acum(handles);

start_counter = (fscanf(fid, '%u', K))';
% plot_start_counter(handles);

IQ_Power = (fscanf(fid, '%u', K))';

N_Coher = (fscanf(fid, '%u', K))';

fclose(fid);

end

