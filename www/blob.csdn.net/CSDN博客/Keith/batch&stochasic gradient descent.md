# batch&stochasic gradient descent - Keith - CSDN博客





2016年11月04日 20:42:55[ke1th](https://me.csdn.net/u012436149)阅读数：656
个人分类：[MachineLearning](https://blog.csdn.net/u012436149/article/category/6504450)









# stochastic gradient descent 和 batch gradient descent

水平有限,如有错误,请指正! 
**本文不是对梯度下降进行推倒,只是介绍两者的区别.**

以Linear Model 为例. 

注: 
$x_j^{(i)}$ 表示第i个样本的第j个特征的值
## batch gradient descent

batch gradient descent 是考虑了batch中所有样本求出来的 $\frac{\partial Loss}{\partial \theta_j}$

更新公式: 

Repeat until convergence{ 
$\theta_j := \theta_j-\alpha\frac{1}{m}\sum_{i=1}^m(y^{i}-h_{\theta}(x^{(i)}))x_j^{(i)}$

}
$\sum_{i=1}^m(y^{i}-h_{\theta}(x^{(i)}))x_j^{(i)}$:就是 $\sum_{i=1}^{m}\frac{\partial Loss^{(i)}}{\partial \theta_j}$

## stochastic gradient descent

stochastic gradient decent :首先从训练集中随机抽取一个样本，然后使用这个样本计算梯度 $\frac{\partial Loss^{(i)}}{\partial \theta_j}$，之后更新一次参数。 

Repeat until convergence{ 

  for i = 1 to m{ 
$\theta_j := \theta_j-\alpha(y^{(i)}-h_{\theta}(x^{(i)}))x_j^{(i)}$

  } 

}
$(y^{(i)}-h_{\theta}(x^{(i)}))x_j^{(i)}$ 就是 $\frac{\partial Loss^{(i)}}{\partial \theta_j}$






