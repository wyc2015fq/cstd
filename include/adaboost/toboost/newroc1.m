function  AUC=newroc(output,Y,weight)
%%output表示每个样本在直方图分布上的最终得分，Y表示每个样本的标签，即正负样本标号
%%%%函数举例%%%%
% clear all;
% clc;
% % output=[1 2 3 4];
% % Y=[1 0 1 0];
% % weight=[0.25 0.25 0.25 0.25];
% output=rand(1,10);
% Y=[1  1 1 1 1 0 0 0 0 1];
% weight=[ 1/(12) 1/(12) 1/(12) 1/(12) 1/(12) 1/8 1/8 1/8 1/8 1/(12) ];
%%%%%%%%%
maxout=max(output);
minout=min(output);
sort_output = sort(output);

thresh = ([minout-1 sort_output] + [sort_output maxout+1])*0.5;

