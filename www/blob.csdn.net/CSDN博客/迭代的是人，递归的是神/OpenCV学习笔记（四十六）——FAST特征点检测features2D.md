# OpenCV学习笔记（四十六）——FAST特征点检测features2D - 迭代的是人，递归的是神 - CSDN博客





2012年03月30日 16:50:32[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：50873
所属专栏：[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html)









特征点检测和匹配是计算机视觉中一个很有用的技术。在物体检测，视觉跟踪，三维常年关键等领域都有很广泛的应用。这一次先介绍特征点检测的一种方法——FAST（features from accelerated segment test）。很多传统的算法都很耗时，而且特征点检测算法只是很多复杂图像处理里中的第一步，得不偿失。FAST特征点检测是公认的比较快速的特征点检测方法，只利用周围像素比较的信息就可以得到特征点，简单，有效。

FAST特征检测算法来源于corner的定义，这个定义基于特征点周围的图像灰度值，检测候选特征点周围一圈的像素值，如果候选点周围领域内有足够多的像素点与该候选点的灰度值差别够大，则认为该候选点为一个特征点。

![](https://img-my.csdn.net/uploads/201203/30/1333094316_3973.png)


其中I（x）为圆周上任意一点的灰度，I（p）为圆心的灰度，Ed为灰度值差得阈值，如果N大于给定阈值，一般为周围圆圈点的四分之三，则认为p是一个特征点。

为了获得更快的结果，还采用了额外的加速办法。如果测试了候选点周围每隔90度角的4个点，应该至少有3个和候选点的灰度值差足够大，否则则不用再计算其他点，直接认为该候选点不是特征点。候选点周围的圆的选取半径是一个很重要的参数，这里我为了简单高效，采用半径为3，共有16个周边像素需要比较。为了提高比较的效率，通常只使用N个周边像素来比较，也就是大家经常说的FAST-N。我看很多文献推荐FAST-9，作者的主页上有FAST-9、FAST-10、FAST-11、FAST-12，大家使用比较多的是FAST-9和FAST-12。上个图说明的更形象一些


![](https://img-my.csdn.net/uploads/201203/30/1333094661_2934.png)


OpenCV里对FAST的使用也非常简单，先声明一组特征点，构建FAST特征检测，接下来调用detect函数检测图像中的特征点，最后把特征点绘制到图片上。上代码说的清楚些



```cpp
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <vector>

using namespace cv;

void main()
{
	Mat image;
	image = imread("church01.jpg");
	// vector of keyPoints
	std::vector<KeyPoint> keyPoints;
	// construction of the fast feature detector object
	FastFeatureDetector fast(40);	// 检测的阈值为40
	// feature point detection
	fast.detect(image,keyPoints);
	drawKeypoints(image, keyPoints, image, Scalar::all(255), DrawMatchesFlags::DRAW_OVER_OUTIMG);
	imshow("FAST feature", image);
	cvWaitKey(0);
}
```


最后上一张效果图



![](https://img-my.csdn.net/uploads/201203/30/1333097601_3417.png)




