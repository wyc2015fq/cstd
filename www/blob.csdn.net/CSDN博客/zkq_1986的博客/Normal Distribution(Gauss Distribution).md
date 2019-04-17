# Normal Distribution(Gauss Distribution) - zkq_1986的博客 - CSDN博客





2016年08月13日 14:59:47[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：205
个人分类：[统计学](https://blog.csdn.net/zkq_1986/article/category/6323679)









## 1

## 2

## 3 近似正态分布

中心极限定理Central Limit Theorem：设从均值为μ、方差为$σ^2$的任意一个**总体**中抽取样本量为n的样本，当n充分大时，**样本均值**的抽样分布近似服从均值为μ、方差为$σ^2/n$的正态分布。原来的分布**不一定要符合正态分布**，可以是任何的分布，可以是离散也可以是连续，即无要求。

假设总体的均值为μ，方差为$σ^2$；样本的均值为$μ_{\bar X}$，样本的方差为S；样本均值的均值同样本的均值为为$μ_{\bar X}$，样本均值的方差为$σ_{\bar X}^2$。那么有以下公式： 


$μ_{\bar X}=μ$


$σ_{\bar X}^2=\frac {σ^2}{n}$


$S \approx σ^2$
### 3.1 例题1

You sample 36 apples from you farm‘s harvest of over 200,000 apples. The mean weight of the sample is 112 grams (with a 40 gram sample standard deviation). What is the probability that the mean weight of all 200,000 apples is within 100 and 124 grams? 

这里给出一个样本集的情况，这36个样本（或者n=36的一次样本集采样中）$μ_{\bar X}$=112，这36个**样本**的σ=40。可以估算总体方差，还记得无偏差样本方差吗？是**除以n-1，而不是n**，**无偏差样本方差**，可以近似为总本方差σ。 
![这里写图片描述](https://img-blog.csdn.net/20160813145833149)

这道题目所求概率转换成以下形式： 
![这里写图片描述](https://img-blog.csdn.net/20160813145030107)

其中，z为标准正态分布，$σ_{\bar X}=\frac {σ}{n} \approx \frac {s}{n} = \frac {40}{6} = 6.67 $

p(z<=12/6.67)=p(z<=1.8)，查正态分布表知，p(z<=1.8)=0.9282 

p(0<=z<=1.8)=0.9282-1/2=… 

最后，所求概率p=2*p(0<=z<=1.8)=92.3%。




