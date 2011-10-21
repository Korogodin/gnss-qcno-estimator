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

fclose(fid);

end

