# 系统学习NLP（七）--词语相似度 - 工作笔记 - CSDN博客





2018年10月16日 17:57:50[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5541








目的是为了计算词语之间的各种语义关系。分为以下几类：

**基于语义字典的方法：**

      这类方法，主要使用WordNet,MeSH这样的在线语义字典来度量两个义项之间的距离。基于WordNet中的概念是由概念间关系连接在一起的，每个概念都通过关系和其他概念相连，而整个WordNet则是由概念和关系组成的巨大的网络。最上面图是WordNet中部分概念网络结构图。

       下图是以"car"和“bag”为主体的概念，得到is_a关系树状分类图

![](https://upload-images.jianshu.io/upload_images/6861691-425c2be867d9ec63.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/666)



![](https://upload-images.jianshu.io/upload_images/6861691-2296857bc2fdc4bf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/703)

![](https://upload-images.jianshu.io/upload_images/6861691-82f5694e9833ca4b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/706)

![](https://upload-images.jianshu.io/upload_images/6861691-b69a47fa05566023.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/778)

![](https://upload-images.jianshu.io/upload_images/6861691-8bd28b6952e7f15c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/781)

还有基于路径长度的，这是最简单的，Sim(c1,c2) = -log len(c1,c2),上面3-2是对这个路径相似度的改进，即方式假设边是等距离的，但明显不适合很多场景，因此3-2做了长度归一化。这种改进效果不大。实际上，对这种评估方式的最好改进是LCS。

也就是Resnik相似度：-log P(LCS(c1,c2)),就是对词语在树图中的概念概率化，利用信息定义概念c的信息量。这里作者只考虑相同信息部分

后来Lin改进了上面的方式，同时考虑排除不同信息：2 * IC(lcs) / (IC(s1) + IC(s2))

类似的，Jiang-Conrath距离：1/(IC(s1) + IC(s2) - 2 * IC(lcs))

**基于字典的相似度:**

就是扩展的Lesk算法，主要思想是基于字典中两个概念或者义项的注释包含相同的词语，则他们就相似。实现的时候，定义重叠函数，计算两个概念A,B的上下位，部件及其他关系的的概念的注释之间的交集。

**基于特征向量**

这个就很多了，有手动定义的特征向量，有自动提取的特征向量。

度量有：

曼哈顿距离，欧氏距离，余弦距离，Jaccard距离（最小/最大距离），Dice距离，交叉熵距离（Jenson-Shannon 散度距离）

实际上，我想大多数应用现在都是基于向量算的，这里记录这些，只是做个基础了解。**先占个坑，后面再更新这个方向所有主流的方式。**




