function  result=kerneldistri(inputscore,input,bandwith)
%%%%高斯核密度分布%%
% clear all;
% clc;
% inputscore=0.99;
% input=rand(1,100);
% % [input,index]=sort(input);
% weight=ones(1,100);
% % weight=weight(index);
%%%%
%%%input为输入矩阵，inputscore为一个数值
 nsample=length(input);
%  bandwith=0.0000015;%%0.0000015
 zongfkernel=0;
 for i=1:nsample
 distance=(inputscore-input(i)).^2;
 fkernel=1/(bandwith.*sqrt(2*pi))*exp(-(distance)./(2*bandwith^2));
 zongfkernel=zongfkernel+fkernel;
 end
 result=zongfkernel/nsample;
% result=zongfkernel;

