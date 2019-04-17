# ThiNet算法详解 - AI之路 - CSDN博客





2017年09月01日 08:04:48[AI之路](https://me.csdn.net/u014380165)阅读数：3569








论文：ThiNet： A Filter Level Pruning Method for Deep Neural Network Compression 

论文链接：[https://arxiv.org/abs/1707.06342](https://arxiv.org/abs/1707.06342)

**prune（剪枝）是深度学习模型压缩和加速的重要方法**。ThiNet是**ICCV2017**的文章，主要通过prune方式达到模型压缩和加速，prune以filter（卷积核）为单位，根据该层filter的输出来判断该filter是否对结果有贡献，如果没有贡献或贡献很小，则直接把这个filter去掉，因此文章的核心就在于filter的选择方式，**依据则是如果可以用某一层的输入的一个子集代替原来的输入得到尽可能类似原来的输出的话，那么子集以外的输入就可以去掉，同时其对应的前面一层的filter也就可以去掉**。以去掉冗余filter做prune的研究还有很多，关键在于选择方式，比如计算filter的绝对值和，认为如果一个filter的绝对值和比较小，说明该filter并不重要，这种算法暂且叫**Weight sum**；还有计算激活层输出的feature map的值的稀疏程度，如果feature map的值很稀疏，也就是大部分值是0，那么该feature map对应的filter也是冗余的，可以去掉，这种算法暂且叫**APoZ（Average Percentage of Zeros）**。这两种压缩算法在后面的实验中都会提到。

**具体ThiNet的压缩和加速效果怎么样**？VGG-16可以达到减少3.31倍FLOPs和16.63倍的压缩，top-5准确率只下降0.52%。ResNet50可以达到减少2.26倍FLOPs和2.06倍的压缩，top-5准确率只下降了1%。

框架的流程图如下Figure1，第一行是filter selection，选择的依据是：**如果我们可以用第i+1层的输入channel的一个子集作为第i+1层的输入且近似得到第i+1层的输出，那么这个子集以外的channel就可以去掉了，因为第i+1层的一个输入channel对应第i层的一个filter（卷积核），因此去掉第i+1层的channel同时也就可以去掉第i层的filter。**第二行是prune，将第一步的weak channel和对应的前面一层的filter去掉，得到更窄（thin）的网络，这也是ThiNet名称的由来。第三行是Fine-tuning，这里为了节约时间，当对每一层做prune后，都fine-tune1到2个epoch，然后等所有层都prune后，再fine-tune多个epoch。因此整体上就是上面这三步迭代应用到每一层上，依次对每一层做prune。

![这里写图片描述](https://img-blog.csdn.net/20170901075840186?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**从前面的介绍可以看出ThiNet的关键在于怎么选择冗余的filter**，接下来详细介绍。首先一个卷积操作可以用下面这个式子表示。W是卷积核，x是输入，b是偏置，c表示输入channel。

![这里写图片描述](https://img-blog.csdn.net/20170901075907307?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如果我们定义：

![这里写图片描述](https://img-blog.csdn.net/20170901075936527?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

那么式子1就可以写成下面这个形式：

![这里写图片描述](https://img-blog.csdn.net/20170901075946054?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如果定义一个集合S：

![这里写图片描述](https://img-blog.csdn.net/20170901080010827?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

那么式子3就可以写成：

![这里写图片描述](https://img-blog.csdn.net/20170901080021110?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**作者在文中采用的是贪心算法来求解最优的集合S**。假设输入是：

![这里写图片描述](https://img-blog.csdn.net/20170901080049522?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

那么就能得到下面这个优化式子：

![这里写图片描述](https://img-blog.csdn.net/20170901080058882?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这里r表示压缩率。也就是说我们需要找到能使得这个优化式子最小的channel集合S。作者采用贪心算法来求解这个S，由于S包含channel较多，因此求解速度会很慢，因此定义另一个集合T，集合T所包含的channel要少于S，满足：

![这里写图片描述](https://img-blog.csdn.net/20170901080123292?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这样就能将式子5转换成如下的优化式子，这样求解最优值的速度会更快。

![这里写图片描述](https://img-blog.csdn.net/20170901080135576?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

综上，求最优集合T的算法如下：

![这里写图片描述](https://img-blog.csdn.net/20170901080152532?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

前面介绍的都是关于算法的细节，**那么如何应用到现有的网络结构中呢**？这里作者主要介绍VGG-16和ResNet-50。在两个网络中应用ThiNet做prune有一些需要注意的地方，比如像**ResNet**这样的网络，在每一个stage的每一个block中一般有三层卷积，其中最后一层卷积的结果需要和skip connection的结果做element-wise product，这样的话就得保证该block的最后一层卷积的输出channel个数核skip connection的输出channel个数一样。**因此在文中作者采用只对一个block的前两层卷积做prune，而不动最后一个卷积层，如下图Figure3**。另外对于**VGG-16**网络，由于前面10层卷积占据了90%的计算量，而全连接层又占据了86%的参数，**因此作者采用对前面10层卷积层进行prune，达到加速目的，另外将所有全连接层用一个global average pooling层代替。**

![这里写图片描述](https://img-blog.csdn.net/20170901080245365?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**实验结果：**

本文的ThiNet在进行网络压缩的时候，关键在于怎么选择冗余的filter，**因此Figure4就是不同filter选择方式的实验结果对比**。Random就是随机选择，Weight sum和APoZ在前面都介绍过了。

![这里写图片描述](https://img-blog.csdn.net/20170901080315058?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**Table1是在VGG-16网络上应用ThiNet压缩**。其中ThiNet-Conv表示只对前面10个卷积层进行压缩，ThiNet-GAP表示在对卷积层压缩的基础上还将所有的全连接层用global average pooling代替。ThiNet-Tiny是压缩率较高的网络，主要是和SqueezeNet做对比。

![这里写图片描述](https://img-blog.csdn.net/20170901080340009?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**Table2也是VGG-16，这是和其他基于filter做prune的算法的对比**。APoZ-1和APoZ-2的差别在于后者prune的卷积层数量更少。Taylor算法对filter做prune的依据在于如果一个filter的存在对减少loss没有帮助，则可以去掉。

![这里写图片描述](https://img-blog.csdn.net/20170901080407871?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**Table3是在ResNet-50上应用ThiNet进行压缩和加速的结果**。ResNet-50在网络结构设计上本身存在的冗余就比较少，所以可压缩的空间比VGG-16要少很多。另外由于在对ResNet-50压缩时候只是压缩大部分卷积层，而没有压缩像BN，pooling层等，但是这些层在测试的时候会占用约40%的GPU计算资源，因此测试时候的加速不会太明显。

![这里写图片描述](https://img-blog.csdn.net/20170901080431244?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



