# 线性判别分析（LDA） - littlely_ll的博客 - CSDN博客





2017年05月09日 14:33:10[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：441标签：[机器学习																[LDA																[数据挖掘](https://so.csdn.net/so/search/s.do?q=数据挖掘&t=blog)
个人分类：[机器学习																[特征提取与选择](https://blog.csdn.net/littlely_ll/article/category/6911098)](https://blog.csdn.net/littlely_ll/article/category/6601963)

所属专栏：[特征提取与选择](https://blog.csdn.net/column/details/15615.html)





线性判别分析(LDA)是一种监督学习方法，和主成分分析（PCA）一样，其主要用来降维。有些资料也把LDA称为Fisher线性判别（FLD）。LDA在机器学习，图像识别，数据挖掘等领域有着广泛的应用。

LDA的基本思想就是：给定训练样本集，设法将样本投影到一条直线上，使得同类样本的投影点尽可能地接近，异类样本的投影点尽可能远离，在对新样本进行分类时，将其投影到相同的这条直线上，再根据投影点的位置来确定新样本的类别。当是二分类问题时，如下图： 
![这里写图片描述](https://img-blog.csdn.net/20170509133656335?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
假设用来区分二分类的直线（投影函数）为： 


$y==w^Tx$

LDA分类的一个目标是使得不同类之间的距离越大越好，同一类别之间的距离越小越好，先定义几个值： 

类别$i$的原始中心点，也就是类别$i$的均值为： 


$m_i=\frac{1}{n_i}\Sigma_{x \in D_i}x$
$D_i$为属于类别$i$的样本点。 

类别$i$投影后的中心点（均值）为： 


$\widetilde{m_i} = w^Tm_i$

衡量类别$i$投影后，类别点之间的分散程度（方差）为： 


$\widetilde{s_i} = \Sigma_{y\in Y_i}(y-\widetilde{m_i})^2$
$Y_i$为类别$i$投影后的样本点。 

同时考虑不同类之间的距离越大越好，同一类别之间的距离越小越好，则可最大化目标函数： 


$J(w)=\frac{|\widetilde{m_1}-\widetilde{m_2}|^2}{\tilde{s_1}^2+\tilde{s_2}^2}$

我们最大化$J(w)$就可以求出最优的w了。要求出w，可以使用拉格朗日乘数法，但是在使用拉格朗日乘数之前，我们需要把$J(w)$里的w单独提取出来。
定义一个投影前各分类分散程度的矩阵： 


$S_i = \Sigma_{i\in D_i}(x-m_i)(x-m_i)^T$

可以看出，如果样本点距离中心点越近，$S_i$趋近于0，这表示的是类内离散程度。
将$J(w)$分母2化为： 


$\tilde{s_i}=\Sigma_{x\in D_i}(w^Tx-w^Tm_i)^2\\=\Sigma_{x\in D_i}w^T(x-m_i)(x-m_i)^Tw\\=w^TS_iw$

而$\tilde{s_1}^2+\tilde{s_2}^2=w^T(S_1+S_2)w=w^TS_ww$，$S_w$就是类内离散度矩阵。 

同样，将$J(w)$分子化为： 


$|\widetilde{m_1}-\widetilde{m_2}|^2=w^T(m_1-m_2)(m_1-m_2)^Tw=w^TS_bw$
$S_b$表示类间的离散度矩阵。 

这样目标函数转化为： 


$J(w)=\frac{w^TS_bw}{w^TS_ww}$

这样就可以使用拉格朗日乘数了，但是还有一个问题，如果分子、分母是都可以取任意值的，那就会使得有无穷解，将分母限制为1（一个技巧），作为拉格朗日乘数的限制条件，代入得： 


$c(w)=w^TS_bw-\lambda(s^TS_ww-1)\\ \Rightarrow \frac{dc}{dw}=2S_bw-2\lambda S_ww=0\\ \Rightarrow S_bw=\lambda S_ww$
对于多分类，直接写出结论： 


$S_w=\Sigma_{i=1}^cS_i\\S_b=\Sigma_{i=1}^cn_i(m_i-m)(m_i-m)^T\\S_bw=\lambda S_ww$

其中：$c$为类别个数,$m$为样本总体的中心点（均值向量）。 

W的闭式解则是$S_w^{-1}S_b的c-1$个最大广义特征值所对应的特征向量组成的矩阵。](https://so.csdn.net/so/search/s.do?q=LDA&t=blog)](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)




