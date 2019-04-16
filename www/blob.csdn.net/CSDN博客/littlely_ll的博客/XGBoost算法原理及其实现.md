# XGBoost算法原理及其实现 - littlely_ll的博客 - CSDN博客





2017年04月08日 14:46:17[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：2082








# 目标函数



$Obj(\theta)=L(\theta)+\Omega(\theta)$

其中，$L(\theta)$表示模型拟合训练数据的程度，$\Omega(\theta)$是正则化项，用来表示模型的复杂程度。

一般，训练集的损失函数记为：$L=\Sigma_{i=1}^nl(y_i,\hat{y_i})$

 - 平方损失函数：$l(y_i,\hat{y_i})=(y_i,\hat{y_i})^2$

 - 对数损失函数：$l(y_i,\hat{y_i})=y_iln(1+e^{-\hat{y_i}})+(1-y_i)ln(1+e^{\hat{y_i}})$
正则化： 

 - L2正则化：$\Omega(\omega)=\lambda||\omega||^2$

 - L1正则化：$\Omega(\omega)=\lambda||\omega||_1$
优化训练损失函数能保证模型具有预测性，而优化正则化项能是模型更简单。

# 回归树的集成

模型： 

假设我们有K个树，则 


$\hat{y_i}=\Sigma_{k=1}^Kf_k(x_i), f_k \in F$
$F$为包含所有回归树的函数空间。 

如下图： 
![这里写图片描述](https://img-blog.csdn.net/20170408104655233?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
每一个人代表一个样例，而每个样例的总得分是由每个树在该样例得分加总得到的。所以$f_k(x_i)$表示在树$f_k$中样本$x_i$的得分。

# 学习函数

![这里写图片描述](https://img-blog.csdn.net/20170408105722208?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

从第一张图我们看出，这是在话题k上用户兴趣随着时间的变化，在第二张图中，我们训练了一个模型，有非常多的切分点，这样虽然能很好的拟合数据，但是模型的复杂度上升了，这样可能导致大的方差；在第三张图中由于分割点错误，模型拟合不够，会造成较大的偏差，而最后一张图中在方差和偏差之间找到了较好的平衡。但是要怎样才能找到这样的平衡呢。
# 梯度提升

目标函数：$\Sigma_{i=1}^nl(y_i,\hat{y_i})+\Sigma_k\Omega(f_k), f_k \in F$

首先从一个常数预测开始，每次加一个函数： 


$\hat{y}_i^{(0)} = 0\\\hat{y}_i^{(1)} = f_1(x_i)=\hat{y}_i^{(0)}+f_1(x_i)\\\hat{y}_i^{(2)}=f_1(x_i)+f_2(x_i)=\hat{y}_i^{(1)}+f_2(x_i)\\...\\\hat{y}_i^{(t)}=\Sigma_{k=1}^tf_k(x_i)=\hat{y}_i^{(t-1)}+f_t(x_i)$
在第t轮预测中$\hat{y}_i^{(t)}=\hat{y}_i^{(t-1)}+f_t(x_i)$，而这个$f_t(x_i)$就是在第t轮我们需要决定的。



$Obj^{(t)}=\Sigma_{i=1}^nl(y_i,\hat{y_i}^{(t)})+\Sigma_{i=1}^t\Omega(f_i)\\=\Sigma_{i=1}^nl(y_i,\hat{y_i}^{(t-1)}+f_t(x_i))+\Omega(f_t)+constant$

我们的目标就是找到$f_t$使得$\Sigma_{i=1}^nl(y_i,\hat{y_i}^{(t-1)}+f_t(x_i))+\Omega(f_t)$最小。 

如果我们考虑平方损失函数，则 


$Obj^{(t)}=\Sigma_{i=1}^n(y_i-(\hat{y_i}^{(t-1)}+f_t(x_i)))^2+\Omega(f_t)+const\\=\Sigma_{i=1}^n[2(\hat{y_i}^{(t-1)}-y_i)f_t(x_i)+f_t(x_i)^2]+\Omega(f_t)+const$

现在目标函数看起来仍非常复杂。那么，我们对目标函数进行泰勒展开式展开 

二阶泰勒展开式为 


$f(x+\triangle x)\simeq f(x)+f'(x)\triangle x+1/2f''(x)\triangle x^2$

定义： 


$g_i=\partial_{\hat{y}^{(t-1)}}l(y_i,\hat{y}^{(t-1)}),h_i=\partial_{\hat{y}^{(t-1)}}^2l(y_i,\hat{y}^{(t-1)})$
$g_i,h_i$分别是$l(y_i,\hat{y_i}^{(t-1)})$对$\hat{y}^{(t-1)}$的一阶和二阶求导。 

在这里，$f(x)=l(y_i,\hat{y}^{(t-1)})$，而由$\hat{y}_i^{(t)}=\hat{y}_i^{(t-1)}+f_t(x_i)$可知 


$\triangle x=f_t(x_i)=\hat{y}_i^{(t)}-\hat{y}_i^{(t-1)}$

则目标函数可以写作： 


$Obj^{(t)}\simeq \Sigma_{i=1}^n[l(y_i,\hat{y_i}^{(t-1)})+g_if_t(x_i)+1/2h_if_t^2(x_i)]+\Omega(f_t)+const$

当目标函数去掉常数项时为 


$Obj^{(t)}\simeq \Sigma_{i=1}^n[g_if_t(x_i)+1/2h_if_t^2(x_i)]+\Omega(f_t)$

其中$g_i=\partial_{\hat{y}^{(t-1)}}l(y_i,\hat{y}^{(t-1)}),h_i=\partial_{\hat{y}^{(t-1)}}^2l(y_i,\hat{y}^{(t-1)})$
# 重新定义树



$f_t(x)=w_{q(x)}, w\in R^T, q:R^d\rightarrow \{1,2,...,T\}$
$q$代表每棵树的结构，它把一个样例映射到对应的叶结点上，$q(x)$即代表样例x在第几个叶节点上，$T$是叶结点的个数，$w_{q(x)}$为树的第$q(x)$个叶结点的权重得分 
![这里写图片描述](https://img-blog.csdn.net/20170408130737258?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 定义树的复杂度



$\Omega=\gamma T + 1/2\lambda\Sigma_{j=1}^Tw_j^2$

右边第一项$T$为叶结点的个数，第二项为L2正则化项。当然这个复杂度函数并不是唯一的。 

下图展现了怎样计算复杂度 
![这里写图片描述](https://img-blog.csdn.net/20170408132005538?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 再次回顾目标函数

定义叶结点$j$的样例集为$I_j=\{i|q(x_i)=j\}$

这时，目标函数为 


$Obj^{(t)}\simeq \Sigma_{i=1}^n[g_if_t(x_i)+1/2h_if_i^2(x_i)]+\Omega(f_t)\\=\Sigma_{i=1}^n[g_iw_{q(x_i)}+1/2h_iw^2_{q(x_i)}]+\gamma T+\lambda\frac{1}{2}\Sigma_{j=1}^Tw_j^2\\=\Sigma_{j=1}^T[(\Sigma_{i\in I_j}g_i)w_j+1/2(\Sigma_{i\in I_j}h_i+\lambda)w_j^2]+\gamma T$
$\Sigma_{i\in I_j}$为样例$i$在叶结点$j$的集合，而$\Sigma_{j=1}^T$为所有的叶结点，故$\Sigma_{i=1}^n=\Sigma_{j=1}^T\Sigma_{i\in I_j}$，这样就得到最后一步了。
# 结构得分及其计算

定义$G_j=\Sigma_{i\in I_j}g_i,H_j=\Sigma_{i\in I_j}h_i$，则 


$Obj^{(t)}=\Sigma_{j=1}^T[G_jw_j+1/2(H_j+\lambda)w_j^2]+\gamma T$

假设树的结构（$q(x)$）固定，则每个叶结点的最优权重和目标函数值为： 


$w_j^*=-\frac{G_j}{H_j+\lambda},Obj=-1/2\Sigma_{j=1}^T\frac{G_j^2}{H_j+\lambda}+\gamma T$

这里的目标函数可被用作得分函数，用来测量树结构q的性能。
![这里写图片描述](https://img-blog.csdn.net/20170408134518465?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

从上图，我们只需要计算一阶和二阶导数，然后加总并应用到得分方程。
# 树的贪婪学习

实际上，树的生长是贪婪的 

 * 首先从树的深度为0开始，即从根节点开始，枚举所有的特征； 

 * 对每一个特征，按照特征值对样例排序 

 * 对树的每一个叶结点，增加一个切割点，在增加切割点后，目标函数的变化为： 


$Gain=\frac{1}{2}[\frac{G_L^2}{H_L+\lambda}+\frac{G_R^2}{H_R+\lambda}-\frac{(G_L+G_R)^2}{H_L+H_R+\lambda}]-\gamma$

右式第一项为左子节点得分，第二项为右子节点得分，第三项为分割前的得分，$\gamma$为增加额外节点的复杂度。 

 * 从左向右线性扫描排序后的样例，决定最佳的特征分割节点。
时间复杂度为：$O(ndKlogn)$

对每一水平，需要$O(nlogn)$来排序，有d个特征和K个水平。

# 处理分类变量

xgBoost偏好处理稀疏矩阵，对于分类变量，我们需要进行[one-hot编码](https://en.wikipedia.org/wiki/One-hot)。对分类变量进行one-hot编码后矩阵就会变得非常稀疏，这样xgBoost就容易处理。

# 总结
- 每次迭代增加一颗新树
- 开始每次迭代，计算 


$g_i=\partial_{\hat{y}^{(t-1)}}l(y_i,\hat{y}^{(t-1)}),h_i=\partial_{\hat{y}^{(t-1)}}^2l(y_i,\hat{y}^{(t-1)})$
- 生成树$f_t(x)$


$Obj = 1/2\Sigma_{j=1}^T\frac{G_j^2}{H_j+\lambda}+\gamma T$
- 
把$f_t(x)$加到模型$\hat{y_i}^{(t)}=\hat{y_i}^{(t-1)}+f_t(x_i)$

> - 通常我们用$\hat{y_i}^{(t)}=\hat{y_i}^{(t-1)}+\epsilon f_t(x_i)$代替上式
- $\epsilon$称为步长，这样能防止过度拟合。



# 算法

![这里写图片描述](https://img-blog.csdn.net/20170408143300019?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# R语言实现

使用的数据为kaggle竞赛的[手写字体识别](https://www.kaggle.com/c/digit-recognizer/data)
**注意：数据由int型必须转换为numeric，否则在训练时会出现错误**，请看 [stackoverflow](https://stackoverflow.com/questions/43359154/error-in-matrixunlistout-ncol-times-data-is-too-long-when-create-data-p)提问的问题。
`Error in xgb.DMatrix(train[, -1], label = train[, 1]) : REAL() can only be applied to a 'numeric', not a 'integer'`
```
library(data.table)
library(caret)
library(xgboost)
digit_train <- fread("train.csv",header=T)
digit_test <- fread("test.csv",header=T)
index <- createDataPartition(digit$label,p=0.75,list=F)
train <- digit[index]
test <- digit[-index]

xgb <- xgboost(data=as.matrix(apply(digit_train[,-1],2,as.numeric)),##如果不转化为numeric，会出现上述错误
               label=as.numeric(digit_train$label),eta=0.1,nrounds=50,
               booster="gbtree",
               num_class=10,
               objective = "multi:softmax",
               max_depth = 12,
              )
pred <- predict(xgb,as.matrix(apply(digit_test,2,as.numeric)))
write.csv(pred,"pred.csv",row.names=F)
```

最后的精确度大约为0.95，不算好，还可以再调整参数。

CRAN上的参考文档：[XGBoost R tutorial](https://cran.r-project.org/web/packages/xgboost/vignettes/xgboostPresentation.html)

**参考文献：**
[Introduction to Boosted Tree](http://homes.cs.washington.edu/~tqchen/pdf/BoostedTree.pdf)
[XGBoost: A Scalable Tree Boosting System](https://arxiv.org/abs/1603.02754)
[XGBoost](https://xgboost.readthedocs.io/en/latest/model.html)
[one-hot](https://en.wikipedia.org/wiki/One-hot)
[XGBoost-Python完全调参指南-参数解释篇](http://blog.csdn.net/wzmsltw/article/details/50994481)
[github](https://github.com/dmlc/xgboost)
[XGBoost package](https://cran.r-project.org/web/packages/xgboost/xgboost.pdf)















