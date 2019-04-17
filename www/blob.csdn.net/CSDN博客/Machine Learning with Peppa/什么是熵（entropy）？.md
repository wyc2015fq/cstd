# 什么是熵（entropy）？ - Machine Learning with Peppa - CSDN博客





2018年06月03日 19:56:14[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：8547








熵的概念最早起源于物理学，用于度量一个热力学系统的无序程度。在信息论里面，熵是对不确定性的测量。

### 1.1 熵的引入

    事实上，熵的英文原文为entropy，最初由德国物理学家鲁道夫·克劳修斯提出，其表达式为：![](https://img-blog.csdn.net/20180603195424138?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

    它表示一个系系统在不受外部干扰时，其内部最稳定的状态。后来一中国学者翻译entropy时，考虑到entropy是能量Q跟温度T的商，且跟火有关，便把entropy形象的翻译成“熵”。

    我们知道，任何粒子的常态都是随机运动，也就是"无序运动"，如果让粒子呈现"有序化"，必须耗费能量。所以，温度（热能）可以被看作"有序化"的一种度量，而"熵"可以看作是"无序化"的度量。

    如果没有外部能量输入，封闭系统趋向越来越混乱（熵越来越大）。比如，如果房间无人打扫，不可能越来越干净（有序化），只可能越来越乱（无序化）。而要让一个系统变得更有序，必须有外部能量的输入。

    1948年，香农Claude E. Shannon引入信息（熵），将其定义为离散随机事件的出现概率。一个系统越是有序，信息熵就越低；反之，一个系统越是混乱，信息熵就越高。所以说，信息熵可以被认为是系统有序化程度的一个度量。
    若无特别指出，下文中所有提到的熵均为信息熵。




### 1.2 熵的定义

    下面分别给出熵、联合熵、条件熵、相对熵、互信息的定义。

**熵**：如果一个随机变量X的可能取值为X = {x1, x2,…, xk}，其概率分布为P(X = xi) = pi（i = 1,2, ..., n），则随机变量X的熵定义为：

> 
> 
> 
![](http://image79.360doc.com/DownloadImg/2014/11/1122/47016837_2)![](https://img-blog.csdn.net/20180603195429380?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



    把最前面的负号放到最后，便成了：

> 
> 
> 
![](http://image79.360doc.com/DownloadImg/2014/11/1122/47016837_3)![](https://img-blog.csdn.net/20180603195433550?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




    上面两个熵的公式，无论用哪个都行，而且两者等价，一个意思（这两个公式在下文中都会用到）。

**联合熵**：两个随机变量X，Y的联合分布，可以形成联合熵Joint Entropy，用H(X,Y)表示。
**条件熵**：在随机变量X发生的前提下，随机变量Y发生所新带来的熵定义为Y的条件熵，用H(Y|X)表示，用来衡量在已知随机变量X的条件下随机变量Y的不确定性。

    且有此式子成立：H(Y|X) = H(X,Y) – H(X)，整个式子表示(X,Y)发生所包含的熵减去X单独发生包含的熵。至于怎么得来的请看推导：

> 
![](http://image79.360doc.com/DownloadImg/2014/11/1122/47016837_4)![](https://img-blog.csdn.net/20180603195437929?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


   简单解释下上面的推导过程。整个式子共6行，其中


- 第二行推到第三行的依据是边缘分布p(x)等于联合分布p(x,y)的和；
- 第三行推到第四行的依据是把公因子logp(x)乘进去，然后把x,y写在一起；
- 第四行推到第五行的依据是：因为两个sigma都有p(x,y)，故提取公因子p(x,y)放到外边，然后把里边的-（log p(x,y) - log p(x)）写成- log (p(x,y)/p(x) ) ；
- 第五行推到第六行的依据是：条件概率的定义p(x,y) = p(x) * p(y|x)，故p(x,y) / p(x) =  p(y|x)。

**相对熵：**又称互熵，交叉熵，鉴别信息，Kullback熵，Kullback-Leible散度等。设p(x)、q(x)是X中取值的两个概率分布，则p对q的相对熵是：


> 
![](https://img-blog.csdn.net/20180603195445738?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


> 
> 
> 
![](http://image79.360doc.com/DownloadImg/2014/11/1122/47016837_5)




> 



    在一定程度上，相对熵可以度量两个随机变量的“距离”，且有D(p||q) ≠D(q||p)。另外，值得一提的是，D(p||q)是必然大于等于0的。


**互信息**：两个随机变量X，Y的互信息定义为X，Y的联合分布和各自独立分布乘积的相对熵，用I(X,Y)表示：

> 
![](https://img-blog.csdn.net/2018060319545265?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


    且有I(X,Y)=D(P(X,Y) || P(X)P(Y))。下面，咱们来计算下H(Y)-I(X,Y)的结果，如下：

> 
> 
> 
![](http://image79.360doc.com/DownloadImg/2014/11/1122/47016837_7)![](https://img-blog.csdn.net/20180603195501880?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



    通过上面的计算过程，我们发现竟然有H(Y)-I(X,Y) = H(Y|X)。故通过条件熵的定义，有：H(Y|X) = H(X,Y) - H(X)，而根据互信息定义展开得到H(Y|X) = H(Y) - I(X,Y)，把前者跟后者结合起来，便有I(X,Y)= H(X) + H(Y) - H(X,Y)，此结论被多数文献作为互信息的定义。




