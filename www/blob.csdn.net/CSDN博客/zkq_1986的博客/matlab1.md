# matlab1 - zkq_1986的博客 - CSDN博客





2016年07月20日 10:35:06[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：201








# 1 拟合

## 1.1 多项式拟合

p = poly(x,y,n)   %其中x、y都为数据向量，n为拟合多项式的阶数，p为拟合后多项式的系数向量




## 1.2 最小二乘非线性拟合

lsqcurvefit()

例子1：




function y=yuww(x,xd)                                  %  保存为yuww.m 文件，反正不要是[fit](https://www.baidu.com/s?wd=fit&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y3PvRYrH0LujuhmWc3mHcs0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EPHD4P1mvPHD4)

y=50*x(1)./(x(1)-x(2))*(exp(-x(2)*xd)-exp(-x(1)*xd));

--------------------------------------------------------------------------------------

运行以下：

clear;clc

x=1:31;

y=[1 1 8 14 30 11 8 7 17 11 5 4 0 8 7 2 4 2 5 2 1 6 2 4 5 10 1 3 3 0 6];

c0=[2,0.1];    %初始值

c=lsqcurve[fit](https://www.baidu.com/s?wd=fit&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1Y3PvRYrH0LujuhmWc3mHcs0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EPHD4P1mvPHD4)('yuww',c0,x,y)

plot(x,y,'r*-')

hold on

plot(x,yuww(c,x))




![](https://img-blog.csdn.net/20160720123013141)




# 2 命令解释

hold on：叠加图像，在off之前的命令都叠加进来

hold off：取消叠加图像






# 3 输出图像

## 3.1 设置图像的白边

set (gca,'position',[0.03,0.03,0.95,0.95]) 

顺序为左、下、右、上，是代表百分比。

![](https://img-blog.csdn.net/20160822161537798)



