# MobileNets—深度学习模型的加速 - AI之路 - CSDN博客





2017年06月08日 22:29:18[AI之路](https://me.csdn.net/u014380165)阅读数：19621








**本篇博文来介绍一个深度学习模型加速的算法：MobileNets，可以在基本不影响准确率的前提下大大减少计算时间和参数数量。**

论文：MobileNets Efficient Convolutional Neural Networks for Mobile Vision Applications 

论文链接：[https://arxiv.org/abs/1704.04861](https://arxiv.org/abs/1704.04861)

MXNet框架代码：[https://github.com/miraclewkf/mobilenet-MXNet](https://github.com/miraclewkf/mobilenet-MXNet)
## **算法概述：**

深度学习网络广泛应用在图像分类，检测中，但是网络结构复杂，参数过多，计算时间过长使其不容易在移动端应用。因此像模型压缩、模型加速应该会是未来深度学习比较活跃的一个领域。**本文提出一种将传统的卷积结构改造成两层卷积结构的网络：MobileNets，采用类似ResNext里面的group操作来实现。这种新的结构可以在基本不影响准确率的前提下大大减少计算时间（约为原来的1/9）和参数数量（约为原来的1/7）。**

**这篇博客中不特殊强调的话，卷积核默认都是三维，这三维分别对应长、宽和输入通道数，因为不同人对卷积核维度的理解不同。对于常规卷积而言，假设输入特征通道数是M，卷积核的长宽分别是DK和DK，卷积核的数量是N，那么可以说是有N个M*DK*DK卷积核，也可以说是有N组卷积核，每组有M个DK*DK的卷积核。不管是哪种理解，都不影响卷积层的本质：该层有N*M*DK*DK个参数。**

## **算法详解：**

**MobileNet模型的核心就是将原本标准的卷积操作因式分解成一个depthwise convolution和一个1*1的卷积（文中叫pointwise convolution）操作。简单讲就是将原来一个卷积层分成两个卷积层，其中前面一个卷积层的每个filter都只跟input的每个channel进行卷积，然后后面一个卷积层则负责combining，即将上一层卷积的结果进行合并。**
**如下图：M表示输入特征的通道数，N表示输出特征的通道数（也是本层的卷积核个数）。因此如果假设卷积核大小是DK*DK*M*N，输出是DF*DF*N，那么标准卷积的计算量是DK*DK*M*N*DF*DF。这个式子可以这么理解，先去掉M*N，那么就变成一个二维卷积核去卷积一个二维输入feature map；那么如果输出feature map的尺寸是DF*DF，由于输出feature map的每个点都是由卷积操作生成的，而每卷积一次就会有DK*DK个计算量，因此一个二维卷积核去卷积一个二维输入feature map就有DF*DF*DK*DK个计算量；如果有M个输入feature map和N个卷积核，那么就会有DF*DF*DK*DK*M*N计算量。（另外博主认为文中DF和DG使用比较混乱，且有些错误，所以希望能认同我这段分析）**

![这里写图片描述](https://img-blog.csdn.net/20170608223158111?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

本文的算法是用上图中的（b）+（c）代替（a），**接下来详细讲解下什么怎么卷积的。依然假设有N个卷积核，每个卷积核维度是DK*DK*M，输入feature map的通道数是M，输出feature map为DF*DF*N。那么（b）表示用M个维度为DK*DK*1的卷积核去卷积对应输入的M个feature map，然后得到M个结果，而且这M个结果相互之间不累加（传统的卷积是用N个卷积核卷积输入的所有（也就是M个）feature map，然后累加这M个结果，最终得到N个累加后的结果），注意这里是用M个卷积核而不是N个卷积核，所以（b）中没有N，只有M。因此计算量是DF*DF*DK*DK*M。（b）生成的结果应该是DF*DF*M，图中的（b）表示的是卷积核的维度。**

**（c）表示用N个维度为1*1*M的卷积核卷积（b）的结果，即输入是DF*DF*M，最终得到DF*DF*N的feature map。这个就可以当做是普通的一个卷积过程了，所以计算量是DF*DF*1*1*M*N（联系下前面讲的标准卷积是DF*DF*DK*DK*M*N，就可以看出这个（c）其实就是卷积核为1*1的标准卷积）。**

**所以最重要的来了，采用这种算法的计算量变成了DF*DF*DK*DK*M+DF*DF*M*N。具体和原来相比减少了多少计算量？可以看下面这个式子：**

![这里写图片描述](https://img-blog.csdn.net/20170608223325881?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**也就是说如果卷积核大小为3*3，那么差不多卷积操作的时间能降到原来的1/9左右！**

所以看看Fig3表达的标准卷积（左边）和因式分解后的卷积（右边）的差别。注意到卷积操作后都会跟一个Batchnorm和ReLU操作。

![这里写图片描述](https://img-blog.csdn.net/20170608223400772?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后看看这个网络结构：如果把depthwise和pointwise看做不同层的话，MobileNet一共包含28层。第一个卷积层不做分解，另外最后有个均值pooling层，全连接层和softmax层。这里dw就表示depthwise。

![这里写图片描述](https://img-blog.csdn.net/20170608223422803?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这个Table2介绍了网络中不同类型层的计算时间在总时间中的比例以及参数的数量在总的参数数量中的比例。

![这里写图片描述](https://img-blog.csdn.net/20170608223442803?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

前面讲的都是在计算时间和参数的减少，现在看看计算准确率的对比：Table4，表示本文卷积和标准卷积的对比。**可以看出Accuracy减少得不是很明显，但是却大大减少了计算时间和参数数量。**

![这里写图片描述](https://img-blog.csdn.net/20170608223511678?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

为了得到更小和更快的模型，作者介绍了两个概念：Width Multiplier 和 Resolution Multiplier。前者表示输入channel变成baseline的多少倍，如Table6；后者表示对输入图像做缩放，如Table7。

![这里写图片描述](https://img-blog.csdn.net/20170608223533521?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其他更多实验对比可以参看论文。

一个细节：实际中L2正则项的系数要比较小，因为本身参数已经减少许多了。

## **总结**

**作者主要提出一种将标准卷积层拆分成两个卷积层的MolileNet网络，可以在基本保证准确率的前提下大大减少计算时间和参数数量。个人认为应该对于全卷积网络（ResNet等）的提升效果比较明显，可以在最近的object detection算法中借鉴。另外模型加速和压缩应该会是最近几年比较活跃和值得关注的领域。**




