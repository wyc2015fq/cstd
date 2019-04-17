# NLP之条件随机场 - miner_zhu的博客 - CSDN博客





置顶2018年09月19日 22:14:22[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：126标签：[nlp																[条件随机场](https://so.csdn.net/so/search/s.do?q=条件随机场&t=blog)](https://so.csdn.net/so/search/s.do?q=nlp&t=blog)
个人分类：[NLP](https://blog.csdn.net/miner_zhu/article/category/7896187)








条件随机场（conditional random fields, CRFs）由J. Lafferty等人（2001）提出，近几年来在自然语言处理和图像处理等领域中得到了广泛的应用。

CRF是用来标注和划分序列结构数据的概率化结构模型。言下之意，就是对于给定的输出标识序列Y和观测序列X，条件随机场通过定义条件概率P（Y|X），而不是联合概率分布P（X，Y）来描述模型。 CRF也可以看作一个无向图模型或者马尔可夫随机场（Markov random field）。

定义6-5（条件随机场） 设G＝（V，E）为一个无向图，V为结点集合，E为无向边的集合。Y＝{Yv|v∈V}，即V中的每个结点对应于一个随机变量Yv，其取值范围为可能的标记集合{y}。如果以观察序列X为条件，每一个随机变量Yv都满足以下马尔可夫特性：

![](https://img-blog.csdn.net/20180919220607561?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21pbmVyX3podQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中，w～v表示两个结点在图G中是邻近结点。那么，（X，Y）为一个条件随机场。

理论上，只要在标记序列中描述了一定的条件独立性，G的图结构可以是任意的。对序列进行建模可以形成最简单、最普通的链式结构（chain-structured）图，结点对应标记序列Y中的元素（图6-13）。或者更直观一点，把CRF的链式结构图画为如图6-14所示。

![](https://img-blog.csdn.net/20180919220928662?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21pbmVyX3podQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180919220934843?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21pbmVyX3podQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

显然，观察序列X的元素之间并不存在图结构，因为这里只是将观察序列X作为条件，并不对其作任何独立性假设。

在给定观察序列X时，某个特定标记序列Y的概率可以定义为

![](https://img-blog.csdn.net/20180919220955432?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21pbmVyX3podQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中，tj（yi-1，yi，X，i）是转移函数，表示对于观察序列X其标注序列在i及i－1位置上标记的转移概率；sk（yi，X，i）是状态函数，表示对于观察序列X其i位置的标记概率；λj和μk分别是tj和sk的权重，需要从训练样本中估计出来。

参照最大熵模型的做法，在定义特征函数时可以定义一组关于观察序列的{0，1}二值特征b（X, i）来表示训练样本中某些分布特性，例如，

![](https://img-blog.csdn.net/20180919221059391?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21pbmVyX3podQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

转移函数可以定义为如下形式：

![](https://img-blog.csdn.net/20180919221108537?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21pbmVyX3podQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

为了便于描述，可以将状态函数书写成如下形式：

s（yi，X，i）＝s（yi－1，yi，X，i）

这样，特征函数可以统一表示为：

![](https://img-blog.csdn.net/20180919221128599?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21pbmVyX3podQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中，每个局部特征函数fj（yi－1，yi，X，i）表示状态特征s（yi－1， yi，X，i）或转移函数t（yi－1，yi，X，i）。

由此，条件随机场定义的条件概率可以由下式给出：

![](https://img-blog.csdn.net/20180919221147295?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21pbmVyX3podQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中，分母Z（X）为归一化因子：

![](https://img-blog.csdn.net/20180919221156874?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L21pbmVyX3podQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

条件随机场模型也需要解决三个基本问题：特征的选取、参数训练和解码。其中，参数训练过程可在训练数据集上基于对数似然函数的最大化进行。

相对于HMM，CRF的主要优点在于它的条件随机性，只需要考虑当前已经出现的观测状态的特性，没有独立性的严格要求，对于整个序列内部的信息和外部观测信息均可有效利用，避免了MEMM和其他针对线性序列模型的条件马尔可夫模型会出现的标识偏置问题。CRF具有MEMM的一切优点，两者的关键区别在于，MEMM使用每一个状态的指数模型来计算给定前一个状态下当前状态的条件概率，而CRF用单个指数模型来计算给定观察序列与整个标记序列的联合概率。因此，不同状态的不同特征权重可以相互交替代换。




