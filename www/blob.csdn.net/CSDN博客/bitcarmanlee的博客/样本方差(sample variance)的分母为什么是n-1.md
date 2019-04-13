
# 样本方差(sample variance)的分母为什么是n-1 - bitcarmanlee的博客 - CSDN博客


2019年02月12日 17:44:07[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：105



## 1.相关的数学符号
为了说明上述问题，先定义如下数学符号
总体的均值为$\mu$
总体的方差为$\sigma^2$
样本为随机变量$x_1, x_2, \cdots, x_n$
样本的均值$\bar x$
样本的方差$s^2$
## 2.样本方差的定义
在各种概率统计的教材中，都有样本方差的定义：
$$
s^2 = \frac{1}{n-1} \sum_{i=1}^n(x_i - \bar x)^2
$$
大家第一眼看到这个公式估计都会有疑问:为什么分母是n-1而不是n?教科书上的解释也很清楚但也很简单：样本方差中分母为n-1的目的是为了让方差的估计是无偏估计(unbiased estimator)。那么问题在于：
为什么分母为n-1的时候方差的估计是无偏估计？
从数学公式上说，要证明方差的估计是无偏估计，即
$$
E(s^2) = \sigma^2
$$

## 3.公式推导
下面对公式进行一下简单推导
$$
s
$$
$$
2
$$
$$
\begin{aligned}
s^2  &amp; = \frac{1}{n}  \sum_{i=1}^n(x_i - \bar x)^2 =  \frac{1}{n}  \sum_{i=1}^n\left((x_i - \mu)^2 - (\mu - \bar x) \right) ^2 \\
&amp; = \frac{1}{n}  \sum_{i=1}^n(x_i - \mu)^2 - \frac{2}{n} \sum_{i=1}^n(x_i - \mu)(\mu - \bar x) + \frac{1}{n}  \sum_{i=1}^n(\mu - \bar x)^2 \\ 
&amp; =  \frac{1}{n}  \sum_{i=1}^n(x_i - \mu)^2 - 2(\bar x - \mu)(\mu - \bar x) + (\mu - \bar x)^2 \\
&amp; = \frac{1}{n}  \sum_{i=1}^n(x_i - \mu)^2 - (\mu - \bar x)^2 \\
&amp; \leq  \frac{1}{n}  \sum_{i=1}^n(x_i - \mu)^2
\end{aligned}
$$
从上面的推导可以看出，只有当$\bar x = \mu$时，等号才成立。否则一定有
$$
s^2 = \frac{1}{n}  \sum_{i=1}^n(x_i - \bar x)^2   \lt \frac{1}{n}  \sum_{i=1}^n(x_i - \mu)^2
$$
在上述的不等式中，$\frac{1}{n}  \sum_{i=1}^n(x_i - \mu)^2$是真正的方差。但是一般情况下，我们不知道整体的均值是多少，所以会通过样本的均值去代替整体的均值。从上面的推导过程来看，如果直接用样本的均值代替整体均值，对方差进行估计的时候会是有偏估计，会使估计的方差比真正的方差偏小。为了得到无偏估计的方差，所以要对上面的方差计算公式进行修正。最后修正的公式即为：
$$
s^2 = \frac{1}{n-1} \sum_{i=1}^n(x_i - \bar x)^2
$$

## 4.为什么修正以后的分母是n-1
由前面的推导可知
$$
\begin{aligned}
E(s^2) &amp;=   E\left(\frac{1}{n}  \sum_{i=1}^n(x_i - \bar x)^2 \right) \\
&amp; = E \left[ \frac{1}{n}   \sum_{i=1}^n (x_i)^2 -  \frac{2}{n} \sum_{i=1}^n x_i \bar x + \frac{1}{n}   \sum_{i=1}^n (\bar x)^2 \right] \\
&amp; = E \left[  \frac{1}{n}   \sum_{i=1}^n (x_i)^2 - \bar x ^ 2  \right] \\
&amp; = E  \left[  \frac{1}{n}   \sum_{i=1}^n (x_i)^2 \right] - E (\bar x  ^2) \\
&amp; = E (x_i ^ 2) - E (\bar x ^ 2) \\
&amp; = D(x_i ^ 2) + (Ex_i)^2 - \left( D(\bar x^2) + (E\bar x)^2 \right)
\end{aligned}
$$
容易有如下结论
$$
E(\bar x) = \bar x = E(x_i), D(\bar x) = \frac{1}{n}D[x](i = 1, 2, \cdots, n)
$$
继续对上面式子处理可知:
$$
E(s^2) = D(x) - \frac{1}{n}D(x) = \frac{n-1}{n}D(x) = \frac{n-1}{n} \sigma ^ 2
$$
所以有:
$$
\frac{n}{n-1} E(s^2) = \frac{n}{n-1} \times \frac{n-1}{n}D(x) = \sigma ^ 2
$$
最后可知样本方差修正以后的公式为:
$$
s
$$
$$
2
$$
$$
\begin{aligned}
s^2 &amp; = \frac{n}{n-1} \left( \frac{1}{n}  \sum_{i=1}^n(x_i - \bar x)^2 \right ) \\
&amp; = \frac{1}{n-1} \sum_{i=1}^n(x_i - \bar x)^2
\end{aligned}
$$


