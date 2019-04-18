# 最大熵模型MaxEnt - Orisun - 博客园







# [最大熵模型MaxEnt](https://www.cnblogs.com/zhangchaoyang/articles/7100511.html)





### 最大熵模型

最大熵模型的思想是在满足已知条件的情况下，不对未知情况做任何有偏的假设，这样得到的模型预测风险最小。

满足已知条件，且不对未知情况做任何有偏的假设，即认为未知情况各种事件发生的概率相等，这实际上就是说条件熵最大。

$$max \; H(Y|X)=-\sum_i{\sum_j{p(x_i,y_j)logp(y_j|x_i)}}$$

 注：本文我们讨论的$x,y$都是离散随机变量。

定义特征函数：

$$f(x,y)=\left\{\begin{matrix} 1 & if  \; x,y满足某个条件 \\ 0 & otherwise \end{matrix}\right.$$

举两个特征函数的例子：

$$f_1(x,y)=\left\{\begin{matrix} 1 & if  \; x>3且y="张三" \\ 0 & otherwise \end{matrix}\right.$$

$$f_2(x,y)=\left\{\begin{matrix} 1 & if  \; x=0且y="李四" \\ 0 & otherwise \end{matrix}\right.$$

x和y的联合分布的经验分布：

$$\tilde{p}(x=x_i,y=y_j)=\frac{count(x=x_i,y=y_j)}{N}$$

$x,y$都是随机变量，$x_i,y_j$是其具体的取值，$N$是样本的总量。

x的经验分布：

$$\tilde{p}(x=x_i)=\frac{count(x=x_i)}{N}$$

特征函数$f$关于$\tilde{p}(x,y)$的期望：

$$E_{\tilde{p}}(f)=\sum_i{\sum_j{\tilde{p}(x_i,y_j)f(x_i,y_j)}}=\frac{1}{N}\sum_i{\sum_j{f(x_i,y_j)}}$$

特征函数$f$关于$p(x,y)$的期望：

$$E_{p}(f)=\sum_i{\sum_j{p(x_i,y_j)f(x_i,y_j)}} \approx \sum_i{\sum_j{\tilde{p}(x_i)p(y_j|x_i)f(x_i,y_j)}}$$

带特征函数的最大熵模型：

$$min \; -H(y|x)=\sum_i{\sum_j{\tilde{p}(x_i)p(y_j|x_i)logp(y_j|x_i)}}$$

$$s.t.\left\{\begin{matrix}\sum_j{p(y_j|x_i)}=1 & \forall{i} \\ E_{p}(f_k)=E_{\tilde{p}}(f_k) & \forall{k} \end{matrix}\right.$$

### 模型求解

对于带若干等式约束的凸优化问题，利用[拉格朗日乘子法](http://www.cnblogs.com/zhangchaoyang/articles/2726873.html)得到最大熵模型的等价问题为：

\begin{equation} \underset{p}{arg \; min}L(p;w,\lambda)=\sum_i{\sum_j{\tilde{p}(x_i)p(y_j|x_i)logp(y_j|x_i)}}+\sum_i{w_i\left(1-\sum_j{p(y_j|x_i)}\right)}+\sum_k{\lambda_k\left[\sum_i{\sum_j{\tilde{p}(x_i,y_j)f_k(x_i,y_j)}}-\sum_i{\sum_j{\tilde{p}(x_i)p(y_j|x_i)f_k(x_i,y_j)}}\right]}  \label{L} \end{equation}

其中$w_i\ne 0,\lambda_k\ne 0,\forall{i},\forall{k}$

这里的$p$指代的就是$p(y|x)$。

[KKT条件](http://www.cnblogs.com/zhangchaoyang/articles/2726873.html)指出上述问题与$\underset{w,\lambda}{max} \; \underset{p}{min} \; L(p;w,\lambda)$等价。下面就分step1和step2两步走，先调$p$求极小，再调$w,\lambda$求极大。

**Step 1**

先把$w,\lambda$当成常量，调整$p$求$L(p;w,\lambda)$的极小值，所以对$p$求导，令导数为0。

$$\frac{\partial L(p;w,\lambda)}{\partial p(y_j|x_i)}=\tilde{p}(x_i)[logp(y_j|x_i)+1]-w_i-\sum_k{\lambda_k\tilde{p}(x_i)f_k(x_i,y_j)}$$

$$=\tilde{p}(x_i)\left[logp(y_j|x_i)+1-\frac{w_i}{\tilde{p}(x_i)}-\sum_k{\lambda_kf_k(x_i,y_j)}\right]=0$$

$$\therefore p(y_j|x_i)=exp\left\{-1+\frac{w_i}{\tilde{p}(x_i)}+\sum_k{\lambda_kf_k(x_i,y_j)}\right\}=\frac{exp\left\{\sum_k{\lambda_kf_k(x_i,y_j)}\right\}}{exp\left\{1-\frac{w_i}{\tilde{p}(x_i)}\right\}}$$

$$\because \sum_jp(y_j|x_i)=1$$

$$\therefore \frac{\sum_jexp\left\{\sum_k{\lambda_kf_k(x_i,y_j)}\right\}}{\sum_jexp\left\{1-\frac{w_i}{\tilde{p}(x_i)}\right\}}=\frac{\sum_jexp\left\{\sum_k{\lambda_kf_k(x_i,y_j)}\right\}}{exp\left\{1-\frac{w_i}{\tilde{p}(x_i)}\right\}}=1$$

$$\therefore exp\left\{1-\frac{w_i}{\tilde{p}(x_i)}\right\}=\sum_jexp\left\{\sum_k{\lambda_kf_k(x_i,y_j)}\right\}$$  

\begin{equation} \therefore p(y_j|x_i)=\frac{exp\left\{\sum_k{\lambda_kf_k(x_i,y_j)}\right\}}{\sum_jexp\left\{\sum_k{\lambda_kf_k(x_i,y_j)}\right\}} \label{p} \end{equation}

我们看到(\ref{p})式就是softmax函数的一般化，只不过经典softmax函数中的特征$x$在这里变成了特征函数$f(x,y)$。

**Step 2**

令

$$Z_i=\sum_j{exp\left\{\sum_k{\lambda_kf_k(x_i,y_j)}\right\}}$$

由于已经对$j$和$k$进行了求和，所以$Z_i$只跟$i$有关系，即$Z_i=\sum_jZ_i=\sum_kZ_i$

由(\ref{p})式得

$$p(y_j|x_i)=\frac{exp\left\{\sum_k{\lambda_kf_k(x_i,y_j)}\right\}}{Z_i}$$

\begin{equation} logp(y_j|x_i)=\sum_k{\lambda_kf_k(x_i,y_j)}-logZ_i \label{log} \end{equation}

由(\ref{p})式得

\begin{equation} \sum_jp(y_j|x_i)=1 \label{s1} \end{equation}

把(\ref{s1})式和(\ref{log})式代入(\ref{L})式得

$$\underset{w,\lambda}{arg \; max} \; L(w,\lambda;p)=\sum_i{\sum_j{\tilde{p}(x_i)p(y_j|x_i)\left[\sum_k{\lambda_kf_k(x_i,y_j)}-logZ_i\right]}}+\sum_k{\lambda_k\left[\sum_i{\sum_j{\tilde{p}(x_i,y_j)f_k(x_i,y_j)}}-\sum_i{\sum_j{\tilde{p}(x_i)p(y_j|x_i)f_k(x_i,y_j)}}\right]}$$

$$=\sum_i\sum_j\sum_k\tilde{p}(x_i,y_j)\lambda_kf_k(x_i,y_j)-\sum_i\sum_j\tilde{p}(x_i)p(y_j|x_i)logZ_i$$

$$=\sum_i\sum_j\sum_k\tilde{p}(x_i,y_j)\lambda_kf_k(x_i,y_j)-\sum_i\tilde{p}(x_i)logZ_i\sum_jp(y_j|x_i)$$

$$=\sum_i\sum_j\sum_k\tilde{p}(x_i,y_j)\lambda_kf_k(x_i,y_j)-\sum_i\tilde{p}(x_i)logZ_i$$

最后$w$已经消失，只剩下$\lambda$了。有时候还会针对$\lambda$加一个正则项。求函数的极大值可以用梯度下降法、牛顿法等，也可以用专业的GIS、IIS法。












