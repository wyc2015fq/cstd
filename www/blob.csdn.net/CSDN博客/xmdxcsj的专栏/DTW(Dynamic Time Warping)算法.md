# DTW(Dynamic Time Warping)算法 - xmdxcsj的专栏 - CSDN博客





2017年04月20日 19:47:29[xmucas](https://me.csdn.net/xmdxcsj)阅读数：2679








## 变量定义

1.warping function 


$F=c(1),c(2)...c(k)...c(K)$

其中$c(k)=(i(k),j(k))$

表示两个语音特征序列A和B之间的映射关系。 

2.time-normalized distance 


$D(A,B)=Min_F[\frac{\sum_{k=1}^K d(c(k))\cdot w(k)}{\sum_{k=1}^K w(k)}]$

其中$\sum w(k)$用来归一化不同路径的K大小不一致的问题，$d()$表示两个特征向量之间的距离。
## warping function的限制条件

![这里写图片描述](https://img-blog.csdn.net/20170420194650318?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

1.单调性 


$i(k-1)\le i(k)\ and\ j(k-1) \le j(k)$

2.连续性 


$i(k)-i(k-1) \le 1 \ and\ j(k)-j(k-1)\le 1$

由以上两个条件限制了只能向三个方向移动，向上/向右/向右上，而且每次只能移动一步 

3.边界条件 


$i(1)=1, j(1)=1\ and\ i(K)=I,j(K)=J$

4.窗口限制 


$|i(k)-j(k)|\le r$

加窗为了将对齐路径限制在对角线附近。 

5.斜率限制 


$P=n/m$

避免在横轴i方向或纵轴j方向走的太偏,当在一个方向走了m步以后，需要在对角线方向走n步。
## 权重系数w

有两种典型的计算方式 

1.对称形式 


$w(k)=(i(k)-i(k-1))+(j(k)-j(k-1))$


$N=\sum_{k=1}^K w(k)=I+J$

2.非对称形式 


$w(k)=(i(k)-i(k-1))$


$N=\sum_{k=1}^K w(k)=I$

在j方向也一样。 
**对称形式的效果更好。**
## DP求解

假设使用对称形式的权重系数，不使用斜率限制 

- 初始条件 


$g(1,1)=2d(1,1)$

- DP 


$g(i,j)=min[g(i,j-1)+d(i,j)，\ g(i-1,j-1)+2d(i,j)，\ g(i-1,j)+d(i,j)]$

其中窗口限制条件$j-r\le i\le j+r$

- 距离 


$D(A,B)=\frac{1}{I+J}g(I,J)$
算法流程图如下： 
![这里写图片描述](https://img-blog.csdn.net/20170420194706944?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## Reference

Dynamic Programming Algorithm Optimization for Spoken Word Recognition








