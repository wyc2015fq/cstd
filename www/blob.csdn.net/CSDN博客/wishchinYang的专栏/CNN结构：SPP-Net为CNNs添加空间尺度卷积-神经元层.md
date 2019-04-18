# CNN结构：SPP-Net为CNNs添加空间尺度卷积-神经元层 - wishchinYang的专栏 - CSDN博客
2015年06月18日 18:27:35[wishchin](https://me.csdn.net/wishchin)阅读数：1461
        前几个CNN检测的框架要求网络的图像输入为固定长宽，而SPP-Net在CNN结构中添加了一个实现图像金字塔功能的卷积层SPP层，用于在网络中实现多尺度卷积，由此对应多尺度输入，以此应对图像的缩放变换和仿射变换。
        原文链接：[Spatial Pyramid Pooling in Deep Convolutional Networks for Visual Recognition](http://blog.csdn.net/woyaopojie1990/article/details/38582677)
[原文链接： http://zhangliliang.com/2014/09/13/paper-note-sppnet/](http://zhangliliang.com/2014/09/13/paper-note-sppnet/)
        作者主页：[http://research.microsoft.com/en-us/um/people/kahe/eccv14sppnet/index.html](http://research.microsoft.com/en-us/um/people/kahe/eccv14sppnet/index.html)
**一、文章的主要思想**
考虑到传统的CNN构架的输入图像的尺寸都是固定的（例如：256*256），这种人工改变输入图像的尺寸破坏了输入图像的尺度和长宽比例。作者认为卷积层的输入的尺寸可以是任意，全连接层的输入是固定不变。针对这个问题，作者提出了spatial pyramid pooling（SPP-net）结构，在目标检测方面，比R-CNN快30-170倍。
![](https://img-blog.csdn.net/20140815162921177?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd295YW9wb2ppZTE5OTA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**二、spatial pyramid pooling（SPP-net）的优势**
        1、针对不同尺寸的输入可以得到相同维度的输出，而siding window pooling 做不到；
        2、SPP使用multi-level spatial bins, 而siding window pooling采用的单一的窗口，multi-level对目标变形非常鲁棒；
        3、由于输入尺寸的可变性，SPP可以提取不同尺度的特征。
           这样就可以省去对输入图像进行标准化的麻烦,可以节省大量人力。
**三、****Deep Networks with Spatial Pyramid Pooling**
特征提取的过程其实很简单，就是将SPP放在卷积层的最后一层，pooling层以前，将相当于用SPP代替最后一层pooling。假设最后一层卷积层有256个maps，每个maps的尺寸为a*a，n*n个bins。那么采用窗口win=ceil（a/n）和步长str=floor（a/n）的max-pooling。最后将所有的特征级联起来作为全连接层的输入。这样就保证了无论输入图像的尺寸，输入全连接层的输入都有同样的大小。示意图如下：
![](https://img-blog.csdn.net/20140815171018243?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd295YW9wb2ppZTE5OTA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20140815170833240?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd295YW9wb2ppZTE5OTA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20140815170854814?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd295YW9wb2ppZTE5OTA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**四、实验结果**
![](https://img-blog.csdn.net/20140815171721706?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd295YW9wb2ppZTE5OTA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
    从这个结果来看，确实有不少的提升。在ILSVRC比赛获得了第二名？
**五、论文总结**
本论文的思想主要还是基于SPM的思想，将CNN和SPM进行了结合，值得借鉴。并且提到了再训练模型的时候，采用不同的尺寸输入交替训练，这种思想还是首次提到。
     增加一层神经元消畸变和尺度变换.
