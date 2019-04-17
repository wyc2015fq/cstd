# 对Bounding-Box regression的理解 - 站在巨人的肩膀上coding - CSDN博客





2018年04月22日 19:38:34[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：683








本博客主要介绍RCNN中的Bounding-box的回归问题，这个是RCNN定准确定位的关键。本文是转载自博客：[Faster-RCNN详解](http://blog.csdn.net/ture_dream/article/details/52896452)，从中截取有关RCNN的bounding-box的回归部分。原博文详细介绍了RCNN，Fast-RCNN以及Faster-RCNN，感兴趣的可以去看一下。下面是内容：






# 1.      为什么要做Bounding-box regression？



![](https://img-blog.csdn.net/20161020131820060?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

图10  示例

如图10所示，绿色的框为飞机的Ground Truth，红色的框是提取的Region Proposal。那么即便红色的框被分类器识别为飞机，但是由于红色的框定位不准(IoU<0.5)，那么这张图相当于没有正确的检测出飞机。如果我们能对红色的框进行微调，使得经过微调后的窗口跟Ground Truth更接近，这样岂不是定位会更准确。确实，Bounding-box regression 就是用来微调这个窗口的。

# 2.      回归/微调的对象是什么？

![](https://img-blog.csdn.net/20161020131905237?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


# 3.      Bounding-box regression（边框回归）

那么经过何种变换才能从图11中的窗口P变为窗口呢？比较简单的思路就是：

![](https://img-blog.csdn.net/20161020131949020?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



![](https://img-blog.csdn.net/20161020132023864?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


注意：只有当Proposal和Ground Truth比较接近时（线性问题），我们才能将其作为训练样本训练我们的线性回归模型，否则会导致训练的回归模型不work（当Proposal跟GT离得较远，就是复杂的非线性问题了，此时用线性回归建模显然不合理）。这个也是G-CNN: an Iterative Grid Based Object Detector多次迭代实现目标准确定位的关键。

线性回归就是给定输入的特征向量X，学习一组参数W，使得经过线性回归后的值跟真实值Y(Ground Truth)非常接近。即![](https://img-blog.csdn.net/20161020132136413?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)。那么Bounding-box中我们的输入以及输出分别是什么呢？

输入：![](https://img-blog.csdn.net/20161020132157795?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)这个是什么？输入就是这四个数值吗？其实真正的输入是这个窗口对应的CNN特征，也就是R-CNN中的Pool5feature（特征向量）。(注：训练阶段输入还包括 Ground Truth，也就是下边提到的![](https://img-blog.csdn.net/20161020132230477?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center))

输出：需要进行的平移变换和尺度缩放![](https://img-blog.csdn.net/20161020132257337?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，或者说是![](https://img-blog.csdn.net/20161020132415307?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)。我们的最终输出不应该是Ground Truth吗？是的，但是有了这四个变换我们就可以直接得到Ground Truth，这里还有个问题，根据上面4个公式我们可以知道，P经过![](https://img-blog.csdn.net/20161020132257337?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，得到的并不是真实值G，而是预测值![](https://img-blog.csdn.net/20161020132449964?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)。

的确，这四个值应该是经过 Ground Truth 和Proposal计算得到的真正需要的平移量![](https://img-blog.csdn.net/20161020132536371?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)和尺度缩放![](https://img-blog.csdn.net/20161020132547762?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)。

这也就是R-CNN中的：

![](https://img-blog.csdn.net/20161020132558872?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





那么目标函数可以表示为![](https://img-blog.csdn.net/20161020132629825?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)是输入Proposal的特征向量，![](https://img-blog.csdn.net/20161020132746905?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)是要学习的参数（*表示![](https://img-blog.csdn.net/20161020132757766?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，也就是每一个变换对应一个目标函数），![](https://img-blog.csdn.net/20161020132808827?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)是得到的预测值。我们要让预测值跟真实值![](https://img-blog.csdn.net/20161020132906313?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)差距最小，得到损失函数为：

![](https://img-blog.csdn.net/20161020132920063?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

函数优化目标为：

![](https://img-blog.csdn.net/20161020132932689?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

利用梯度下降法或者最小二乘法就可以得到![](https://img-blog.csdn.net/20161020132746905?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)。



# 4.      测试阶段
   根据3我们学习到回归参数，对于测试图像，我们首先经过 CNN 提取特征![](https://img-blog.csdn.net/20161020133042236?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，预测的变化就是![](https://img-blog.csdn.net/20161020133059643?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，最后根据以下4个公式对窗口进行回归：


![](https://img-blog.csdn.net/20161020133115800?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







