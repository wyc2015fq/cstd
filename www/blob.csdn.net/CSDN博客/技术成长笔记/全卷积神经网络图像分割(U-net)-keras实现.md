# 全卷积神经网络图像分割(U-net)-keras实现 - 技术成长笔记 - CSDN博客





置顶2017年04月17日 20:36:34[zhixuhao](https://me.csdn.net/u012931582)阅读数：83318








最近在研究全卷积神经网络在图像分割方面的应用，因为自己是做医学图像处理方面的工作，所以就把一个基于FCN（全卷积神经网络）的神经网络用 [keras](https://keras.io/) 实现了，并且用了一个医学图像的数据集进行了图像分割。

# 全卷积神经网络

大名鼎鼎的FCN就不多做介绍了，这里有一篇很好的博文 [http://www.cnblogs.com/gujianhan/p/6030639.html。](http://www.cnblogs.com/gujianhan/p/6030639.html%E3%80%82)

不过还是建议把论文读一下，这样才能加深理解。

# 医学图像分割框架

医学图像分割主要有两种框架，一个是基于CNN的，另一个就是基于FCN的。

## 基于CNN 的框架

这个想法也很简单，就是对图像的每一个像素点进行分类，在每一个像素点上取一个patch，当做一幅图像，输入神经网络进行训练，举个例子：

![cnnbased](https://img-blog.csdn.net/20170417203813765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这是一篇发表在NIPS上的论文[Ciresan D, Giusti A, Gambardella L M, et al. Deep neural networks segment neuronal membranes in electron microscopy images[C]//Advances in neural information processing systems. 2012: 2843-2851.](http://papers.nips.cc/paper/4741-deep-neural-networks-segment-neuronal-membranes-in-electron-microscopy-images)

这是一个二分类问题，把图像中所有label为0的点作为负样本，所有label为1的点作为正样本。

这种网络显然有两个缺点：
- 冗余太大，由于每个像素点都需要取一个patch，那么相邻的两个像素点的patch相似度是非常高的，这就导致了非常多的冗余，导致网络训练很慢。
- 感受野和定位精度不可兼得，当感受野选取比较大的时候，后面对应的pooling层的降维倍数就会增大，这样就会导致定位精度降低，但是如果感受野比较小，那么分类精度就会降低。

## 基于FCN框架

在医学图像处理领域，有一个应用很广泛的网络结构----[U-net](https://lmb.informatik.uni-freiburg.de/people/ronneber/u-net/) ,网络结构如下：

![这里写图片描述](https://img-blog.csdn.net/20170417204711784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

可以看出来，就是一个全卷积神经网络，输入和输出都是图像，没有全连接层。较浅的高分辨率层用来解决像素定位的问题，较深的层用来解决像素分类的问题。

# 问题分析

我采用的数据集是一个isbi挑战的数据集，网址为： [http://brainiac2.mit.edu/isbi_challenge/](http://brainiac2.mit.edu/isbi_challenge/)

数据集需要注册下载，我的[GitHub](https://github.com/zhixuhao/unet)上也有下载好的数据集。

这个挑战就是提取出细胞边缘，属于一个二分类问题，问题不算难，可以当做一个练手。

![这里写图片描述](https://img-blog.csdn.net/20170417205229755?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这里最大的挑战就是数据集很小，只有30张512*512的训练图像，所以进行图像增强是非常有必要的。

在这里，我参考了一篇做图像扭曲的论文，[http://faculty.cs.tamu.edu/schaefer/research/mls.pdf](http://faculty.cs.tamu.edu/schaefer/research/mls.pdf)

实现的效果如下：

这是扭曲之前：

![这里写图片描述](https://img-blog.csdn.net/20170417205751603?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这是扭曲之后：

![这里写图片描述](https://img-blog.csdn.net/20170417205820931?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这是我进行图像增强的代码： [http://download.csdn.net/detail/u012931582/9817058](http://download.csdn.net/detail/u012931582/9817058)

# keras 实现

keras是一个非常简单地深度学习框架，可以很方便的搭建自己的网络，这是我的keras实现： [https://github.com/zhixuhao/unet](https://github.com/zhixuhao/unet)

# 效果

最后在测试集上的精度可以达到95.7。

效果如下所示：

![这里写图片描述](https://img-blog.csdn.net/20170427163259396?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20170427163438849?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjkzMTU4Mg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



