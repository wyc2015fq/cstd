
# 数据增强——HIKVISION - 小花生的博客 - CSDN博客


2018年07月15日 08:53:40[Peanut_范](https://me.csdn.net/u013841196)阅读数：435


**海康威视研究院ImageNet2016竞赛使用的数据增强方法：**
![这里写图片描述](https://img-blog.csdn.net/20180715083018802?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**1.数据增强：**
**数据增强对最后的识别性能和泛化能力都有着非常重要的作用。**我们使用下面这些数据增强方法。
第一，对颜色的数据增强，包括色彩的饱和度、亮度和对比度等方面，主要从Facebook的代码里改过来的。
第二，PCA Jittering，最早是由Alex在他2012年赢得ImageNet竞赛的那篇NIPS中提出来的. 我们首先按照RGB三个颜色通道计算了均值和标准差，对网络的输入数据进行规范化，随后我们在整个训练集上计算了协方差矩阵，进行特征分解，得到特征向量和特征值，用来做PCA Jittering。
第三，在图像进行裁剪和缩放的时候，我们采用了随机的图像差值方式。
第四， Crop Sampling，就是怎么从原始图像中进行缩放裁剪获得网络的输入。
比较常用的有2种方法：一是使用Scale Jittering，VGG和ResNet模型的训练都用了这种方法。二是尺度和长宽比增强变换，最早是Google提出来训练他们的Inception网络的。我们对其进行了改进，提出Supervised Data Augmentation方法。
![这里写图片描述](https://img-blog.csdn.net/20180715083045981?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
尺度和长宽比增强变换有个缺点，随机去选Crop Center的时候，选到的区域有时候并不包括真实目标的区域。这意味着，有时候使用了错误的标签去训练模型。如图所示，左下角的图真值标签是风车农场，但实际上裁剪的区域是蓝天白云，其中并没有任何风车和农场的信息。我们在Bolei今年CVPR文章的启发下，提出了有监督的数据增强方法。我们首先按照通常方法训练一个模型，然后用这个模型去生成真值标签的Class Activation Map（或者说Heat Map）, 这个Map指示了目标物体出现在不同位置的概率. 我们依据这个概率，在Map上随机选择一个位置，然后映射回原图，在原图那个位置附近去做Crop。
![这里写图片描述](https://img-blog.csdn.net/20180715083113752?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
如图所示，对比原始的尺度和长宽比增强变换，我们方法的优点在于，我们根据目标物体出现在不同位置的概率信息，去选择不同的Crop区域，送进模型训练。通过引入这种有监督的信息，我们可以利用正确的信息来更好地训练模型，以提升识别准确率。 (+0.5~0.7)
[
](https://img-blog.csdn.net/20180715083113752?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**2.样本平衡：**
场景数据集有800万样本，365个类别，各个类别的样本数非常不平衡，有很多类别的样本数达到了4万，也有很多类别的样本数还不到5000。这么大量的样本和非常不均匀的类别分布，给模型训练带来了难题。
Label Shuffling平衡策略
在去年冠军团队的Class-Aware Sampling方法的启发下，我们提出了Label Shuffling的类别平衡策略。在Class-Aware Sampling方法中，他们定义了2种列表，一是类别列表，一是每个类别的图像列表，对于365类的分类问题来说，就需要事先定义366个列表，很不方便。我们对此进行了改进，只需要原始的图像列表就可以完成同样的均匀采样任务。以图中的例子来说，步骤如下：
首先对原始的图像列表，按照标签顺序进行排序；
然后计算每个类别的样本数量，并得到样本最多的那个类别的样本数。
根据这个最多的样本数，对每类随机都产生一个随机排列的列表；
然后用每个类别的列表中的数对各自类别的样本数求余，得到一个索引值，从该类的图像中提取图像，生成该类的图像随机列表；
然后把所有类别的随机列表连在一起，做个Random Shuffling，得到最后的图像列表，用这个列表进行训练。
每个列表，到达最后一张图像的时候，然后再重新做一遍这些步骤，得到一个新的列表，接着训练。Label Shuffling方法的优点在于，只需要原始图像列表，所有操作都是在内存中在线完成，非常易于实现。
![这里写图片描述](https://img-blog.csdn.net/20180715083144701?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Label Smoothing策略
我们使用的另外一个方法是Label Smoothing，是今年Google的CVPR论文中提出来的方法。根据我们的混淆矩阵（Confusion Matrix）的分析，发现存在很多跨标签的相似性问题，这可能是由于标签模糊性带来的。所以，我们对混淆矩阵进行排序，得到跟每个标签最相近的4个标签，用它们来定义标签的先验分布，将传统的 one-hot标签，变成一个平滑过的soft标签。通过这种改进，我们发现可以从某种程度上降低过拟合问题。(+0.2~0.3)
![这里写图片描述](https://img-blog.csdn.net/20180715083210463?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**3.性能提升技巧**
这边还有一些其他的技巧来提升性能，我们将其总结成一个原则：**训练和测试要协调**。在训练的时候，我们通常都需要做数据增强，在测试的时候，我们通常很少去做数据增强。这其中似乎有些不协调，因为你训练和测试之间有些不一致。通过我们的实验发现，如果你在训练的最后几个epoch，移除数据增强，然后跟传统一样测试，可以提升一点性能。同样，如果训练的时候一直使用尺度和长宽比增强数据增强，在测试的时候也同样做这个变化，随机取32个crop来测试，也可以在最后的模型上提升一点性能。还有一条，就是多尺度的训练，多尺度的测试。另外，值得指出的是，使用训练过程的中间结果，加入做测试，可以一定程度上降低过拟合。
![这里写图片描述](https://img-blog.csdn.net/20180715083238641?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
对于模型结构，没什么特别的改进，我们主要使用了Inception v3和Inception ResNet v2，以及他们加深加宽的版本。还用到了Wide ResNet 。
[
](https://img-blog.csdn.net/20180715083238641?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)摘录：[https://zhuanlan.zhihu.com/p/23249000](https://zhuanlan.zhihu.com/p/23249000)
[
](https://img-blog.csdn.net/20180715083238641?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**PCA Jittering的实现：**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190222135445434.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
---
\#\#\#\#\#\#注：博众家之所长，集群英之荟萃。

[
  ](https://img-blog.csdn.net/20180715083238641?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)