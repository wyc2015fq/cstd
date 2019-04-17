# 声学模型学习笔记（二） DNN - xmdxcsj的专栏 - CSDN博客





2016年10月08日 19:51:12[xmucas](https://me.csdn.net/xmdxcsj)阅读数：2901








## training

![这里写图片描述](https://img-blog.csdn.net/20161008195041504)

### training criteria

1.MSE 

对于回归任务，可以使用MSE（mean square error）准则: 


$J_{MSE}(W,b;o,y)=\frac{1}{2}(v^L-y)^T(v^L-y)$
$y$表示标注结果，$v$表示预测结果 

2.CE 

对于分类任务，可以使用CE（cross-entropy）准则: 


$J_{CE}(W,b;o,y)=-\sum_{i=1}^Cy_ilogv_i^L$

3.NLL 

对于输出概率只有对应标注的为1，其他的概率都是0，所以CE可以简化为NLL(negative log-likilihood)准则： 


$J_{NLL}(W,b;o,y)=-logv_c^L$

其中c表示标注的类别。
### bp training

假设使用CE准则，层数为$l=1,2,...,L$，每一层线性转化以后的输出为$z_t^l=W_t^l*v_t^{l-1}+b_t^l$，非线性转化以后的输出为$v_t^l$，误差为$e_t^l$. 

BP推导比较简单，直接结果如下： 

1.对于L层（最后一层） 


$\Delta _{W_t^L}J_{CE}(W,b;o,y)=\Delta _{z_t^L}J_{CE}(W,b;o,y)\frac{\partial z_t^L}{\partial W_t^L}=(v_t^L-y)(v_t^{L-1})^T$


$\Delta _{b_t^L}J_{CE}(W,b;o,y)=(v_t^L-y)$

2.其他层 


$\Delta _{W_t^l}J_{CE}(W,b;o,y)=[f'(z_t^l) \bullet e_t^l](v_t^{l-1})^T$


$\Delta _{b_t^l}J_{CE}(W,b;o,y)=f'(z_t^l) \bullet e_t^l$

其中误差$e_t^l$如下： 


$e_t^{L-1}=(W_t^L)^Te_t^L$


$e_t^{l-1}=(W_t^l)^T[f'(z_t^l) \bullet e_t^l]$
## tricks

### data preprocessing

对于训练集$(o^m,y^m) ,0\le m \lt M$，首先根据训练集计算均值和方差： 


$\mu_i=\frac{1}{M}\sum_{m=1}^Mo_i^m$


$\delta_i=\sqrt{\frac{1}{M}\sum_{m=1}^M(o_i^m-\mu_i)^2}$

然后训练集和测试集进行归一化： 


$\tilde{o}_i^m=\frac{o_i^m-\mu_i}{\delta_i}$

当特征的各个维度在同样的数值范围值归一化以后，可以进行更好的训练。
### initialization

可以使用均值为0、方差为$\delta_{W^{l+1}=\frac{1}{\sqrt{N_l}}}$的高斯分布，其中$N_l$表示连接到这个节点的入边个数。 

也可以使用RBM、DBM、autodecoder等高级方法进行模型初始化。

### weight decay

为了解决过拟合问题引入[正则化](http://blog.csdn.net/xmdxcsj/article/details/50286705)

### dropout

[dropout](http://blog.csdn.net/xmdxcsj/article/details/50286673)也是为了解决过拟合问题引入。

### batch size

使用梯度下降的训练方法包括BGD/SGD/MBGD三种形式，三者的关系[这片博客](http://www.cnblogs.com/maybe2030/p/5089753.html)写的比较清楚。 

BGD：更新一次参数使用全部训练集；目标函数是整体风险最小，收敛于全局最优；但是训练集较大的时候存在训练速度慢的问题。 
[SGD](https://en.wikipedia.org/wiki/Stochastic_gradient_descent)：更新一次参数使用一个训练数据；目标函数是单个样本风险最小（容易引入noise，反而有利于跳出局部最优），收敛于局部最优，当满足一定条件，可以接近全局最优；训练速度快(参数更新快)；不容易并行，而且会在最小值附近震荡。 

MBGH：更新一次参数使用minibatch的数据；介于BGD和SGD之间，方便并行；开始使用较小的batch size，后面使用较大的batch size
### sample randomization

BGD不受影响，SGD和MBGD需要使用，如果训练数据的随机采样做的不好的话（比如都是同一个人的语音），会导致模型训练沿着特定的方向严重走偏。

### momentum

如果当前模型参数的更新，不仅使用当前的梯度，还包含里之前梯度的信息（具备了全局的视角），那么收敛速度将会加快。引入了动量技术，梯度表示为： 


$\Delta W_t^l=\rho \Delta b_{t-1}^l+(1-\rho)\frac{1}{M_b}\sum_{m=1}^{M_b}\nabla_{W_t^l}J(W,b;o^m,y^m) $

其中$\rho$成为动量因子，使用SGD或者MBGD的时候，一般取值0.9-0.99。
### learning rate

开始较大，后面减小

### reproducibility and restartability

使用同样的random seed保证可以复现 

使用check-point，可以中断以后接着上次的结果继续训练

## 参考文献

《automatic speech recognition a deep learning approach》 chapter 4 








