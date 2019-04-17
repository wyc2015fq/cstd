# 【机器学习详解】SVM解回归问题 - 勿在浮砂筑高台 - CSDN博客





2016年04月12日 10:47:09[勿在浮砂筑高台](https://me.csdn.net/luoshixian099)阅读数：24123标签：[SVM回归																[机器学习](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)](https://so.csdn.net/so/search/s.do?q=SVM回归&t=blog)
个人分类：[Machine Learning](https://blog.csdn.net/luoshixian099/article/category/5618837)

所属专栏：[机器学习详解](https://blog.csdn.net/column/details/ml-theory.html)








**转载请注明出处：**[http://blog.csdn.net/luoshixian099/article/details/51121767](http://blog.csdn.net/luoshixian099/article/details/51121767)


$\color{Blue}{CSDN-勿在浮沙筑高台}$

对于SVM解分类二分类问题，及多分类问题，在上一篇文章已经详述[http://blog.csdn.net/luoshixian099/article/details/51073885](http://blog.csdn.net/luoshixian099/article/details/51073885)。本文将对SVM解回归问题，进行分析。
## **1.方法分析**

在样本数据集$(x_n,t_n)$中，$t_n$不是简单的离散值，而是连续值。如在[线性回归](http://blog.csdn.net/luoshixian099/article/details/50880432)中，预测房价的问题。与线性回归类似，目标函数是正则平方误差函数： 
![这里写图片描述](https://img-blog.csdn.net/20160411150722085)

在SVM回归算法中，目的是训练出超平面$y=w^Tx+b$，采用$y_n=w^Tx_n+b$作为预测值。为了获得稀疏解，即计算超平面参数$w,b$不依靠所有样本数据，而是部分数据（如在SVM分类算法中，支持向量的定义），采用$\epsilon-insensitive$ 误差函数–Vapnik,1995。 
$\epsilon-insensitive$ 误差函数定义为，如果预测值$y_n$与真实值$t_n$的差值小于阈值$\epsilon$将不对此样本点做惩罚，若超出阈值，惩罚量为$|y_n-t_n|-\epsilon$。 
![这里写图片描述](https://img-blog.csdn.net/20160411152556703)

下图为$\epsilon-insensitive$ 误差函数与平方误差函数的图形 
![这里写图片描述](https://img-blog.csdn.net/20160411152729502)
## **2.目标函数**

观察上述的$E_{\epsilon}$ 误差函数的形式,可以看到，实际形成了一个类似管道的样子，在管道中样本点，不做惩罚，所以被称为$\epsilon-tube$，如下图阴影红色部分 
![这里写图片描述](https://img-blog.csdn.net/20160411155459743)

采用$E_{\epsilon}$替代平方误差项，因此可以定义最小化误差函数作为优化目标： 
![这里写图片描述](https://img-blog.csdn.net/20160411153051069)

由于上述目标函数含有绝对值项不可微。我们可以转化成一个约束优化问题，常用的方法是为每一个样本数据定义两个松弛变量$\xi_n\geq0,\hat{\xi_n}\geq0$，表示度量$t_n$与$\epsilon-tube$的距离。 

如上图所示： 

当样本点真实值$t_n$位于管道上方时，$\xi_n>0$,写成表达式：$t_n>y(x_n)+\epsilon$时，$\xi_n>0，\hat\xi_n=0$； 

当样本点真实值$t_n$位于管道下方时，$\hat{\xi_n}>0$,写成表达式：$t_n<y(x_n)-\epsilon$时，$\hat{\xi_n}>0，\xi_n=0$； 

因此使得每个样本点位于管道内部的条件为： 

当$t_n$位于管道上方时，$\xi_n>0$，有$t_n-y(x_n)-\xi_n\leq\epsilon$

当$t_n$位于管道下方时，$\hat{\xi_n}>0$，有$y(x_n)-t_n-\hat{\xi}_n\leq\epsilon$

误差函数可以写为一个凸二次优化问题： 
![这里写图片描述](https://img-blog.csdn.net/20160412094305757)

约束条件： 
$\xi_n\geq0$
$\hat{\xi_n}\geq0$
$t_n-y(x_n)-\xi_n\leq\epsilon$
$y(x_n)-t_n-\hat{\xi}_n\leq\epsilon$

写成拉格朗日函数： 
![这里写图片描述](https://img-blog.csdn.net/20160412094523477)
## **3.对偶问题**

上述问题为极小极大问题：$min_{w,b,\xi_n,\hat{\xi_n}}\ max_{\mu_n,\hat{\mu_n},\alpha_n,\hat{\alpha_n}}L$与SVM分类分析方法一样，改写成对偶问题$max_{\mu_n,\hat{\mu_n},\alpha_n,\hat{\alpha_n}}\ min_{w,b,\xi_n,\hat{\xi_n}}L$；首先分别对$w,b,\xi_n,\hat{\xi_n}$求偏导数 
![这里写图片描述](https://img-blog.csdn.net/20160412100833783)

带回到拉格朗日函数中，化简得到只关于$\alpha_n,\hat{\alpha_n}$的函数，目标即最大化此函数。 
![这里写图片描述](https://img-blog.csdn.net/20160412095608718)

约束条件为： 
$0\leq\alpha_n\leq C$
$0\leq \hat{\alpha_n} \leq C$,其中$k(x_n,x_m)=(x_n)^Tx_m$为向量内积。 

下面考虑KKT条件： 
![这里写图片描述](https://img-blog.csdn.net/20160412100752782)

由式7.65,7.66知： 

当$\alpha_n\neq0$时，必有$\epsilon+\xi_n+y(x_n)-t_n=0$,这些点位于管道上方边界出，或者管道上面。 

当$\hat\alpha_n\neq0$时，必有$\epsilon+\xi_n-y(x_n)+t_n=0$,这些点位于管道下方边界出，或者管道下面。 

同时，由式7.65,7.66知，对于任意一个数据点，由于$\epsilon>0$，则$\alpha_n，\hat\alpha_n$不可能同时不为0，而且得到在管道内部的点，必然有$\alpha_n=0，\hat\alpha_n=0$。 
![这里写图片描述](https://img-blog.csdn.net/20160412103813821)
## **4.超平面计算：**

把$w$表达式带入到$y=w^Tx+b$得： 
![这里写图片描述](https://img-blog.csdn.net/20160412102230706)

由上述的分析，影响超平面参数的点为位于管道边界处，或者管道外面。 

关于b的计算，可以考虑在管道上方边界处一个点必然有： 
$\xi_n=0$
$\epsilon+\xi_n+y(x_n)-t_n=0$

联立解出： 
![这里写图片描述](https://img-blog.csdn.net/20160412102908058)
参考：PRML









