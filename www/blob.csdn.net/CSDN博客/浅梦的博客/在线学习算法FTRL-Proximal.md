# 在线学习算法FTRL-Proximal - 浅梦的博客 - CSDN博客





2017年10月09日 15:52:53[浅梦s](https://me.csdn.net/u012151283)阅读数：1660








本文根据Google2013年发表的论文《Ad Click Prediction: a View from the Trenches》总结。

# CTR预估中的在线学习算法

在广告点击率预估模型中，面临着特征维度巨大的问题，这导致了模型参数的存储空间极高，以及模型进行预测时的时间代价大的问题。

## Online gradient descnet(OGD)

在线梯度下降法类似于随机梯度下降，一次只处理一个样本，但不同的是不要求序列的样本满足独立同分布的性质。 

OGD方法在许多问题上均有不错的表现，但是获得的解不够稀疏。

## L1 OGD

在OGD目标函数中添加L1正则化项，可以带来一定的稀疏性，使得模型的参数接近零。但由于参数的更新是两个浮点数进行相加，很难有机会产生精确零解。

## 截断梯度法TG和FOBOS

FOBOS和TG方法能够引入稀疏性

> 
[Sparse Online Learning via Truncated Gradient](https://arxiv.org/abs/0806.4686)
[Efficient Learning using Forward-Backward Splitting](http://web.stanford.edu/~jduchi/projects/DuchiSi09b.pdf)


## 正则对偶平均RDA

RDA方法在准确率和稀疏性的权衡之间好过FOBOS

> 
[Dual Averaging Methods for Regularized Stochastic Learning and Online Optimization](https://www.microsoft.com/en-us/research/publication/dual-averaging-methods-for-regularized-stochastic-learning-and-online-optimization/?from=http://research.microsoft.com/apps/pubs/default.aspx?id=141578)


# FTRL-Proximal

## 参数更新策略

Follow The (Proximally) Regularized Leader 或FTRL-Proximal具有RDA的优秀精度以及FOBOS的稀疏性质。 

传统的OGD更新策略如下： 
$w_{t+1}=w_t-\eta_tg_t$，其中$\eta_t=\frac{1}{\sqrt{t}}$

FTRL-Priximal使用如下更新策略 
$w_{t+1}=\arg\min\limits_w({g_{1:t}\cdot w+\frac{1}{2}\sum\limits_{s=1}^t}\sigma_s||w-w_s||_2^2+\lambda_1||w||_1)$

其中$\sigma_s$是一个和学习率$\eta_t$相关的参数， 
$\sigma_{1:t}=\sum\limits_{s=1}^t\sigma_s=\frac{1}{\eta_t}$
$g_{1:t}=\sum\limits_{s=1}^tg_t$

公式中，若令$\lambda_1=0$，则和传统的梯度下降方法一致。 

若令$\lambda_1>0$则可以产生稀疏解。 

公式第一项保证向正确的方向更新，而使用历史累计梯度保证不会过早地将重要特征的参数约束为0。 

公式第二项要求新产生的权重不要偏离历史权重太远，即参数更新不要太激进。 

公式第三项保证稀疏性。
## 公式推导



$\begin{align}F(w)&=g_{1:t}\cdot w+\frac{1}{2}\sum\limits_{s=1}^t\sigma_s||w-w_s||_2^2+\lambda_1||w||_1\\&=g_{1:t}\cdot w+\frac{1}{2}\sum\limits_{s=1}^t\sigma_s(w^Tw-2w^Tw_s+w_s^Tw_s)+\lambda_1||w||_1\\&=g_{1:t}\cdot w+\frac{1}{2}\sum\limits_{s=1}^t\sigma_s(w^Tw)-\sum\limits_{s=1}^t\sigma_sw^Tw_s+\lambda_1||w||_1+constant\\&=(g_{1:t}-\sum\limits_{s=1}^t\sigma_sw_s)w+\frac{1}{2}\sum\limits_{s=1}^t\sigma_s(w^Tw)+\lambda_1||w||_1+const\end{align}$

令$z_t=g_{1:t}-\sum\limits_{s=1}^t\sigma_sw_s$并由$\frac{1}{\eta_t}=\sum\limits_{s=1}^t\sigma_s$, 
$F(w)=z_t\cdot w+\frac{1}{2\eta_t}||w||_2^2+\lambda_1||w||_1+(const)$

根据推导 
$z_t=z_{t-1}+g_t-(\frac{1}{\eta_t}-\frac{1}{\eta_{t-1}})w_t$

下面求解使得$F(w)$最小的$w$
$\partial_w F(w)=z_t+\frac{w}{\eta_t}+\lambda_1\partial_w|w|_1$

由于$||w||$在零点处不可导，使用次梯度方法。则$||w||_1$的次梯度如下， 


$ \partial_{w_j}|w_j|=\begin{cases}-1&w_j<0\\[-1,1]& w_j=0\\1&w_j>0\end{cases}$

则 


$ \partial_{w_i}F(w)=\begin{cases}z_{t,i}+\frac{w_i}{\eta_t}-\lambda_1&w_i<0\\[z_{t,i}+\frac{w_i}{\eta_t}-\lambda_1,z_{t,i}+\frac{w_i}{\eta_t}+\lambda_1]& w_i=0\\z_{t,i}+\frac{w_i}{\eta_t}+\lambda_1&w_i>0\end{cases}$

令$\partial_{w_i}F(w)=0$，并根据条件限制，有 


$ w_{t+1,i}=\begin{cases}(\lambda_1-z_{t,i})\eta_t&z_{t,i}>\lambda_1\\0& |z_{t,i}|\le\lambda_1\\(-\lambda_1-z_{t,i})\eta_t&z_{t,i}<-\lambda_1\end{cases}$

整理成原论文格式 


$ w_{t+1,i}=\begin{cases}0&|z_{t,i}|\le\lambda_1\\-\eta_t(z_{t,i}-sgn(z_{t,i})\lambda_1)&|z_{t,i}|>\lambda_1\end{cases}$

由最后的参数更新公式可知，实现的时候可以直接存储$-\eta_tz_t$，当$\lambda_1=0$时，存储的就是通常的梯度下降的参数。 

注意到如果$\eta=constant,\lambda_1=0$，该公式和原始OGD则等价，$w_{t+1}=-\eta z_t=-\eta\sum_{s=1}^tg_s$
## 逐维度的学习率

OGD理论建议采用全局的学习率$\eta=\frac{1}{\sqrt{t}}$，所有维度使用相同的学习率。 

FTRL建议每个维度采用的学习率如下 
$\eta_{t,i}=\frac{\alpha}{\beta+\sqrt{\sum_{s=1}^tg_{s,i}^2}}$， 

其中$\alpha$和$\beta$是超参数，论文提到$\alpha$可以通过渐进验证的方法得到，而$\beta$通常设置为1。 

这种方法考虑了数据在不同维度上的特征分布的不均匀性，如果包含w某一个维度特征的训练样本很少，每一个样本都很珍贵，那么该特征维度对应的训练速率可以独自保持比较大的值，每来一个包含该特征的样本，就可以在该样本的梯度上前进一大步，而不需要与其他特征维度的前进步调强行保持一致。
# Per-Coordinate FTRL-Proximal with L1 and L2 Regularization for Logistic Regression

## 目标函数$w_{t+1}=\arg\min\limits_w({g_{1:t}\cdot w+\frac{1}{2}\sum\limits_{s=1}^t}\sigma_s||w-w_s||_2^2+\lambda_1||w||_1+\frac{1}{2}\lambda_2||w||_2^2)$

![这里写图片描述](https://img-blog.csdn.net/20171009164340297?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## TensorFlow实现

[https://www.tensorflow.org/versions/r1.8/api_docs/python/tf/train/FtrlOptimizer](https://www.tensorflow.org/versions/r1.8/api_docs/python/tf/train/FtrlOptimizer)

```python
tf.train.FtrlOptimizer(self, learning_rate, learning_rate_power=-0.5, initial_accumulator_value=0.1, l1_regularization_strength=0.0, l2_regularization_strength=0.0, use_locking=False, name="Ftrl", accum_name=None, linear_name=None, l2_shrinkage_regularization_strength=0.0)
```

当“为0时，优化的目标函数为$w_{t+1}=\arg\min\limits_w({g_{1:t}\cdot w+\frac{1}{2}\sum\limits_{s=1}^t}\sigma_s||w-w_s||_2^2+\lambda_1||w||_1+\lambda_2||w||_2^2)$
**与paper区别为l2正则项前系数为1**。 

函数参数与论文对应关系|TensorFlow|Paper|
|----|----|
|learning_rate|alpha|
|initial_accumulator_value|approximately  beta|
|l1_regularization_strength=0.0|$\lambda_1$|
|l2_regularization_strength=0.0|$\lambda_2$|

参数说明：

# 大规模数据集上的内存节省策略

我们已经看到L1正则化可以在模型推断的时候节省大量内存，下面介绍一些训练期间节省内存的策略
- 概率特征包含 
- Posson Inclusion
- Bloom Filter Inclusion

- 使用更少的位数编码值 

论文提到使用FTRL算法时，大部分参数的值都在(-2,+2)之间，那么使用传统OGD算法使用的32或64位的浮点数存储方案就显得过大了。 

提出使用16位的浮点数保存数值，2位整数，13位小数，1位符号位。- 训练若干相似模型
- Single Value Strcture
- 使用样本数计算学习率
- 下采样训练集 

正样本全部采样，负样本以比例$\tau$采样，但在训练时候给予负样本$\frac{1}{\tau}$的权重

# 预测结果修正

由于模型假设的不准确，或隐藏特征在训练或测试期间没有出现，模型会产生系统偏差（预测的平均值和实际平均值有差距）。可以通过一些策略进行修正。 

如泊松回归$\tau(p)=\gamma p^k$

# 不成功的实验
- 渐进特征哈希
- Dropout
- 特征Bagging
- 特征向量归一化

参考资料

> 
Ad Click Prediction: a View from the Trenches.H. Brendan McMahan, Gary Holt, D. Sculley et al  [researchgate](https://www.researchgate.net/publication/262412214_Ad_click_prediction_a_view_from_the_trenches)
[各大公司广泛使用的在线学习算法FTRL详解](http://www.cnblogs.com/EE-NovRain/p/3810737.html)


其他的不错的资料

> 
[Online Learning算法理论与实践](https://tech.meituan.com/online-learning.html)
[【每周一文】Ad Click Prediction: a View from the Trenches(2013)](http://blog.csdn.net/fangqingan_java/article/details/51020653)
[FTRL算法在使用中需不需要通过Batch Model初始化？](https://www.zhihu.com/question/37866658)
[CTR预测算法之FTRL-Proximal](https://zhuanlan.zhihu.com/p/25315553)
[为什么FTRL比FOBOS更容易获得稀疏解？](https://www.zhihu.com/question/55468616)
[关于点击率模型，你知道这三点就够了](https://mp.weixin.qq.com/s?__biz=MzIyOTAyOTEyNw==&mid=400706652&idx=1&sn=5e583a0cb7801e4762f32bb74a259ed0&scene=0#wechat_redirect)









