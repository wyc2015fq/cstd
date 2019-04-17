# 【机器学习详解】AdaBoost算法原理 - 勿在浮砂筑高台 - CSDN博客





2016年06月20日 20:09:45[勿在浮砂筑高台](https://me.csdn.net/luoshixian099)阅读数：6132
所属专栏：[机器学习详解](https://blog.csdn.net/column/details/ml-theory.html)









转载请注明出处：**勿在浮沙筑高台**[http://blog.csdn.net/luoshixian099/article/details/51714346](http://blog.csdn.net/luoshixian099/article/details/51714346)

## **1.概念**

AdaBoost是一种级联算法模型，即把几个弱分类器级联到一起去处理同一个分类问题。也就是“三个臭皮匠顶一个诸葛亮”的道理。例如一个专家作出的判定往往没有几个专家一起作出的判定更准确。一种情况：如果每个专家都仅有一票的权利，采用投票机制的方法属于uniform形式；另一种情况是分配给每个专家的票数不一致则属于linear形式。AdaBoost即属于第二种的行式，同时尽量使得每个专家考虑的则重点不同，最终给出投票结果更可信。

## **2.原理描述**

给定一个数据集D(x,y)，x表述特征，y对应标签值。在某种分类器上训练出模型$G_1(x)$,下标*1*表示第一轮。第二轮上考虑上一轮$G_1(x)$上被误分类的那一部分数据即$y^{(i)}\neq sign(G_1(x^{(i)}))$，然后提高被误分类点的训练权重，降低被正确分类点的权重 ，被误分类的的数据由于权重的增大会受到下一轮分类器更大的关注，继续学习训练出模型$G_2(x)$；依此类推。最终级联上这M个分类器：$G(x)=sign\left(\sum_{m=1}^{M}\alpha_mG_m(x)\right)$,其中$\alpha_m$表示$G_m(x)$分类器所占的比重，直观上可以想到$\alpha_m$一定与分类器$G_m(x)$的性能有关，即分类误差率$\epsilon_m$有直接关系。$G_m(x)$的分类误差率$\epsilon_m$越低，$\alpha_m$越大。

> 
**下图中$w_n^{(1)}$表示第一轮训练样本的权重向量，$y_1(x)$表示第一轮训练出的模型，然后根据$y_1(x)$对样本数据分类情况，调整$w_n^{(2)}$，继续训练出$y_2(x)$….**


## ![这里写图片描述](https://img-blog.csdn.net/20160620150859569)

**数学语言描述**
- 训练数据集$D(x,y),x_i\in R^n$表示数据特征，$y_i\in \{-1,1\}$表示对应的标签值，共N个样本点。分类器$G_m$分类误差率定义为$\epsilon_m=\frac{\sum_{i=1}^{N}w_{mi}I(G_m(x_i)\neq y_i)}{\sum_{i=1}^{N}w_{mi}}$；
- 第一轮训练数据的权值：$w_{1i}=\frac{1}{N}$,i=1,…N,即同等对待每个样本点。学习出弱分类器$G_1(x)$。计算出$G_1(x)$的分类误差率$\epsilon_1=\frac{\sum_{i=1}^{N}w_{1i}I(G_1(x_i)\neq y_i)}{\sum_{i=1}^{N}w_{1i}}$;提高被$G_1(x)$误分类样本点的权值，降低被正确分类样本点的权值。

> 
现在讨论一下具体提高多少权值或者降低多少权值。AdaBoost的想法是让分类器$G_1(x)$在调整权重后的$w_{2i}$上表现出乱猜的效果。即$\frac{\sum_{i=1}^{N}w_{2i}I(G_1(x_i)\neq y_i)}{\sum_{i=1}^{N}w_{2i}}=\frac12$。由于每一轮训练的目标函数都是最小化误差函数，所以第二轮训练出的分类器与上一轮会不同。 

  具体的调整权值方法： 

  被错误分类的样本点：$w_{2i}=w_{1i}*\sqrt{\frac{1-\epsilon_1}{\epsilon_1}}$;被正确分类的点:$w_{2i}=w_{1i}\div \sqrt{\frac{1-\epsilon_1}{\epsilon_1}}$； 

  两种情况写在一起为$w_{2i}=w_{1i}*exp(-y_iG_1(x_i)log\sqrt{\frac{1-\epsilon_1}{\epsilon_1}})$

  由于$\epsilon_1\leq \frac12$，则$\sqrt{\frac{1-\epsilon_1}{\epsilon_1}}\geq1$,所以被错分的样本点权值会升高，相反，被正确分类的样本点权值会降低。
3.得到第二轮的训练样本权值$w_{2i}$,i=1,2,…N;继续训练样本分类器得到$G_2(x)$。重复上述步骤2，得到M个分类器。 

4.组合上述M个分类器：$G(x)=sign\left(\sum_{m=1}^{M}\alpha_mG_m(x)\right)$,其中$\alpha_m$表示每个分类器的权重，$\alpha_m=log \sqrt{\frac{1-\epsilon_m}{\epsilon_m}}$；关于$\alpha_m$的定义，如果分类器的误差率$\epsilon_m=\frac12$(误差率达到$\frac12$就相当于乱猜)，则$\alpha_m=0$。如果分类器的误差率$\epsilon_m=0$，则$\alpha_m=\infty$;

## **3.算法步骤**
- 初始化权重$w_1i=\frac1N$,i=1,2,…,N
- For m=1,…,M: 

 (a)训练分类器$G_m(x)$以最小化加权误差函数作为目标函数

$\epsilon_m=\frac {\sum_{i=1}^{N}w_{mi}I(G_m(x_i)\neq y_i)}{\sum_{i=1}^{N}w_{mi}}$

 (b)根据分类器误差$\epsilon_m$,计算此分类器的权重$\alpha_m=log \sqrt{\frac{1-\epsilon_m}{\epsilon_m}}$

 (c)更新下一轮样本权重$w_{m+1,i}=w_{mi}*exp(-y_iG_m(x_i)log\sqrt{\frac{1-\epsilon_1}{\epsilon_1}})$;由于$\alpha_m=log \sqrt{\frac{1-\epsilon_m}{\epsilon_m}}$，所以可以记为：

$w_{m+1,i}=w_{mi}*exp(-\alpha_my_iG_m(x_i))$- 联合上述M个分类器得： 


$G(x)=sign(\sum_{m=1}^{M}\alpha_mG_m(x))$

## **4.Adaboost与前向分布算法**

加法模型$f(x)=\sum_{m=1}^{M}\beta_mb(x,\gamma_m)$,其中$b(x,\beta_m)$为基函数，$\beta_m$为基函数系数。在给定训练数据及损失函数$L(y,f(x))$情况下，目标函数为

$min_{\beta_m,\gamma_m}\sum_{i=1}^{N}L \left( y_i,\sum_{m=1}^{M}\beta_mb(x_i;\gamma_m) \right)$

上述位置参数有2M个，可以采用前向分布算法，从前向后每一步只学习一个基函数及其系数逐步逼近上述目标函数。每一个只需优化下述损失函数：

$min_{\beta,\gamma}\sum_{i=1}^{N}L \left( y_i,\beta b(x_i;\gamma) \right)$
**前向分步算法**- 初始化$f_0(x)=0$
- 
对m=1,2…M 

(a)极小化损失：

$(\beta_m,\gamma_m)=min_{\beta,\gamma}\sum_{i=1}^{N}exp\left(-y_i(f_{m-1}(x_i)+\beta b(x_i,\gamma))\right)$

(b)更新：$f_m(x)=f_{m-1}(x)+\beta_mb(x;\gamma_m)$- 
得到加法模型$f(x)=\sum_{m=1}^{M}\beta_mb(x,\gamma_m)$


**下面证明Adaboost是前向分布算法的一个特例,基函数为分类器，误差函数为指数误差函数**。 

指数误差函数定义：$L(x,f(x))=\sum_{i=1}^{N}exp(-y_if(x_i))$

假设经过m-1轮迭代前向分布算法已经得到$f_{m-1}(x)$,下一步即的优化函数为:

$(\beta_m,\gamma_m)=min_{\beta,\gamma}\sum_{i=1}^{N}exp\left(-y_i(f_{m-1}(x_i)+\beta b(x_i,\gamma))\right)$替换成Adaboost的表示方法为：

$(\alpha_m,G_m)=min_{\alpha,G}\sum_{i=1}^{N}exp\left(-y_i(f_{m-1}(x_i)+\alpha G(x_i))\right)$

定义$w_{mi}=exp(-y_if_{m-1}(x_i))$,上式转变 
$=\sum_{i=1}^{N}w_{mi}exp\left(-y_i\alpha G(x_i)\right)$
$=\sum_{y_i=G(x_i)}w_{mi}e^{-\alpha} +\sum_{y_i\neq G(x_i)}w_{mi}e^{\alpha}$
$=\sum_{i=1}^{N}w_{mi}\left(\frac{\sum_{y_i=G(x_i)}w_{mi}}{\sum_{i=1}^{N}w_{mi}}e^{-\alpha}+\frac{\sum_{y_i\neq G(x_i)}w_{mi}}{\sum_{i=1}^{N}w_{mi}}e^{\alpha}\right)$
$=\sum_{i=1}^{N}w_{mi}\left((1-\epsilon_m) e^{-\alpha}+\epsilon_me^{\alpha}\right)$;其中$\epsilon_m=\frac{\sum_{y_i\neq G(x_i)}w_{mi}}{\sum_{i=1}^{N}w_{mi}}$
$=\sum_{i=1}^{N}w_{mi}\left(\epsilon_m(e^{\alpha}-e^{-\alpha})+e^{-\alpha})\right)$

固定$\alpha>0$,这样以指数函数作为损失函数的前向分布算法的每一步即最小化$\epsilon_m$. 
$\color{red}{\epsilon_m=\frac{\sum_{y_i\neq G(x_i)}w_{mi}}{\sum_{i=1}^{N}w_{mi}}与AdaBoost每一步分类器的目标函数相同}$。 

现在考虑参数$\alpha$的优化，对$\sum_{i=1}^{N}w_{mi}\left(\epsilon_m(e^{\alpha}-e^{-\alpha})+e^{-\alpha})\right)$求导，令为0得： 
$\epsilon_m(e^{\alpha}+e^{-\alpha})-e^{-\alpha}=0$

即$\alpha=log\sqrt \frac{1-\epsilon_m}{\epsilon_m}$

对应到AdaBoost的每个分类器的权重$\alpha_m$的计算方法。从而可知，AdaBoost算法优化方法实际上相当于前向分步算法。
考虑Adaboost最后的指数误差损失函数：$E(z)=e^{-z},z=yf(x)$;如下图绿色曲线，实际也是在对0-1损失函数(黑色)上限逼近；蓝色曲线对应SVM的合页损失；红色对应逻辑回归损失函数。 
![这里写图片描述](https://img-blog.csdn.net/20160620194626594)
*Reference:*

统计学习方法-李航 

PRML-M.BISHOP 
[http://www.loyhome.com/≪统计学习精要the-elements-of-statistical-learning≫课堂笔记（十四）/](http://www.loyhome.com/%E2%89%AA%E7%BB%9F%E8%AE%A1%E5%AD%A6%E4%B9%A0%E7%B2%BE%E8%A6%81the-elements-of-statistical-learning%E2%89%AB%E8%AF%BE%E5%A0%82%E7%AC%94%E8%AE%B0%EF%BC%88%E5%8D%81%E5%9B%9B%EF%BC%89/)











