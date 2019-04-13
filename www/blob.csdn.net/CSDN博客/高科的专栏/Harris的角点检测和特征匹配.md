
# Harris的角点检测和特征匹配 - 高科的专栏 - CSDN博客

2018年09月11日 14:05:55[高科](https://me.csdn.net/pbymw8iwm)阅读数：1474


一.特征检测（提取）
基于特征的图像配准方法是图像配准中最常见的方法之一。它不是直接利用图像像素值，二十通过像素值导出的符号特征（如特征点、特征线、特征区域）来实现图像配准，因此可以克服利用灰度信息进行图像配准的缺点，主要体现在以下三个方面：（1）利用特征点而不是图像灰度信息，大大减少了在匹配过程中的计算量；（2）特征点的匹配度量值相对位置变化比较敏感，可以提高匹配的精度；（3）特征点的提取过程可以减少噪声的影响，对灰度变化、图像形变以及遮挡等都有较好的适应能力。
一类重要的点特征：角点（corner points），其定义主要有以下：

局部窗口沿各方向移动，灰度均产生明显变化的点
图像局部曲率突变的点
典型的角点检测算法：Harris角点检测、CSS角点检测
Harris角点检测基本思想
从图像局部的小窗口观察图像特征，角点定义：窗口向任意方向的移动都导致图像灰度的明显变化（如下图）
![](https://img-blog.csdn.net/20151029225637325?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Harris检测：数学表达
将图像窗口平移[u,v]产生灰度变化E(u,v)
![](https://img-blog.csdn.net/20151030084301218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
由泰勒展开，得：
![](https://img-blog.csdn.net/20151030084528619?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20151030084650433?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
利用角点响应函数：
![](https://img-blog.csdn.net/20151030084852888?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
判断特征点是否为角点的依据：R只与M值有关，R为大数值正数时特征点为角点，R为大数值负数时为边缘，R为小数值时为平坦区，如下图：
![](https://img-blog.csdn.net/20151030085303911?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
寻找R位于一定阈值之上的局部最大值，去除伪角点。
Harris角点检测流程：
1.通过高斯函数的导数对原始图像进行卷积计算；图像在水平方向和垂直方向的导数Ix和Iy；
2.计算对应这些梯度外积即（Ix2 、Iy2、IxIy）三个图像如下图：
4.使用高斯函数对以上图像进行卷积滤波；
3.使用前面的公式计算角点响应函数R值；
5.对计算到的角点图像进行局部极大值抑制。

# 二、特征描述
# 在检测到特征（关键点）之后，我们必须匹配它们，也就是说，我们必须确定哪些特征来自于不同图像中的对应位置。物体识别的核心问题是将同一目标在不同时间、不同分辨率、不同光照、不同位姿情况下所成的图像相匹配。而为了进行匹配，我们首先要合理的表示图像。
SIFT（Scale invariant feature transform）特征通过计算检测到的关键点周围16x16窗口内每一个像素的梯度得到。在这里我只是简单的实现类似于SIFT特征描述子的特征描述方法，即我通过每4x4的四分之一象限，通过将加权梯度值加到直方图八个方向区间中的一个，计算出一个梯度方向直方图，因此在每一个特征点都会形成一个128维的非负值形成了一个原始版本的SIFT描述子向量如下图，并且将其归一化以减少对比度和增益的影响，最后为了使描述子对其他各种光度变化鲁棒，再将这些值以0.2截尾，然后再归一化到单位长度。
![](https://img-blog.csdn.net/20151030165707489?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

# 三、特征匹配
一旦我们从两幅或者多幅图像中提取到特征及其描述子后，下一步就是要在这些图像之间建立一些初始特征之间的匹配。
匹配策略一：对前面提取到的两幅图像的128维特征描述子向量做欧式距离度量，最简单的一个策略就是先设定一个阈值（最大距离），然后返回在这个阈值范围之内的另外一个图像中的所有匹配。
匹配策略二：做最近邻匹配，即比较最近邻距离和次近邻距离的比值，即最近邻比率（NNDR）。
匹配策略一的缺点是，如果阈值设得太高，就会产生误报，也就是说会出现不正确的匹配。如果阈值设得太低，就会产生很多“漏报”，也就是说，很多正确的匹配被丢失。
![](https://img-blog.csdn.net/20151030170604893?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
固定阈值，最近邻和最近邻比率匹配。在固定阈值（虚线圆）下，描述子DA未能与DB匹配，DD错误地与DC和DE匹配。如果我们选择最近邻，DA和DE匹配。使用最近邻比率（NNDR），小的NNDR（d1/d2）正确地将DA和DB匹配，大的NNDR（d1'/d2'）正确地拒绝DD与DC、DE的匹配。
代码如下：
[https://download.csdn.net/download/pbymw8iwm/10659247](https://download.csdn.net/download/pbymw8iwm/10659247)

