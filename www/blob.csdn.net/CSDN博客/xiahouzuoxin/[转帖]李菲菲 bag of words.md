# [转帖]李菲菲 bag of words - xiahouzuoxin - CSDN博客





2012年11月02日 09:12:52[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：3301







Bag-Of-Words中K-Means聚类的效率优化
![](http://hiphotos.baidu.com/grandyang/pic/item/38fafaf50ad162d9f21a3cbf11dfa9ec8813cd94.jpg)最初的Bag
 of words，也叫做“词袋”，在信息检索中，Bag of words model假定对于一个文本，忽略其词序和语法，句法，将其仅仅看做是一个词集合，或者说是词的一个组合，文本中每个词的出现都是独立的，不依赖于其他词 是否出现，或者说当这篇文章的作者在任意一个位置选择一个词汇都不受前面句子的影响而独立选择的。

现在Computer Vision中的Bag of words来表示图像的特征描述也是很流行的。大体思想是这样的，假设有5类图像，每一类中有10幅图像，这样首先对每一幅图像划分成patch（可以是刚性分割也可以是像SIFT基于关键点检测的），这样，每一个图像就由很多个patch表示，每一个patch用一个特征向量来表示，咱就假设用Sift表示的，一幅图像可能会有成百上千个patch，每一个patch特征向量的维数128。

接下来就要进行构建Bag of words模型了，假设Dictionary词典的Size为100，即有100个词。那么咱们可以用K-means算法对所有的patch进行聚类，k=100，我们知道，等k-means收敛时，我们也得到了每一个cluster最后的质心，那么这100个质心（维数128）就是词典里的100个词了，词典构建完毕。

词典构建完了怎么用呢？是这样的，先初始化一个100个bin的初始值为0的直方图h。每一幅图像不是有很多patch么？我们就再次计算这些patch和和每一个质心的距离，看看每一个patch离哪一个质心最近，那么直方图h中相对应的bin就加1，然后计算完这幅图像所有的patches之后，就得到了一个bin=100的直方图，然后进行归一化，用这个100维的向量来表示这幅图像。对所有图像计算完成之后，就可以进行分类聚类训练预测之类的了。

那么，这里影响效率的一个方面是构建词典时的K-means聚类，我在用的时候遇到了两个问题：1、内存溢出。这是由于一般的K-means函数的输入是待聚类的完整的矩阵，在这里就是所有patches的特征向量f合成的一个大矩阵，由于这个矩阵太大，内存不顶了。我内存为4G。2、效率低。因为需要计算每一个patch和每一个质心的欧拉距离，还有比较大小，那么要是循环下来这个效率是很低的。

为了解决这个问题，我采用一下策略，不使用整一个数据矩阵X作为输入的k-means，而是自己写循环，每次处理一幅图像的所有patches，对于效率的问题，因为matlab强大的矩阵处理能力，可以有效避免耗时费力的自己编写的循环迭代。

我自己的代码下载：

[PG_SPBOW.zip](http://ir.sdu.edu.cn/~pijili/PG_SPBOW.zip): Matlab code for computing the bag of visual words at different levels of spatial pyramid
 of images.



