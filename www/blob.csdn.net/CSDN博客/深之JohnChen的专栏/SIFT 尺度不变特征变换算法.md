# SIFT 尺度不变特征变换算法 - 深之JohnChen的专栏 - CSDN博客

2009年07月11日 14:33:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：7052


SIFT 尺度不变特征变换算法

David Lowe关于Sfit算法，2004年发表在Int. Journal of Computer Vision的经典论文中，对尺度空间（scal space)是这样定义的 ：
   It has been shown by Koenderink (1984) and Lindeberg (1994) that under a variety of
reasonable assumptions the only possible scale-space kernel is the Gaussian function. Therefore,
the scale space of an image is defined as a function, L(x; y; delta) that is produced from the convolution of a variable-scale Gaussian, G(x; y; delta), with an input image, I(x; y):
因此 ，一个图像的尺度空间，L（x,y,delta) ,定义为原始图像I (x,y)与一个可变尺度的2维高斯函数G(x,y,delta) 卷积运算。

    关于图象处理中的空间域卷积运算，可以参考经典的图像处理教材（比如 美国 冈萨雷斯的图象处理，第二版，或者其 Matlab版，都有如何在离散空间进行运算的例子和说明）

    注：原文中 delta为希腊字母，这里无法表示，用delta代替。

Sift算法中，提到了尺度空间，请问什么是尺度和尺度空间呢？

   在上述理解的基础上，尺度 就是受delta这个参数控制的表示 。
而不同的L（x,y,delta)就构成了尺度空间（ Space ,我理解，由于描述图像的时候，一般用连续函数比较好描述公式，所以，采用空间集合 ，空间的概念正规一些） ，实际上，具体计算的时候，即使连续的高斯函数，都要被离散为（一般为奇数大小）(2*k+1) *(2*k+1)矩阵，来和数字图像进行卷积运算。

1、 SIFT 算法提出及其改进

SIFT算法由D.G.Lowe 1999年提出，2004年完善总结。代表性文献

[1]David G. Lowe, **"Object recognition from local scale-invariant features,"***International Conference on Computer Vision,* Corfu, Greece (September 1999), pp.1150-1157.

[2] David G. Lowe, **"Distinctive image features from scale-invariant keypoints,"***International Journal of Computer Vision,* 60, 2 (2004), pp. 91-110.

具体的MATLAB代码在

[http://www.cs.ubc.ca/~lowe/keypoints/](http://www.cs.ubc.ca/~lowe/keypoints/) 可以下载。

Rob Hess 基于GSL和Opencv编写了C语言程序。具体的代码可以在

[http://web.engr.oregonstate.edu/~hess/index.html](http://web.engr.oregonstate.edu/~hess/index.html) 中下载，可以在VC++.net环境中运行，在调试时要注意对GSL和Opencv的正确配置。

后来Y.Ke将其描述子部分用PCA代替直方图的方式，对其进行改进。

[3] Y. Ke and R. Sukthankar. *PCA-SIFT: A More Distinctive Representation for Local Image Descriptors.*Computer Vision and Pattern Recognition, 2004

Yanke’s homepage:

[http://www.andrew.cmu.edu/user/yke/](http://www.andrew.cmu.edu/user/yke/)

2、 SIFT算法主要思想

SIFT算法是一种提取局部特征的算法，在尺度空间寻找极值点，提取位置，尺度，旋转不变量。

3、 SIFT算法的主要特点：

a) SIFT特征是图像的局部特征，其对旋转、尺度缩放、亮度变化保持不变性，对视角变化、仿射变换、噪声也保持一定程度的稳定性。

b) 独特性(Distinctiveness)好，信息量丰富，适用于在海量特征数据库中进行快速、准确的匹配[23]。

c) 多量性，即使少数的几个物体也可以产生大量SIFT特征向量。

d) 高速性，经优化的SIFT匹配算法甚至可以达到实时的要求。

e) 可扩展性，可以很方便的与其他形式的特征向量进行联合。

4、SIFT算法步骤：

Sift特征匹配算法主要包括两个阶段，一个是Sift特征的生成，即从多幅图像中提取对尺度缩放、旋转、亮度变化无关的特征向量；第二阶段是Sift特征向量的匹配。

     Sift特征的生成一般包括以下几个步骤：

     1、构建尺度空间，检测极值点，获得尺度不变性； 

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090711/sift1.jpg)

     2、特征点过滤并进行精确定位；

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090711/sift2.jpg)

     3、为特征点分配方向值；

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090711/sift3.gif)

     4、生成特征描述子。 

    以特征点为中心取16*16的邻域作为采样窗口，将采样点与特征点的相对方向通过高斯加权后归入包含8个bin的方向直方图，最后获得4*4*8的128维特征描述子。示意图如下：

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090711/sift4.gif)

    当两幅图像的Sift特征向量生成以后，下一步就可以采用关键点特征向量的欧式距离来作为两幅图像中关键点的相似性判定度量。取图1的某个关键点，通过遍历找到图像2中的距离最近的两个关键点。在这两个关键点中，如果次近距离除以最近距离小于某个阙值，则判定为一对匹配点。

    一些Sift特征匹配的例子：

![](https://p-blog.csdn.net/images/p_blog_csdn_net/byxdaz/EntryImages/20090711/sift5.jpg)

5、特征提取函数的使用

作为一种匹配能力较强的局部描述算子，SIFT算法的实现相当复杂，但从软件开发的角度来说，只要会使用其中几个比较重要的函数就行了。这里要感谢David Lowe这个大牛，不但提供了一种强悍的特征匹配算法，还给出了C++的实现代码，后来有人用C#实现了这个算法，需要的朋友可到网上自行下载。

     关键函数一：

   int sift_features( IplImage* img, struct feature** feat )

   这个函数就是用来提取图像中的特征向量。参数img为一个指向IplImage数据类型的指针，用来表示需要进行特征提取的图像。IplImage是opencv库定义的图像基本类型（关于opencv是一个著名的图像处理类库，详细的介绍可以参见http://www.opencv.org.cn）。参数feat 是一个数组指针，用来存储图像的特征向量。函数调用成功将返回特征向量的数目，否则返回-1.

   关键函数二：

   int _sift_features( IplImage* img, struct feature** feat, int intvls,double sigma, double contr_thr, int curv_thr, int img_dbl, int descr_width, int descr_hist_bins )

     这个函数是函数一的重载，作用是一样的，实际上函数一只不过是使用默认参数调用了函数二，核心的代码都是在函数二中实现的。下面将详细介绍一下其他的几个参数。

   intvls: 每个尺度空间的采样间隔数，默认值为3.

   sigma: 高斯平滑的数量，默认值1.6.

   contr_thr:判定特征点是否稳定，取值（0，1），默认为0.04，这个值越大，被剔除的特征点就越多。

   curv_thr:判定特征点是否边缘点，默认为6.

   img_dbl:在建立尺度空间前如果图像被放大了1倍则取值为1，否则为0.

   descr_width:计算特征描述符时邻域子块的宽度，默认为4.

     descr_hist_bins:计算特征描述符时将特征点邻域进行投影的方向数，默认为8，分别是0，45，90，135，180，215，270，315共8个方向。

      如果只是做特征提取，上面这个函数就足够了，不同图像之间的匹配都是建立在这个基础上的，如果你有足够强的编程能力即使不懂这个算法也能够编写出像样的程序来，因为下面的比较说白了都是数据结构的比较，跟算法已经没有太多关系了。如果想要深入了解算法，还是认真的看论文和代码吧。

Sift算法与VC代码：[http://download.csdn.net/source/1481248](http://download.csdn.net/source/1481248)

