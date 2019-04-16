# AdaBoost算法 - littlely_ll的博客 - CSDN博客





2017年04月04日 13:16:34[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：491标签：[AdaBoost算法																[机器学习](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)](https://so.csdn.net/so/search/s.do?q=AdaBoost算法&t=blog)
个人分类：[机器学习](https://blog.csdn.net/littlely_ll/article/category/6601963)








基本常用的几种提升方法 
![这里写图片描述](https://img-blog.csdn.net/20170402175552039?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

而本次所说的就为AdaBoost方法。
## AdaBoost方法基本思想

AdaBoost方法是一种常用的统计学习方法，在分类问题中，它通过改变训练样本权重，学习多个分类器，并将这些分类器进行线性组合，以提高分类性能。实际上，这就是“三个臭皮匠顶个诸葛亮”的道理。 
![这里写图片描述](https://img-blog.csdn.net/20170402174920658?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## AdaBoost算法

对于提升方法，主要有两个问题：一是在每一轮如何改变训练数据的权值或概率分布；而是如何将弱分类器组合成一个强分类器。这在下面算法中将会介绍。

假定一个二分类数据集 


$T = \{(x_1,y_1),(x_2,y_2),...,(x_N,y_N)\}$

其中，实例$x_i \in X \subseteq R^n$，标记$y_i \in Y=\{-1,1\}$
**算法**： 

输入：训练数据集$T =  \{(x_1,y_1),(x_2,y_2),...,(x_N,y_N)\}$；弱学习算法； 

输出：最终分类器$G(x)$. 

（1）初始化训练数据的权值分布 


$D_1=(w_{11},...,w_{1i},...,w_{1N}), w_{1i}=\frac{1}{N},i=1,2,...,N$

（2）对m=1,2,…,M 

    （a）使用具有权值分布$D_m$的训练数据集学习，得到基本分类器 


$G_m(x) : X \rightarrow \{-1,1\}$

    （b）计算$G_m(x)$在训练数据集上的分类误差率 


$e_m = P(G_m(x_i) \ne y_i) = \Sigma_{i=1}^Nw_{mi}I(G_m(x_i)\ne y_i)$

    （c）计算$G_m(x)$的系数 


$\alpha_m = \frac{1}{2}log\frac{1-e_m}{e_m}$

    这里对数为自然对数 

    （d）更新训练数据集的权值分布 


$D_{m+1}=(w_{m+1,1},...,w_{m+1,i},...,w_{m+1,N})\\    w_{m+1,i}=\frac{w_{mi}}{Z_m}exp(-\alpha_my_iG_m(x_i)),i=1,2,...,N$

    这里$Z_m$为规范化因子 


$Z_m=\Sigma_{i=1}^Nw_{mi}exp(-\alpha_my_iG_m(x))$

    它使$D_{m+1}$成为一个概率分布。 

（3）构建基本分类器线性组合 


$f(x)=\Sigma_{m=1}^M\alpha_mG_m(x)$

得到最终分类器 


$G(x)=sign(f(x))=sign(\Sigma_{m=1}^M\alpha_mG_m(x))$
**算法说明**： 

步骤（1）中，首先假设训练数据集具有均匀分布的权值，即每个训练样本在基本分类器的学习中作用相同。 

步骤（2）AdaBoost反复学习基本分类器，在每一轮m=1,2,…,M顺序执行下列操作： 

（a）使用当前分布$D_m$加权的训练数据集，学习基本分类器$G_m(x)$。 

（b）计算基本分类器$G_m(x)$在加权训练集上的分类误差率： 


$e_m=P(G_m(x_i) \ne y_i) = \Sigma_{G_m(x_i) \ne y_i}w_{mi}$

这里，$w_{mi}$表示第m轮中第i个实例的权值，$\Sigma_{i=1}^Nw_{mi}=1.$这表明，$G_m(x)$在加权训练数据集上的分类误差率是被$G_m(x)$误分类样本的权值之和。 

（c）计算基本分类器$G_m(x)$的系数$\alpha_m$。$\alpha_m表示G_m(x)$在最终分类器中的重要性，当$e_m \le \frac{1}{2}时，\alpha_m\ge 0,$并且$\alpha_m随着e_m$的减小而增大，所以分类误差率越小的基本分类器在最终分类器中的作用越大。 

（d）更新训练集权值分布，新一轮权值可写成： 


$w_{m+1,i}=\{_{\frac{w_{mi}}{Z_m}e^{\alpha_m},G_m(x_i) \ne y_i}^{\frac{w_{mi}}{Z_m}e^{-\alpha_m}, G_m(x_i) = y_i}$

由此可知，被基本分类器$G_m(x)$误分类样本的权值得以扩大，而正确分类的样本权值得以缩小。因此，误分类样本将在下一轮学习中起更大作用。 

（3）线性组合f(x)实现M个基本分类器的加权表决。系数$\alpha_m$表示基本分类器$G_m(x)$的重要性，但是所有$\alpha_m$之和并不等于1.







