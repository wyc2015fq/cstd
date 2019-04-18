# HORQ（High-Order Residual Quantization）算法详解 - AI之路 - CSDN博客





2017年09月03日 10:30:54[AI之路](https://me.csdn.net/u014380165)阅读数：1802








论文：Performance Guaranteed Network Acceleration via High-Order Residual Quantization 

论文链接：[https://arxiv.org/abs/1708.08687](https://arxiv.org/abs/1708.08687)

暂无代码
**这是ICCV2017的文章，作者主要来自上海交大。我们知道模型压缩方面可以做到不大影响准确率，但是如果同时要做到模型加速还不影响准确率的就非常少了，而HORQ就可以做到！HORQ在对准确率影响很小的情况下能将模型缩小32倍，同时加速30倍左右！**

我们知道模型的二值化是深度学习模型压缩和加速的一类重要方法，**二值化操作一般包括对权重做二值化，也包括对输入做二值化，前者主要可以压缩模型，后者主要可以加速模型，可以参考**[XNOR-Net算法详解](http://blog.csdn.net/u014380165/article/details/77731595)。但是一般对输入做二值化后模型准确率会下降特别厉害，而这篇文章提出的对权重和输入做high-order residual quantization的方法可以在保证准确率的情况下大大压缩和加速模型。

接下来详细介绍HORQ，因为HORQ可以看做是XNOR的改进版，所以建议先看看XNOR：[XNOR-Net算法详解](http://blog.csdn.net/u014380165/article/details/77731595)。HORQ和XNOR都包含对weight和input做二值化，weight二值化方面基本一样，接下来主要介绍对input的二值化。

假设有个输入X，我们采用XNOR论文里面的**对输入做二值化**的式子就是公式6，H是个二值向量，通过解式子7这个优化函数就可以得到式子8的β和H。

![这里写图片描述](https://img-blog.csdn.net/20170903102414801?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20170903102433191?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

上面这些是XNOR里面对input做二值化的内容，在HORQ中也用到，接下来进入HORQ阶段。首先式子6的β1H1可以看成是一个**first-order binary quantization**，那么可以定义输入X和 first-order binary quantization之间的差为R1(X)，R1(X)也称作**first-order residual tensor**，公式如下。可以看出R1(X)是用来表征二值化后输入信息的损失。

![这里写图片描述](https://img-blog.csdn.net/20170903102511011?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

同样我们可以对R1(X)做二值化：

![这里写图片描述](https://img-blog.csdn.net/20170903102539434?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这样就可以得到输入的**order-two residual quantization**表示：

![这里写图片描述](https://img-blog.csdn.net/20170903102550538?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**这里β1H1就是first-order binary input，β2H2就是second-order binary input**。同样也可以得到式子8类似的优化结果：

![这里写图片描述](https://img-blog.csdn.net/20170903102619957?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

因此，如果将公式11和公式13进一步扩展，就可以得到**order-K residual quantization**，如公式16，以及最优解的一般形式如公式17。

![这里写图片描述](https://img-blog.csdn.net/20170903102641277?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

显然，**如果你的order越高，输入的信息损失会越少，但是计算量会越大。因此在文中作者主要采用order-Two来做实验，效果就比较好了。另外我们之前比较熟悉的XNOR算法其实在这里就是order-one，后面的实验也基本上是二者之间的对比。**

Figure1是HORQ的示意图。

![这里写图片描述](https://img-blog.csdn.net/20170903102702163?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

作者在介绍具体算法的时候采用Figure2的对原来的输入和权值做reshape的操作，得到权值矩阵Wr和输入矩阵Xr，分别对应Figure2第二行的第一个和第二个矩阵，而第二行的第三个矩阵就是输出Yr。来看第2行是怎么做卷积的，**第二行的第一个矩阵的一行和第二个矩阵的一列相乘得到的结果表示一个大小为w*h的卷积核对输入的cin个图像的每个图像的同一个w*h大小区域的卷积结果，并把所以图像的这个结果加起来，因此得到的是一个数**。第二行的第一个矩阵的一行和第二个矩阵的所有列相乘得到的结果表示一个大小为w*h的卷积核对输入的ci个图像的卷积结果，是一个feature map。

![这里写图片描述](https://img-blog.csdn.net/20170903102734596?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**Algorithm2就是一个order-Two binary convolution的流程**。先得到第2,3步的Wr和Xr，分别对应Figure2中的前面两个箭头，生成的都是矩阵，Wr和Xr跟Figure2中的第2行的前两个矩阵对应。然后第4到8行是对weight做二值化，这部分和XNOR论文中的一样。重点在于第9到第16行的对输入均值的二值化操作。第10行中的wout*hout就是Figure2中第二行的中间那个矩阵的列数。第11行的B就对应公式6中的scale factor β，第12行的N就对应公式6中的二值矩阵H。B1表示first-order，B2表示second-order。第17行得到的Yr和Figure2中的第二行的最后一个矩阵对应。

![这里写图片描述](https://img-blog.csdn.net/20170903102758979?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**Algorithm3是训练一个L层HORQ网络的流程**，比较容易理解，因为主要是调用了Algorithm2的过程。**前向和后向中的每一层都进行了weight和input的二值化操作，但是在参数更新的时候还是采用的原来的数值，这和XNOR是一样的。**

![这里写图片描述](https://img-blog.csdn.net/20170903102827169?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**实验结果：**

作者主要在MNIST和CIFAR-10两个数据集上做实验。 
**Figure3是在MNIST数据集上的HORQ和XNOR算法的对比**。可以看出HORQ不仅有更高的准确率，而且收敛速度也更快。
![这里写图片描述](https://img-blog.csdn.net/20170903102853272?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**Figure5是在CIFAR-10数据集上HORQ和XNOR的对比**。蓝色那条线是不经过二值化的网络。可以看出HORQ对准确率的影响已经很小了。

![这里写图片描述](https://img-blog.csdn.net/20170903102915944?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

除了速度上的优势外，还有存储空间的减少如Figure7所示，而这主要得益于对weight做二值化。

![这里写图片描述](https://img-blog.csdn.net/20170903102932571?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**Figure8给出了HORQ加速和filter size，number of channel以及order of quantization的关系**。注意HORQ的加速和input tensor的宽和高是没有关系的，文中有具体公式，这里就不介绍了。

![这里写图片描述](https://img-blog.csdn.net/20170903102957979?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Table2是关于不同order的加速情况。

![这里写图片描述](https://img-blog.csdn.net/20170903103021094?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**总的来讲这篇算法应该是模型二值化方面目前效果最好的，毕竟模型压缩好做，但是模型加速和准确率难以兼得。不知道后期会不会公布代码。**





