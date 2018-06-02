
%%%%%%将数据通过高斯核密度估计和直方图密度估计进行对比，分析估计值和精确值差异%%%
% clear all;
% clc;
% x=randn(1,100);
% x=sort(x);
% n=length(x);
% h=0.5;
% s=zeros(1,n);
% for i=1:n
%     for j=1:n
%         s(i)=s(i)+exp(-((x(i)-x(j)).^2)/(2*h*h));
%     end
% end
% % for i=1:n
% %     s(i)=s(i)+exp(-((x(i)-x)*(x(i)-x)')/(2*h*h));
% % end
% y=s./(n*h*sqrt(2*pi));
% [histy,histx]=hist(x);
% % bar(histx,histy/sum(histy),'FaceColor','b','EdgeColor','w');
% bar(histx,histy/sum(histy),'hist');
% hold on;
% plot(x,y*0.5,'r');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
clear all;
clc;
aa=randn(100,1);
bb=randn(100,1)+4;
feature=[aa;bb];
X=feature;
ya=ones(1,100);
yb=zeros(1,100);
label=[ya yb];
Y=label;
%%%%%%%
% feature=zeros(1000,1475);%lbp的特征长度为1475
% aa=floor(11000*rand(1,500));%从类间中随机选取与类内间图片数相等的数量
% bb=floor(500*rand(1,500));
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
% feature(1:500,:)=interclass;
% feature(501:(numinter+numextra),:)=extraclass;
% clear extraclass interclass;
% labelinter=ones(numinter,1);
% labelextra=zeros(numextra,1);
% Y=[labelinter;labelextra];
% X=feature;
%%%%%%%
[cntSamples,cntFeatures]=size(X);
X=ceil(X*10000)/10000;          % 训练数据整理：删除尾部数据，防止产生误差
positiveCols=find(Y==1);        % 正训练样本标号
negativeCols=find(Y==0);        % 负训练样本标号
weight=ones(cntSamples,1);       % 权值向量;列向量;一行代表一个样本的权重
weight(positiveCols)=1/(2*length(positiveCols));      % 正训练样例的初始权值
weight(negativeCols)=1/(2*length(negativeCols));      % 负训练样例的初始权值
weight=weight/(sum(weight));% 权值归一化
FeatureVector=feature(:,1);
positiveCols= find(Y==1);        % 正训练样本标号
negativeCols= find(Y==0);        % 负训练样本标号
pweight=weight(positiveCols);%正样本权重
nweight=weight(negativeCols);%负样本权重
pfeature=FeatureVector(positiveCols);%正样本特征
nfeature=FeatureVector(negativeCols);%负样本特征
%%%%%%%%%%%方法一%(自己编写的高斯核密度计算)%%%%%%%%%%%%%%%%%%
% poall=zeros(1,length(FeatureVector));
% FeatureVector=sort(FeatureVector);
% pfeature=sort(pfeature);
% h=0.25;%带宽设置
% for i=1:length(FeatureVector)
%     for j=1:length(pfeature)
%          poall(i)=poall(i)+exp(-((FeatureVector(i)-pfeature(j)).^2)/(2*h*h));
%     end
% end
% poall=poall./(length(pfeature)*h*sqrt(2*pi));
% [histpy,histpx]=hist(FeatureVector,64);  %%%%%方法一：直方图密度统计
% % bar(histpx,histpy/sum(histpy),'hist');
% % hold on 
% % plot(pfeature,po*0.4,'r');
% neall=zeros(1,length(FeatureVector));
% nfeature=sort(nfeature);
% for i=1:length(FeatureVector)
%     for j=1:length(nfeature)
%          neall(i)=neall(i)+exp(-((FeatureVector(i)-nfeature(j)).^2)/(2*h*h));
%     end
% end
% neall=neall./(length(FeatureVector)*h*sqrt(2*pi));
% bar(histpx,histpy/sum(histpy),'hist');
% hold on 
% plot(FeatureVector,poall)
% hold on 
% plot(FeatureVector,neall,'r')
%%%%%%%%%%%%%%%%%%%%方法二%（matlab自带函数ksdensity）%%%%%%%%%%%%%%%%%
% FeatureVector=sort(FeatureVector);
% [histfy,histfx]=hist(FeatureVector,64);
% pfeature=sort(pfeature);
% [pf,px]=ksdensity(pfeature);
% nfeature=sort(nfeature);
% [nf,nx]=ksdensity(nfeature);
% [histpy,histpx]=hist(pfeature,64);
% [histny,histnx]=hist(nfeature,64);
% % bar(histpx,histpy/sum(histpy),'hist');
% % hold on 
% plot(px,pf*0.08,'r')
% hold on 
% % bar(histnx,histny/sum(histny),'hist');
% bar(histfx,histfy/sum(histfy),'hist');
% hold on 
% plot(nx,nf*0.08,'g')
% %%%%%%%%%%%%%%%%%方法二：自己编写的直方图密度统计%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% num=32;%%%%%%
% maxfea=max(FeatureVector(:));
% minfea=min(FeatureVector(:));
% disval=(maxfea-minfea)/num;
% numpo=length(pfeature);
% numne=length(nfeature);
% po=zeros(1,num);
% for i=1:numpo
%       dis=ceil((pfeature(i)-minfea)/disval);
%       dis=max(1,dis);
%       dis=min(dis,num);
%       po(dis)=po(dis)+pweight(i);%正样本直方图
% end
% ne=zeros(1,num);
% for j=1:numne
%     di=ceil((nfeature(j)-minfea)/disval);
%     di=max(1,di);
%     di=min(di,num);
%     ne(di)=ne(di)+nweight(j);  %负样本直方图
% end
% 
% figure
% bar(1:num,po);
% % trapz(1:num,po)
% hold on 
% bar(1:num,ne,'r')
% % trapz(1:num,ne)
% hold on 
% trapz(1:num,po)+trapz(1:num,ne)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
poall=zeros(1,length(FeatureVector));
[FeatureVector,index]=sort(FeatureVector);
Y=Y(index);
weight=weight(index);
positiveCols= find(Y==1);        % 正训练样本标号
negativeCols= find(Y==0);        % 负训练样本标号
pweight=weight(positiveCols);%正样本权重
nweight=weight(negativeCols);%负样本权重
pfeature=FeatureVector(positiveCols);%正样本特征
nfeature=FeatureVector(negativeCols);%负样本特征
h=8.5;%带宽设置
for i=1:length(FeatureVector)
    poall(i)=kerneldistri(FeatureVector(i),pfeature);
end
% poall=poall./(length(pfeature)*h*sqrt(2*pi));
% poall=poall./(length(FeatureVector));
[histpy,histpx]=hist(FeatureVector,64);  %%%%%方法一：直方图密度统计
neall=zeros(1,length(FeatureVector));
for i=1:length(FeatureVector)
    neall(i)=kerneldistri(FeatureVector(i),nfeature);
end
% neall=neall./(length(FeatureVector)*h*sqrt(2*pi));
% neall=neall./(length(FeatureVector));
figure
% bar(histpx,histpy/sum(histpy),'hist');
hold on 
% plot(FeatureVector,poall)
% hold on 
plot(FeatureVector,neall,'r')
% trapz(FeatureVector,poall)
% trapz(FeatureVector,neall)
% trapz(FeatureVector,poall)+trapz(FeatureVector,neall)
