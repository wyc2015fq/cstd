# 图像局部特征（十一）--ORB描述子 - 工作笔记 - CSDN博客





2016年06月26日 22:32:14[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：7889
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









﻿﻿

# Oriented FAST and Rotated BRIEF

[www.cnblogs.com/ronny](http://www.cnblogs.com/ronny/)



这篇文章我们将介绍一种新的具有局部不变性的特征 —— ORB特征，从它的名字中可以看出它是对FAST特征点与BREIF特征描述子的一种结合与改进，这个算法是由Ethan Rublee,Vincent Rabaud,Kurt Konolige以及Gary R.Bradski在2011年一篇名为“ORB：An Efficient Alternative to SIFT or SURF”的文章中提出。就像文章题目所写一样，ORB是除了SIFT与SURF外一个很好的选择，而且它有很高的效率，最重要的一点是它是免费的，SIFT与SURF都是有专利的，你如果在商业软件中使用，需要购买许可。

如果你对FAST特征点与BRIEF特征描述子不了解，请先阅读以下两篇文章。本文不打算对它们作详细的算法说明。
- [FAST特征点检测](http://www.cnblogs.com/ronny/p/4078710.html)
- [BRIEF特征描述子](http://www.cnblogs.com/ronny/p/4081362.html)

# 1. ORB的算法原理

ORB特征是将FAST特征点的检测方法与BRIEF特征描述子结合起来，并在它们原来的基础上做了改进与优化。

首先，它利用FAST特征点检测的方法来检测特征点，然后利用Harris角点的度量方法，从FAST特征点从挑选出Harris角点响应值最大的*N* N个特征点。其中Harris角点的响应函数定义为：



*R*=*d**e**t**M*−*α*(*t**r**a**c**e**M*) 2  R=detM−α(traceM)2





关于*M* M的含义和响应函数的由来可以参考[Harris角点检测](http://www.cnblogs.com/ronny/p/4009425.html)这篇文章。

## 1.1 旋转不变性

我们知道FAST特征点是没有尺度不变性的，所以我们可以通过构建高斯金字塔，然后在每一层金字塔图像上检测角点，来实现尺度不变性。那么，对于局部不变性，我们还差一个问题没有解决，就是FAST特征点不具有方向，ORB的论文中提出了一种利用灰度质心法来解决这个问题，灰度质心法假设角点的灰度与质心之间存在一个偏移，这个向量可以用于表示一个方向。对于任意一个特征点*p* p来说，我们定义*p* p的邻域像素的矩为：



*m**p**q* =∑ *x*,*y**x**p**y**q**I*(*x*,*y*) mpq=∑x,yxpyqI(x,y)





其中*I*(*x*,*y*) I(x,y)为点(*x*,*y*) (x,y)处的灰度值。那么我们可以得到图像的质心为：



*C*=(*m* 10 *m* 00  ,*m* 01 *m* 00  ) C=(m10m00,m01m00)





那么特征点与质心的夹角定义为FAST特征点的方向：



*θ*=*a**r**c**t**a**n*(*m* 01 ,*m* 10 ) θ=arctan(m01,m10)





为了提高方法的旋转不变性，需要确保*x* x和*y* y在半径为*r* r的圆形区域内，即*x*,*y*∈[−*r*,*r*] x,y∈[−r,r]，*r* r等于邻域半径。

## 1.2 特征点的描述

ORB选择了BRIEF作为特征描述方法，但是我们知道BRIEF是没有旋转不变性的，所以我们需要给BRIEF加上旋转不变性，把这种方法称为“Steer BREIF”。对于任何一个特征点来说，它的BRIEF描述子是一个长度为*n* n的二值码串，这个二值串是由特征点周围*n* n个点对（2*n* 2n个点）生成的，现在我们将这2*n* 2n个点(*x**i* ,*y**i* ),*i*=1,2,⋯,2*n* (xi,yi),i=1,2,⋯,2n组成一个矩阵*S* S



*S*=(*x* 1 *y* 1  *x* 2 *y* 2  ⋯⋯ *x* 2*n**y* 2*n*  ) S=(x1x2⋯x2ny1y2⋯y2n)





Calonder建议为每个块的旋转和投影集合分别计算BRIEF描述子，但代价昂贵。ORB中采用了一个更有效的方法：使用邻域方向*θ* θ和对应的旋转矩阵*R**θ*  Rθ，构建*S* S的一个校正版本*S**θ*  Sθ



*S**θ* =*R**θ**S* Sθ=RθS





其中

*R**θ* =[*c**o**s**θ*–*s**i**n**θ**s**i**n**θ**c**o**s**θ* ] Rθ=[cosθsinθ–sinθcosθ]





而*θ* θ即我们在1.2中为特征点求得的主方向。

实际上，我们可以把角度离散化，即把360度分为12份，每一份是30度，然后我们对这个12个角度分别求得一个*S**θ*  Sθ，这样我们就创建了一个查找表，对于每一个*θ* θ，我们只需查表即可快速得到它的点对的集合*S**θ*  Sθ。

## 1.3 解决描述子的区分性

BRIEF令人惊喜的特性之一是：对于*n* n维的二值串的每个比特征位，所有特征点在该位上的值都满足一个均值接近于0.5，而方差很大的高斯分布。方差越大，说明区分性越强，那么不同特征点的描述子就表现出来越大差异性，对匹配来说不容易误配。但是当我们把BRIEF沿着特征点的方向调整为Steered BRIEF时，均值就漂移到一个更加分散式的模式。可以理解为有方向性的角点关键点对二值串则展现了一个更加均衡的表现。而且论文中提到经过PCA对各个特征向量进行分析，得知Steered BRIEF的方差很小，判别性小，各个成分之间相关性较大。

为了减少Steered BRIEF方差的亏损，并减少二进制码串之间的相关性，ORB使用了一种学习的方法来选择一个较小的点对集合。方法如下：

首先建立一个大约300k关键点的测试集，这些关键点来自于PASCAL2006集中的图像。

对于这300k个关键点中的每一个特征点，考虑它的31×31 31×31的邻域，我们将在这个邻域内找一些点对。不同于BRIEF中要先对这个Patch内的点做平滑，再用以Patch中心为原点的高斯分布选择点对的方法。ORB为了去除某些噪声点的干扰，选择了一个5×5 5×5大小的区域的平均灰度来代替原来一个单点的灰度，这里5×5 5×5区域内图像平均灰度的计算可以用积分图的方法。我们知道31×31 31×31的Patch里共有*N*=(31−5+1)×(31−5+1) N=(31−5+1)×(31−5+1)个这种子窗口，那么我们要*N* N个子窗口中选择2个子窗口的话，共有*C* 2 *N*  CN2种方法。所以，对于300k中的每一个特征点，我们都可以从它的31×31 31×31大小的邻域中提取出一个很长的二进制串，长度为*M*=*C* 2 *N*  M=CN2，表示为

*b**i**n**A**r**r**a**y*=[*p* 1 ,*p* 2 ,⋯,*p**M* ],*p**i* ∈{0,1} binArray=[p1,p2,⋯,pM],pi∈{0,1}





那么当300k个关键点全部进行上面的提取之后，我们就得到了一个300*k*×*M* 300k×M的矩阵，矩阵中的每个元素值为0或1。

对该矩阵的每个列向量，也就是每个点对在300k个特征点上的测试结果，计算其均值。把所有的列向量按均值进行重新排序。排好后，组成了一个向量*T* T，*T* T的每一个元素都是一个列向量。

进行贪婪搜索：从*T* T中把排在第一的那个列放到*R* R中，*T* T中就没有这个点对了测试结果了。然后把*T* T中的排下一个的列与*R* R中的所有元素比较，计算它们的相关性，旭果相关超过了某一事先设定好的阈值，就扔了它，否则就把它放到*R* R里面。重复上面的步骤，只到*R* R中有256个列向量为止。如果把*T* T全找完也，也没有找到256个，那么，可以把相关的阈值调高一些，再重试一遍。

这样，我们就得到了256个点对。上面这个过程我们称它为rBRIEF。

# 2. OpenCV中的ORB

ORB中有很多参数可以设置，在OpenCV中它可以通过ORB来创建一个ORB检测器。
`ORB::ORB(int nfeatures=500, float scaleFactor=1.2f, int nlevels=8, int edgeThreshold=31, int firstLevel=0, int WTA_K=2, int scoreType=ORB::HARRIS_SCORE, int patchSize=31)`
下面介绍一下各个参数的含义：

**nfeatures** - 最多提取的特征点的数量；

**scaleFactor** - 金字塔图像之间的尺度参数，类似于SIFT中的*k* k；

**nlevels** – 高斯金字塔的层数；

**edgeThreshold** – 边缘阈值，这个值主要是根据后面的patchSize来定的，靠近边缘edgeThreshold以内的像素是不检测特征点的。

**firstLevel** - 看过SIFT都知道，我们可以指定第一层的索引值，这里默认为0。

**WET_K** - 用于产生BIREF描述子的 点对的个数，一般为2个，也可以设置为3个或4个，那么这时候描述子之间的距离计算就不能用汉明距离了，而是应该用一个变种。OpenCV中，如果设置WET_K = 2，则选用点对就只有2个点，匹配的时候距离参数选择NORM_HAMMING，如果WET_K设置为3或4，则BIREF描述子会选择3个或4个点，那么后面匹配的时候应该选择的距离参数为NORM_HAMMING2。

**scoreType** - 用于对特征点进行排序的算法，你可以选择HARRIS_SCORE，也可以选择FAST_SCORE，但是它也只是比前者快一点点而已。

**patchSize** – 用于计算BIREF描述子的特征点邻域大小。

![复制代码](http://common.cnblogs.com/images/copycode.gif)

```
#include <opencv2/core/core.hpp> 
#include <opencv2/highgui/highgui.hpp> 
#include <opencv2/imgproc/imgproc.hpp> 
#include <opencv2/features2d/features2d.hpp>

using namespace cv;

int main(int argc, char** argv) 
{ 
    Mat img_1 = imread("box.png"); 
    Mat img_2 = imread("box_in_scene.png");

    // -- Step 1: Detect the keypoints using STAR Detector 
    std::vector<KeyPoint> keypoints_1,keypoints_2; 
    ORB orb; 
    orb.detect(img_1, keypoints_1); 
    orb.detect(img_2, keypoints_2);

    // -- Stpe 2: Calculate descriptors (feature vectors) 
    Mat descriptors_1, descriptors_2; 
    orb.compute(img_1, keypoints_1, descriptors_1); 
    orb.compute(img_2, keypoints_2, descriptors_2);

    //-- Step 3: Matching descriptor vectors with a brute force matcher 
    BFMatcher matcher(NORM_HAMMING); 
    std::vector<DMatch> mathces; 
    matcher.match(descriptors_1, descriptors_2, mathces); 
    // -- dwaw matches 
    Mat img_mathes; 
    drawMatches(img_1, keypoints_1, img_2, keypoints_2, mathces, img_mathes); 
    // -- show 
    imshow("Mathces", img_mathes);

    waitKey(0); 
    return 0; 
}
```

![复制代码](http://common.cnblogs.com/images/copycode.gif)

![image](http://images.cnitblog.com/blog/378920/201411/081736292998502.png)

# 3. 参考资料

[1] Ethan Rublee, Vincent Rabaud, Kurt Konolige, Gary R. Bradski: ORB: An efficient alternative to SIFT or SURF. ICCV 2011: 2564-2571.

[2] [看ORB特征，一些理解和解释](http://blog.sina.com.cn/s/blog_567a58300101iarh.html)

[3] [OpenCV Tutorials](http://docs.opencv.org/trunk/doc/py_tutorials/py_feature2d/py_orb/py_orb.html#orb)



