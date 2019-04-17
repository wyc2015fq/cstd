# 人脸对齐（十五）--PIFA with a Single CNN - 工作笔记 - CSDN博客





2018年08月21日 14:18:21[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5320
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









### Pose-Invariant Face Alignment with a Single CNN

4.3 FPS on a Titan X GPU

本文是解决 large-pose face alignment (LPFA)的，所谓的 large face poses 如 profile views with ±90 度 yaw angles

针对大姿态的人脸对齐问题，目前主流的方法是采用 a cascade of CNN regressors 结合不同类型的回归设计和特征提取方法。

目前 the cascade of CNNs 在大姿态的人脸对齐问题上主要存在以下三个问题：

1） Lack of end-to-end training 目前存在的方法， at each cascade stage CNN网络通常是独立训练的。有的甚至是每个阶段使用多个独立的CNN网络，例如不同的特征点的定位使用不同的CNN网络，然后再综合起来。 these CNNs can not be jointly optimized and might lead to a sub-optimal solution

2） Hand-crafted feature extraction 因为后一个阶段的CNN的输入依赖于前一个阶段的 CNN输出，导致每个CNN只能是 shallow CNNs，所以 this framework can not extract deep features

3） Slow training speed 每个CNN独立训练，导致整个网络的训练很慢

为了解决上述问题，我们提出了一个 visualization layer
![这里写图片描述](https://img-blog.csdn.net/20171116162524881?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqdW5oaXQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

proposed CNN architecture
![这里写图片描述](https://img-blog.csdn.net/20171116162639135?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqdW5oaXQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

1. 3D and 2D Face Shapes

这里基本的思路是人脸实际上是一个3D 物体，图像中的人脸是一个 2D shape， 这个 2D shape 对应着一个 3D shape ，我们希望通过特征点的对应来得到 2D shape 和 3D shape 的对应关系中的参数，最终我们是通过 CNN网络来学习这些对应参数的

3D face model
![这里写图片描述](https://img-blog.csdn.net/20171116162924471?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqdW5oaXQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20171116162945933?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqdW5oaXQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

visualization block
![这里写图片描述](https://img-blog.csdn.net/20171116162711641?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqdW5oaXQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20171116163526699?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqdW5oaXQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

2. Visualization Layer

我们这里使用 Z-Buffering 中的 z coordinate of surface normals of each vertex, transformed with the pose. It is an indicator of “frontability” of a vertex, i.e., the amount that the surface normalispointingtowardsthecamera. Thisquantityisused to assign an intensity value at its projected 2D location to construct the visualization image

![这里写图片描述](https://img-blog.csdn.net/20171116163745399?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqdW5oaXQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Experimental Results

![这里写图片描述](https://img-blog.csdn.net/20171116163620694?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqdW5oaXQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20171116163629078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqdW5oaXQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

The testing speed of proposed method is 4.3 FPS on a Titan X GPU. It is much faster than the 0.6 FPS speed of [LPFA] and is similar to the 4 FPS speed of [40]



