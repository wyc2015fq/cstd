# FAST角点检测方法详解 - tostq的专栏 - CSDN博客





2015年10月21日 23:14:06[tostq](https://me.csdn.net/tostq)阅读数：9098








FAST（Features from Accelerated Segment Test）由Edward Rosten和Tom Drummond在2006年首先提出，是近年来一总倍受关注的基于模板和机器学习的角点检测方法，它不仅计算速度快，还具有较高的精确度。本文将通过其2006年的原文，详细介绍FAST的基本原理。




**一、FAST算法原理**

FAST的方法主要是考虑像素点附近的圆形窗口上的16个像素，如下图所示，p为中心像素点，而白框标示的点像素则是我们需要考虑的点。

![](https://img-blog.csdn.net/20151021231920849?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


最初的检测方法就是检测在这样的圆环上的16个像素点中，如果有n个连续的点都比中心像素p的强度都大，或都小的话，这样的中心点就是角点，实际上比较强度时，需要加上阈值t。

![](https://img-blog.csdn.net/20151021231957949?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


上图的公式说明了这一过程，一般情况下，n是取12，所以这个标准定义为FAST-12，而实际上当n=9时，往往能取得较好的效果。

![](https://img-blog.csdn.net/20151021232015054?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


如果要提高检测速度的话，只需要检测四个点就可以了，首先比较第1和第9个像素，如果两个点像素强度都在中心像素强度t变化范围内（及都同中心点相似），则说明这不是角点，如果接下来检测第5和13点时，发现上述四点中至少有三个点同中心点不相似，则可以说明这是个角点。之后为了提高精度，我们还可以对上面运算后的剩下的候选角点进行全部的16点检测，从而确定其是不是角点。

然后这个方法也有如下的几个缺点：

（1）如果在首先的四点检测里，只有2个点同中心点不相似，也并不能说明这不是角点。

（2）检测的效率严重依赖于检测点的顺序和角点附近的分布，很难说明所选择比较的像素位置能最好的反应角点性能。

（3）前面的四点检测结果没能充分用到后面检测上来。

（4）并连在一起的特征点很可能检测到了相邻的位置。

所以，为了解决上述问题，接下来，本文将重点介绍FAST算法最为精髓的地方——通过机器学习来改善速度和通用性。



**二、机器学习改善速度及通用性**

通过机器学习的方法，我们可以解决上面提到的前三个问题，而第四个问题可以通过非极大值抑制来解决（之后会介绍）。机器学习方法采用的是决策树方法，通过对像素点进行分类，找到角点附近的16点位置中最能区分这个分类的位置（实际上就指决策树每层的根），而叶则用来指明是否是角点。其主要分为两个部分，首先给定n，对所以的16像素圆环建立FAST-n检测，然后从一组图像内（最好是同目标图像相似）提取大量的角点。

（1）对于这个16个像素的每个位置x[1,16]，将其同中心像素比较，获得三个状态，如下

![](https://img-blog.csdn.net/20151021232222163?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


选择一个位置x，将所有训练图像中的所以像素点（记为集P）同其该位置点进行上述等式比较，分别计算每点的状态
![](https://img-blog.csdn.net/20151021232246711?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，由此以来，对于每个位置x，我们都可以都得到一个状态集，而每个状态就是指一个像素（集P内的每个像素）同其附近（圆环上）该位置像素的状态。

（2）然后我们选择一个x位置的状态集，可以将集P内的像素点根据其在状态集对应位置上状态，分成三个部分![](https://img-blog.csdn.net/20151021232309403?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，其意思是指该子集内的像素，其附近圆环x位置的像素同比较，是更暗（d集），相似（s集）或更亮（b）。

（3）之后再定义一个布尔变量![](https://img-blog.csdn.net/20151021232341001?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)来确定当前像素点p是否是角点。对于任意一个点集Q，我们可以计算总共的熵值：

![](https://img-blog.csdn.net/20151021232403562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20151021232418580?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

对于一个位置x来说，它分类的信息量为：

![](https://img-blog.csdn.net/20151021232503092?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（4）接下来，我们就是选择信息量最大的x位置，同时在其下面的子集![](https://img-blog.csdn.net/20151021232526275?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)内继续迭代选择信息量最大的x位置（每个子集内选择x位置可以分别写成xd，xs，xb），然后继续将子集再次分割，如将![](https://img-blog.csdn.net/20151021232627690?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)。每一次分割所选的x都是通将集分类成拥有最大信息量的位置。

为什么要是最大信息量的位置呢？最大信息量表明分成的子集的信息量最少，也就意味着子集内的点都是具有相同类似的点（即都是角点，或都不是角点），所以最后我们将通过训练集得到一个决策树，其能将像素点较好的区分是否是角点。下图是决策树模型。

![](https://img-blog.csdn.net/20151021232705219?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（5）为了更好的优化，我们强制让xd，xs，xb相等，这样的话，我们选择第二个测试的位置点将会一样，只需要两个位置点的比较，我们就能排除绝大多数的点，如此以来将让速度大大提高。



**三、非极大值抑制**

因为FAST方法没有计算角响应度，所以我们很难采用常规的方法来直接进行非极大值抑制，对于一个给定的n，如果阈值t增加，那么检测的角点数将会减少，因为角点强度可以被定义为如果这个点可以被检测成角点时，t所能取到的最大值。

决策树能根据一个给定的t，非常高效的确定一个点是否是角点，由此以来，我们可以通过变化t，来确定找到该点由角点变为非角点时的t值，而这个t值就是让该点检测为角点的最大阈值，这个查找方法可以用二分法的解决。或者，我们也可以用一个迭代的方法。

找到了角响应度的衡量后，我们就可以应用原来的非极大值抑制方法了，最终得到我们想要的角点。



**参考文献**

[Machine Learning for High-Speed CornerDetection](http://download.csdn.net/detail/tostq/9201463)



