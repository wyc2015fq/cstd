# kmeans算法及其实现 - 李鑫o_O - CSDN博客

置顶2016年03月10日 19:49:47[hustlx](https://me.csdn.net/HUSTLX)阅读数：1181


## 1.1Kmeans算法理论基础

K均值算法能够使聚类域中所有样品到聚类中心距离平方和最小。其原理为：先取k个初始聚类中心，计算每个样品到这k个中心的距离，找出最小距离，把样品归入最近的聚类中心，修改中心点的值为本类所有样品的均值，再计算各个样品到新的聚类中心的距离，重新归类，修改新的中心点，直到新的聚类中心和上一次聚类中心差距很小时结束。此算法结果受到聚类中心的个数和聚类中心初次选择影响，也受到样品的几个性质及排列次序的影响。如果样品的几何性质表明它们能形成几块孤立的区域，则算法一般可以收敛。

## 1.2Kmeans算法实现步骤

①产生二维高斯数据，设置聚类中心数N

②随机取N个点作为聚类中心。

③计算其余样品到这N个聚类中心的距离，将他们归到最近的类，到所有的样品都归完类。

④计算各个类样品的平均值作为该类新的聚类中心，再计算所有样品到新的聚类中心的距离，把他们归到最近的类，如此反复，直到聚类中心不再变化为止。

1.3Kmeans算法编程实现

```
clear all;close all;clc;
% 第一组数据
mu1=[0 0 ];  %均值
S1=[.1 0 ;0 .1];  %协方差
data1=mvnrnd(mu1,S1,100);   %产生高斯分布数据
%第二组数据
mu2=[1.25 1.25 ];
S2=[.1 0 ;0 .1];
data2=mvnrnd(mu2,S2,100);
% 第三组数据
mu3=[-1.25 1.25 ];
S3=[.1 0 ;0 .1];
data3=mvnrnd(mu3,S3,100);
% 显示数据
plot(data1(:,1),data1(:,2),'b+');
hold on;
plot(data2(:,1),data2(:,2),'r+');
plot(data3(:,1),data3(:,2),'g+');
grid on;
%  三类数据合成一个不带标号的数据类
data=[data1;data2;data3]; 
N=3;%设置聚类数目
[m,n]=size(data);
pattern=zeros(m,n+1);
center=zeros(N,n);%初始化聚类中心
pattern(:,1:n)=data(:,:);
for x=1:N
    center(x,:)=data( randi(300,1),:);%第一次随机产生聚类中心
end
while 1
distence=zeros(1,N);
num=zeros(1,N);
new_center=zeros(N,n);
 
for x=1:m
    for y=1:N
    distence(y)=norm(data(x,:)-center(y,:));%计算到每个类的距离
    end
    [~, temp]=min(distence);%求最小的距离
    pattern(x,n+1)=temp;         
end
k=0;
for y=1:N
    for x=1:m
        if pattern(x,n+1)==y
           new_center(y,:)=new_center(y,:)+pattern(x,1:n);
           num(y)=num(y)+1;
        end
    end
    new_center(y,:)=new_center(y,:)/num(y);
    if norm(new_center(y,:)-center(y,:))<0.1
        k=k+1;
    end
end
if k==N
     break;
else
     center=new_center;
end
end
[m, n]=size(pattern);
 
%最后显示聚类后的数据
figure;
hold on;
for i=1:m
    if pattern(i,n)==1 
         plot(pattern(i,1),pattern(i,2),'r*');
         plot(center(1,1),center(1,2),'ko');
    elseif pattern(i,n)==2
         plot(pattern(i,1),pattern(i,2),'g*');
         plot(center(2,1),center(2,2),'ko');
    elseif pattern(i,n)==3
         plot(pattern(i,1),pattern(i,2),'b*');
         plot(center(3,1),center(3,2),'ko');
    elseif pattern(i,n)==4
         plot(pattern(i,1),pattern(i,2),'y*');
         plot(center(4,1),center(4,2),'ko');
    else
         plot(pattern(i,1),pattern(i,2),'m*');
         plot(center(4,1),center(4,2),'ko');
    end
end
grid on;
```

## 1.3Kmeans算法测试结果：

![lip_image001](http://images2015.cnblogs.com/blog/904258/201603/904258-20160310200220491-1235332863.png)![lip_image002](http://images2015.cnblogs.com/blog/904258/201603/904258-20160310200232632-1476893887.png)

a)高斯数对b)N=2

![lip_image003](http://images2015.cnblogs.com/blog/904258/201603/904258-20160310200422225-1014696230.png)![lip_image004](http://images2015.cnblogs.com/blog/904258/201603/904258-20160310200437882-830482214.png)

c) N=3d)N=4

![lip_image005](http://images2015.cnblogs.com/blog/904258/201603/904258-20160310200444897-1580262707.png)![lip_image006](http://images2015.cnblogs.com/blog/904258/201603/904258-20160310200451366-226116649.png)

e)N=5f)N=6

可以看到聚类数目N对聚类有一定影响，同时在N相同的情况下每次的聚类结果也不完全一样，说明初始的聚类中心对聚类结果也有一定影响。

