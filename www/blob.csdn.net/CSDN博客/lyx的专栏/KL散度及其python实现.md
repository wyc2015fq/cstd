# KL散度及其python实现 - lyx的专栏 - CSDN博客





2016年07月05日 21:31:57[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：9696








KL散度又是一个从信息论、熵的角度考量距离的一个量。但是，这里说他是距离有点不妥，因为距离需要满足4个条件：

1) d(x,x) = 0                    反身性

2) d(x,y) >= 0                  非负性

3) d(x,y) = d(y,x)              对称性

4) d(x,k)+ d(k,y) >= d(x,y)    三角形法则

但是，很遗憾，我们的KL散度至满足前面两条，后面介绍的对称KL也只能满足前面三条。所以，我们叫KL散度，而不是叫KL距离。

## 1.KL定义与计算




![](https://img-blog.csdn.net/20160705211756790?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


计算公式就是如此简单。我们做个例题吧。

假设我们有这样的两个分布，A和B，他们出现0和1的概率如下。


![](https://img-blog.csdn.net/20160705211905072?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这个时候，我们计算A和B之间的KL散度，就是这样：

![](https://img-blog.csdn.net/20160705211927932?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


so easy的样子有木有！

如何直观的理解这样的一个度量的量呢。我不说什么用A的概率去编码B之类的，直观的去看KL散度的公式，说白了，P（x）部分可以认为是权重，其值就是P取该值的概率，后面的则是两者出现该变量的概率之比，然后取对数。取对数当然就是因为信息熵啦。也就是说，如果某一个变量出现的概率在P中很小，那么权重很小，即使在Q中很大，使得后半部分值比较大，那么最后值也不会很大；反过来也一样。所以，希望KL散度大，那么就需要有大的权重和大的概率差异，也就是，两个分布要不一样。

对称KL就是KL（P，Q）与KL（Q，P）的值加起来之后取平均。

## 2.KL散度计算Python代码



```python
import numpy as np
from scipy import *
def asymmetricKL(P,Q):
    return sum(P * log(P / Q)) #calculate the kl divergence between P and Q

def symmetricalKL(P,Q):
    return (asymmetricKL(P,Q)+asymmetricKL(Q,P))/2.00
```
上面就是KL散度的计算代码了。










