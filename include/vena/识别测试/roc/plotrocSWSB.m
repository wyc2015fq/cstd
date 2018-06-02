clear all;
clc;
load swdislbp;
load sbdislbp;

interclass=swdistance;
extraclass=sbdistance;

labelinter=ones(length(interclass),1);%生成类内距离长度*1的单位矩阵
labelextra=zeros(length(extraclass),1);%生成类间距离长度*1的零矩阵
Y=[labelinter;labelextra];
feature=[interclass;extraclass];
[FAR FRR]=rocp(feature,Y);%红色
hold on
plot(0:1,0:1,':');
grid on;
box on
%rocplot(feature,Y)