# 提升树和GBDT - 浅梦的博客 - CSDN博客





2017年08月27日 20:10:15[浅梦s](https://me.csdn.net/u012151283)阅读数：617








# 提升树

## 提升树模型

以决策树为基函数的提升方法称为提升树(boosting tree)。 

对分类问题是二叉分类树，对回归问题是二叉回归树。 

提升树可以表示为决策树的加法模型： 
$f_M(x)=\sum\limits_{m=1}^MT(x;\Theta_m)$

其中，$T(x;\Theta_m)$表示决策树；$\Theta_m$为决策树的参数；M为树的个数。 
![这里写图片描述](https://img-blog.csdn.net/20170928123424301?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 提升树算法

提升树算法采用前向分步算法。首先确定初始提升树$f_0(x)=0$，第m步的模型是 
$f_m(x)=f_{m-1}(x)+T(x;\Theta_m)$

其中，$f_{m-1}(x)$为当前模型，通过经验风险极小化确定下一棵决策树的参数$\Theta_m$， 
$\hat\Theta_m=\arg\min\limits_{\Theta_m}=\sum\limits_{i=1}^NL(y_i,f_{m-1}(x_i)+T(x_i;\Theta_m))$

针对不同问题的提升树学习算法，主要区别在于使用的损失函数不同。回归问题使用平方误差损失函数，分类问题使用指数损失函数。一般决策问题使用一般损失函数。 

回归问题提升树使用以下前向分步算法： 


$\begin{align*}f_0(x)&=0\\f_m(x)&=f_{m-1}(x)+T(x;\Theta_m),m=1,...,M\\f_M(x)&=\sum\limits_{m=1}^MT(x;\Theta_m)\end{align*}$

在前向分步算法的m步，给定当前模型$f_{m-1}(x)$，需求解 
$\hat\Theta_m=\arg\min\limits_{\Theta_m}\sum\limits_{i=1}^NL(y_i,f_{m-1}(x_i)+T(x_i;\Theta_m))$

得到$\hat\Theta_m$，即第m棵树的参数。 

当采用平方误差损失函数时， 
$L(y,f(x))=(y-f(x))^2$

其损失变为 


$\begin{align*}L(y,&f_{m-1}(x)+T(x;\Theta_m)\\&=[y-f_{m-1}(x)-T(x;\Theta_m)]^2\\&=[r-T(x;\Theta_m)]^2\end{align*}$

这里， 
$r=y-f_{m-1}(x)$

是当前模型拟合数据的残差。 

所以，对回归问题的提升树算法来说，只需简单地拟合当前模型（前面所有树学到的模型）的残差。 
![这里写图片描述](https://img-blog.csdn.net/20170827200100632?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 梯度提升

提升树利用加法模型与前向分步算法实现学习的优化过程。当损失函数是平方损失和指数损失函数时，每一步优化是很简单的。但对一般损失函数而言，往往每一步优化并不那么容易。 

Freidman提出了梯度提升算法。这是利用最速下降法的近似算法，其关键是利用损失函数的负梯度在当前模型的值 
$-[\frac{\partial L(y_i,f(x_i))}{\partial f(x_i)}]_{f(x)=f_{m-1}(x)}$

作为回归问题提升树算法中的残差的近似值，拟合一个回归树。
# GBDT

## 数学公式

Gradient Tree Boosting or Gradient Boosted Regression Trees(GBRT)是一种适用于具有任意阶可导损失函数的提升方法。 

优点： 

处理不同类型数据 

预测力强 

对异常点具有鲁棒性（通过鲁棒的损失函数） 

缺点： 

可伸缩性Scalability。每棵树按顺序生成，很难并行。 

GBRT加性模型 
$F(x)=\sum\limits_{m=1}^M\gamma_mh_m(x)$

前向分步算法： 
$F_m(x)=F_{m-1}(x)+\gamma_mh_m(x)$

计算梯度 
$F_m(x)=F_{m-1}(x)+\gamma_m\sum\limits_{i=1}^n\nabla_FL(y_i,F_{m-1}(x_i))$

步长$\gamma_m$通过线性搜索确定 
$\gamma_m=\arg\min_{\gamma}L(y_i,F_{m-1}(x_i)-\gamma\frac{\partial L(y_i,F_{m-1}(x_i))}{\partial F_{m-1}(x_i)})$

分类和回归问题的区别在于使用的损失函数。
![这里写图片描述](https://img-blog.csdn.net/20170926211544423?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 损失函数

### 回归问题：

ls,最小二乘回归的损失函数，初始模型$F_0(x)$为目标值的均值

### 分类问题：

deviance,逻辑回归的损失函数，适用于二分类和多分类。当用于多分类时，每一步构建$\text{n_classes}$个回归树。当类别较多时，效率降低。

### exponential，指数损失函数，相当于二分类的Adaboost算法，仅用于二分类。

$L(y_i,F_{m-1}(x_i))=e^{-y_i*F_{m-1}(x_i)}$

损失函数的反向梯度为 
$-\frac{\partial(y_i,F_{m-1}(x_i))}{\partial F_{m-1}(x_i)}=y_i*e^{-y_i*F_{m-1}(x_i)}$
## 步长缩减

经验表明使用较小的学习率可以获得更好的测试表现。 

一般设置其为较小的常数（$lr\le0.1$），通过早停来选择基学习器的数量。更多内容可参考

> 
Ridgeway, “Generalized Boosted Models: A guide to the gbm package”, 2007 

  。


参考资料

> 
《统计学习方法》第8章 
[scikit-learn gradient-boosting](http://scikit-learn.org/stable/modules/ensemble.html#gradient-boosting)

  gbdt算法原理与系统设计简介 weapon









