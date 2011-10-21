clc
clear 
close all

addpath( [pwd '/matlab'] );
addpath( [pwd '/matlab/out_form_func'] );
addpath( [pwd '/matlab/common_func'] );
addpath( [pwd '/matlab/common_func/plot_func'] );
addpath( [pwd '/matlab/lists'] );

globals;
first_init_globals;

if (ispc) % if system is Win
    h_fig_main = guihandles(fig_main);
elseif (isunix)
    h_fig_main = guihandles(fig_main);
end

