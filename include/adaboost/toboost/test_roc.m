clear all;
clc;
aa=randn(100,100);
bb=randn(100,100)+0.0009;
feature=[aa;bb];
X=feature;
ya=ones(1,100);
yb=zeros(1,100);
label=[ya yb];
Y=label;
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
num=64;%%%%%%%划分的直方图区间数%%%%%%%%
maxfea=max(FeatureVector(:));
minfea=min(FeatureVector(:));
disval=(maxfea-minfea)/num;
numpo=length(pfeature);
numne=length(nfeature);
po=zeros(1,num);
for i=1:numpo
    dis=ceil((pfeature(i)-minfea)/disval);
    dis=max(1,dis);
    dis=min(dis,num);
    po(dis)=po(dis)+pweight(i);
end
ne=zeros(1,num);
for j=1:numne
    di=ceil((nfeature(j)-minfea)/disval);
    di=max(1,di);
    di=min(di,num);
    ne(di)=ne(di)+nweight(j);
end
for t=1:num
    score(t)=0.5*log(po(t)+eps)/(ne(t)+eps);
end
trainnum=length(FeatureVector(:));
for i=1:trainnum
    index=ceil((FeatureVector(i)-minfea)/disval);
    index=min(index,num);
    index=max(index,1);
    %     output(i)=(score(index)>0);
    output(i)=score(index);
end
close all;
AA=newroc(output,Y',weight);
% [F,T]=roc(Y,output);
% F=[0 F 1];
% T=[0 T 1];
% t1=trapz(F, T);
% plot(F,T);
% hold on;
BB=newroc(-output,Y',weight);
% [F2,T2]=roc(Y,-output);
% F2=[0 F2 1];
% T2=[0 T2 1];
% plot(F2,T2,'r');
% plot([0:0.05:1],[0:0.05:1],'g');
% % t2=trapz(F2, T2);
% t1
% t2
% t1+t2
if (0)
    sum(fliplr(F2)+F)
    sum(fliplr(T2)+T)
    
    n=3;
    F=rand(1, n);
    T=[1:n]/n;
    
    F=[0 F 1];
    T=[0 T 1];
    
    [F, I]=sort(F);
    T=T(I);
    t1=(F(2:end)-F(1:end-1))*(T(2:end)+T(1:end-1))'/2;
    
    F=F;
    T=1-T;
    [F, I]=sort(F);
    T=T(I);
    t2=(F(2:end)-F(1:end-1))*(T(2:end)+T(1:end-1))'/2;
    
    t1
    t2
    t1+t2
end