# 高斯混合模型（GMM） - curryche的博客 - CSDN博客





2018年06月05日 09:20:36[curryche](https://me.csdn.net/whwan11)阅读数：76








高斯混合模型是由多个高斯子模型线性组合而成的模型，当数据的分布比较复杂时，用单一的高斯模型来表示数据的分布不能很好地反映数据分布的特点，如果用多个高斯子模型的线性组合显然更能准确的反映数据的分布。 


$P(y|\theta)=\sum\limits_{k=1}^{K}\alpha\phi(y|\theta_{k})$

其中，$\alpha_{k}\ge0$是系数，$\sum\limits_{k=1}^{K}\alpha_k=1$，$\phi(y|\theta_{k})$是高斯分布密度，$\theta_k=(\mu_k,\sigma_k^2)$是高斯模型的参数， 


$\phi(y|\theta_{k})=\frac{1}{\sqrt{2\pi}\sigma_k}exp(-\frac{(y-\mu_k)^2}{2\sigma_k^2})$

称为一个高斯子模型。



