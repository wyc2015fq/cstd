# 三维计算机视觉（六）--3DSC（3D形状上下文特征） - 工作笔记 - CSDN博客





2017年10月07日 10:58:24[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6454
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









首先，3DSC特征是在2DSC的基础上扩展的，因此，先介绍2DSC。

### 2D  Shape Context

参考：


http://blog.csdn.net/aivin24/article/details/8846293?reload

http://blog.csdn.net/u012507022/article/details/52437149

《shapematch-Belongie-Pami-ByContext》


形状上下文特征是一种很流行的形状描述子，多用于形状匹配，目标识别，它采用一种基于形状轮廓的特征描述方法,其在对数极坐标系下利用直方图描述形状特征能够很好地反映轮廓上采样点的分布情况。

形状上下文的基本原理如下：



Step1:对于给定的一个形状,通过边缘检测算子(如:canny算子)获取轮廓边缘 ,对轮廓边缘采样得到一组离散的点集P={p1,p2,p3,..pn}，如图1中的字母A。

Step2:计算形状上下文。以其中任意一点pi为参考点,在pi为圆心、R为半径的局域内按对数距离间隔建立N个同心圆。将此区域沿圆周方向M等分，形成如图1所示的靶状模板。点pi到其它各点的向量相对位置简化为模板上各扇区内的点分布数。这些点的统计分布直方图hi(k),称为点pi的形状上下文,其计算公式为：

![h_i(k) = \#\{q \ne p_i  :  (q - p_i) \in \mbox{bin}(k)\}](https://wikimedia.org/api/rest_v1/media/math/render/svg/bc9202a50146d1575d929d677618c92e4b56d5ac)


其中，k={1,2,……,K},K=M*N；

![](https://img-blog.csdn.net/20160905075554966?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

图1

![](https://img-blog.csdn.net/20160905075734624?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


图2

采用对数距离分割可以使形状上下文描述子对邻近的采样点比远离点更敏感，能强化局部特征。轮廓不同点处的形状上下文是不同的，但相似轮廓的对应点处趋于有相似的形状上下文,如图2。

对于整个点集P ,分别以其n个点 p1,p2,p3,..pn作参考点，依次计算与剩下的n-1个点构成的形状直方图，最终得到 n个形状直方图。以n*(n-1)大小的矩阵存储。这样，对于任一目标，可用n*(n-1)大小的矩阵表示其形状信息，n*(n-1)大小的矩阵就是点集P的形状上下文,它描述了整个轮廓形状的特征。采样点越多，形状表达也越精细，计算量也会成倍加大。

利用步骤1,2得到每个目标的形状上下文后，接下来需要计算这两个目标的形状上下文的相似度，从而得到相似与否的结论。形状上下文之间的相似度计算方法叙述如下：

Step3：计算代价矩阵cost

 计算一个目标的形状直方图与另一目标的形状直方图之间的匹配代价，代价函数如下：

![](https://img-blog.csdn.net/20180109111717268) (1)


其中,hi(k)为目标P的点pi的形状直方图；hj(k)为目标Q的点qi的形状直方图。

按照公式，即可得到两个目标之间的代价矩阵C，大小为n*n。


Step4：


然后,基于计算得到的代价矩阵C，进行点的匹配操作，使公式(2)获得最小值。



![](https://img-blog.csdn.net/20180109111847669)                       （2）  
  


经分析，上述点的匹配问题即为典型的双向图的匹配问题。利用匈牙利算法，该问题得到解决。

至此，该方法以一个非矢量的数值表示两个目标的相似度。由于是基于代价矩阵进行计算。所以结果越大表示越不相似，结果越小表示越相似。



优点：

       充分利用上下文信息，在非刚性物体匹配中，具有很好的鲁棒性。

主要缺点：

       1、标准的形状上下文不能很好地解决物体类内部之间的变形，后期改进的基于内部距离的形状上下文改善了一点；

       2、对于有背景、噪声点过多的情况，匹配效果很不好.

### 3D  Shape Context

参考：Andrea Frome, Daniel Huber, Ravi Kolluri and Thomas Bülow, Jitendra Malik
    *     Recognizing Objects in Range Data Using Regional Point Descriptors,

    *     In proceedings of the 8th European Conference on Computer Vision (ECCV),

    *     Prague, May 11-14, 2004

3DSC是直接在2DSC上扩展而来，其中，将2DSC以采样点P为圆心，R为半径的圆区域，扩展为P为球心，R为半径的球区域。球的北极点，由重建法线来估计。其中，与2DSC类似，也有不同大小的同心求，半径计算方式为：

![](https://img-blog.csdn.net/20171007111544254?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

采样点，不再是计算梯度后的采样点，而是相对P点的距离，为了避免距离P点太近的点类似与噪声的干扰（也就是实际采样点太近，噪声的干扰影响程度更大），因此设置一个最小半径。

同心球示例：

![](https://img-blog.csdn.net/20171007111741535?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

这里要注意的是，相对原始2DSC中点数统计方式，3DSC增加了统计区域内点集的每个点不同权重。其中权重计算如下:

![](https://img-blog.csdn.net/20171007114313579?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

其中V（j,k,l）对应第j个同心球，第k个方位方向，第l个仰角方向区域对应的体积。ρ是对应局部点的密度，即以P点为球心，a为半径的球体内的点数。
其他类似2DSC，具体 可以看论文。




