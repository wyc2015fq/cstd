# PCA主成分分析Matlab代码实现 - 心纯净，行致远 - CSDN博客





2018年04月12日 21:43:30[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：2842标签：[数学建模																[主成分																[PPCA](https://so.csdn.net/so/search/s.do?q=PPCA&t=blog)
个人分类：[【数学建模】](https://blog.csdn.net/zhanshen112/article/category/7184378)







主成分分析是一种数据降维方法。

什么时候要降维呢？

> 
当需要对一个样本集中的样本进行排序时，可能有多个维度可以对这些样本进行评价，但维度过多会造成不变，因此我们希望能够用更少的维度评价，同时尽量减少评价的准确度损失。

主成分分析，简单来说，就是直接选择对评价结果贡献度较高的几个维度，或者直接去掉对评价结果贡献度较低的几个维度；（[公式参考](https://en.wikipedia.org/wiki/Principal_component_analysis)） 



## **PCA主成分分析Matlab实现**

数据要求： 

mn（除去表头）的矩阵，样本集大小为n，属性个数为m；或说有n个待评价目标，评价因子个数为m。

```matlab
%读入数据
x=xlsread('data');

%求算标准差
sd=std(x);

%对原始数据标准化
n=size(x,1);    %读出x第一维的长度
jj=ones(n,1);
jj=jj*sd;
x=zscore(x);

%对x数据进行主成分分析
[t,score,r]=princomp(x);

%%以下为绘图示例2例
%%例1
x=score(:,1:2);%取前两个主成分（的得分）
idx=kmeans(x,5);%用k-means法聚类（分为5类）
%绘图
str=num2str([1:n]');
figure(1),clf
plot(x(:,1),x(:,2),'o')
text(x(:,1),x(:,2)+.5,str,'fontsize',12)
hold on
for i=1:n
    if idx(i)==1
        plot(x(i,1),x(i,2),'o','markerfacecolor','b')
    elseif idx(i)==2
        plot(x(i,1),x(i,2),'o','markerfacecolor','g')
    elseif idx(i)==3
        plot(x(i,1),x(i,2),'o','markerfacecolor','c')
    elseif idx(i)==4
        plot(x(i,1),x(i,2),'o','markerfacecolor','r')
    else
        plot(x(i,1),x(i,2),'o','markerfacecolor','m')
    end
end

%%例2
x=score(:,1:3);%取前三个主成分（的得分）
x(:,3)=x(:,3)-min(x(:,3));
idx=kmeans(x,4);
%绘图（3D）
str=num2str([1:n]');
figure(1),clf
plot3(x(:,1),x(:,2),x(:,3),'o')
stem3(x(:,1),x(:,2),x(:,3))
text(x(:,1),x(:,2),x(:,3)+.5,str,'fontsize',12)
hold on
for i=1:n
    if idx(i)==1
        plot3(x(i,1),x(i,2),x(i,3),'o','markerfacecolor','b')
    elseif idx(i)==2
        plot3(x(i,1),x(i,2),x(i,3),'o','markerfacecolor','g')
    elseif idx(i)==3
        plot3(x(i,1),x(i,2),x(i,3),'o','markerfacecolor','c')    
    else
        plot3(x(i,1),x(i,2),x(i,3),'o','markerfacecolor','m')
    end
end
xlabel('PC1'),ylabel('PC2'),zlabel('PC3')
```
本文转载自：[点击打开链接](https://blog.csdn.net/dearrita/article/details/52202258)](https://so.csdn.net/so/search/s.do?q=主成分&t=blog)](https://so.csdn.net/so/search/s.do?q=数学建模&t=blog)




