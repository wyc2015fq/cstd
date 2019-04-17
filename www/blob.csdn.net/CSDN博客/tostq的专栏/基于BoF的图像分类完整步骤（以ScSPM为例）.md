# 基于BoF的图像分类完整步骤（以ScSPM为例） - tostq的专栏 - CSDN博客





2016年03月08日 17:03:52[tostq](https://me.csdn.net/tostq)阅读数：2382








基于BoF的图像分类方法近年来非常流行，这种方法将图像各个局部块的描述子向量合集来描述整个图像，这些描述子太多，一般需要经过聚类量化成为”词“（words），而一张图像将由一个个的词组成，我们将词频构成直方图成为图像的表达，然后通过SVM等分类器来进行图像分类。而限制这类方法一个主要问题就是低抽象水平的词的组合并不能完全反映整个图像（高抽象水平），比如上述的词的直接组合不能反映图像中各局部块的分布，ScSPM方法就是针对于这个问题，将空间金字塔引入BoF的局部特征块转换为词的过程（Feature
 Pooling），同时保留图像块的空间信息。本文将从ScSPM方法出发，详细介绍基于BoF的图像分类完整步骤。

**一、字典训练过程**

（1）计算图像局部块的描述子向量

这里提取描述子向量的方法是SIFT，通过将图像局部块分成4x4的小块，并求每一块的梯度直方图（共8个方向），每个块的描述向量的大小为128。然而通过常规的SIFT方法直接应用于图像时，特征点实在太多，而且分布不均，不利于之后的处理，一种折衷的方式是预先将图像分为相互重叠的局部图像块（这里推荐的参数是图像局部块采样中心点间距为6，每个图像块的大小为16x16）

（2）训练稀疏表达字典

由（1）可以得到大量的特征块的描述子向量及其类别，然后我们只需要少量的词来用于稀疏表达（sparse coding），ScSPM里的Sc就是指Sparse coding，至于为什么要用稀疏表示是因为这种方法非常符合人眼视觉神经细胞的自然特征，具有更好的稳定性（robust），特征块稀疏表示同聚类方法不同之处在于，稀疏表示向量内多数位为零，而只有少数不为零（1%左右），而聚类表示向量一般只有一位为1，其余位都为0。这个训练过程在满足下式最小的时候达到最优，这里的x指特征块的局部描述子，u指的是特征块的稀疏表示，V是字典，v是词，K指字典的长度，M指训练集中特征块的数目（这里推荐K=1024，字典V即为1024x128，u为1x1024，x为1x128）

![](https://img-blog.csdn.net/20160308165924113?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


上式涉及两个未知参数字典V及训练集各点的稀疏表示u，常用的方法是交叉验证如EM等方法，即先固定一个参数，去求另一个参数，一直相互迭代，固定字典V，求各点稀疏表示u，是比较好求的，这里只涉及到一个简单的最小化问题，可以通过梯度下降法来求解，但是为得到稀疏表达，需要利用feature-sign search的方法。

而当固定稀疏表示u，求V却是一个求解复杂非线性问题，Matlab里有好用的函数fmincon

通过反复迭代上述操作，最终我们可以得到一个较好的结果（这里推荐的迭代值是50次）

（3）由此我们可以训练出一个字典V

**二、由特征块生成词（Feature Pooling)**

获得字典后，要获得特征块稀疏表示最简单的方法是直接应用u=x/V，然后将各特征块的稀疏表示叠加（求平均或最大值）在一起就组成了整个图像的表示。

![](https://img-blog.csdn.net/20160308170109756?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)或者![](https://img-blog.csdn.net/20160308170121398?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

然而特征块直接叠加将特征块的次序信息消除，这样会带的问题就是忽视了特征块在图像中的位置，第二点不能得到较为稀疏表示。ScSPM就是为了解决这个问题。

（1）SPM图像分块

将图像按空间金字塔的方式分成多个图像块：

![](https://img-blog.csdn.net/20160308170145601?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20160308170159210?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


图像内每个分块（不同尺度大小不一样），都会生成一个稀疏表示，然后将各尺度下的稀疏表示按不同权重叠加起来，大尺度权重小，而小尺度权重大，定义为![](https://img-blog.csdn.net/20160308170218054?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

然后因为后面还要进行SVM分类，需要提前构造核，由于稀疏表示相比于聚类方法，其在训练字典时的量化误差比较少，可以应用线性核，从而达到最高的SVM分类速度，SPM核定义如下：

![](https://img-blog.csdn.net/20160308170237586?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**三、多类的SVM分类方法**

因为SVM只能进行二分类，所以需要应用多类的SVM分类方法，这个非常简单，只需要根据不同的类构造不同的SVM分类器就可以了，如下：

![](https://img-blog.csdn.net/20160308170259617?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






