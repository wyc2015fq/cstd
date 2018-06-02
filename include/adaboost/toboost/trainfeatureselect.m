%%%%特征选择%%%%%%%%%%%%%
clear all;
clc;
T=100;
selectFeaSize=-1;%特征选取的数目，小于0的话按照循环次数进行特征选取
outputTips=1;
%%%%%%%%%%%%%%测试该分类器在随机产生的矩阵中的分类能力%%%%%%%%%%%%%%%%%
% aa=randn(100,100);
% bb=randn(100,100)+0.0009;
% ya=ones(1,100);
% yb=zeros(1,100);
% label=[ya yb];
% Y=label;
% feature=[aa;bb];
%%%%%%%%%%%%
% feature=zeros(5600,1475);%lbp的特征长度为1475
% aa=floor(11000*rand(1,5000));%从类间中随机选取与类内间图片数相等的数量
% filesave=strcat('D:\我的文档\MATLAB\swdislbp','.mat');
% interclass=load(filesave,'swdistance');
% interclass=interclass.swdistance;
% filesave2=strcat('D:\我的文档\MATLAB\sbdislbp','.mat');
% extraclass=load(filesave2,'sbdistance');
% extraclass=extraclass.sbdistance;
% extraclass=extraclass(aa,:);
% numinter=size(interclass,1);
% numextra=size(extraclass,1);
% feature(1:600,:)=interclass;
% feature(601:(numinter+numextra),:)=extraclass;
% clear extraclass interclass;
% labelinter=ones(numinter,1);
% labelextra=zeros(numextra,1);
% Y=[labelinter;labelextra];
% Y=Y';
%%%%%%
% feature=zeros(200,1475);%lbp的特征长度为1475
% aa=floor(11000*rand(1,100));%从类间中随机选取与类内间图片数相等的数量
% bb=floor(500*rand(1,100));
% filesave=strcat('D:\我的文档\MATLAB\swdislbp','.mat');
% interclass=load(filesave,'swdistance');
% interclass=interclass.swdistance;
% interclass=interclass(bb,:);
% filesave2=strcat('D:\我的文档\MATLAB\sbdislbp','.mat');
% extraclass=load(filesave2,'sbdistance');
% extraclass=extraclass.sbdistance;
% extraclass=extraclass(aa,:);
% numinter=size(interclass,1);
% numextra=size(extraclass,1);
% feature(1:100,:)=interclass;
% feature(101:(numinter+numextra),:)=extraclass;
% clear extraclass interclass;
% labelinter=ones(numinter,1);
% labelextra=zeros(numextra,1);
% Y=[labelinter;labelextra];
% Y=Y';         %%%设置kerneldistri函数中的带宽为 bandwith=0.0000015;
aa=load('C:\Documents and Settings\Administrator\桌面\toboost集\test\data');
Y=aa.Y;
feature=aa.feature;
%%%%%%%%%%%%%%%%%
disp('adboost开始')
BoostSeriesFeaSelect(feature,Y,T,selectFeaSize,outputTips)
% [SelectFeaNo,costTime,SelectFeaName]=AdaBoostBasedSingleFeaSelect(feature,Y,T,selectFeaSize);
% 参数selectFeaSize有效时，选取指定数量的特征
% 否则，按轮数T循环
% features: 训练特征
% Y: 训练样本标号
% feaSelectRate: 选择样本占总样本比例
% trainingRate: 训练样本占总样本比例
% T: 训练轮数
% selectFeaSize: 要选择的特征数目
% testTimes: 测试次数
