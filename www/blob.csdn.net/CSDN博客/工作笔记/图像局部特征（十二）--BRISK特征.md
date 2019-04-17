# 图像局部特征（十二）--BRISK特征 - 工作笔记 - CSDN博客





2016年07月18日 15:46:21[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5419
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









# 原文：

# http://www.mamicode.com/info-detail-940675.html

# 简介

        BRISK算法是2011年ICCV上《[BRISK:Binary Robust Invariant Scalable Keypoints](http://www.robots.ox.ac.uk/~vgg/rg/papers/brisk.pdf)》文章中，提出来的一种特征提取算法，也是一种二进制的特征描述算子。

       它具有较好的旋转不变性、尺度不变性，较好的鲁棒性。在图像配准应用中，速度比较：SIFT>SURF>BRISK>FREAK>ORB，在对有较大模糊的图像配准时，BRISK算法在其中表现最为出色。

# BRISK算法

## 特征点检测

        BRISK算法主要利用FAST9-16进行特征点检测（为什么是主要？因为用到一次FAST5-8），可参见博客：[FAST特征点检测算法](http://blog.csdn.net/hujingshuang/article/details/46898007)。要解决尺度不变性，就必须在尺度空间进行特征点检测，于是BRISK算法中构造了图像金字塔进行多尺度表达。

### 建立尺度空间

        构造n个octave层（用ci表示）和n个intra-octave层（用di表示），文章中n=4，i={0,1,...,n-1}。假设有图像img，octave层的产生：c0层就是img原图像，c1层是c0层的2倍下采样，c2层是c1层的2倍下采样，以此类推。intra-octave层的产生：d0层是img的1.5倍下采样，d2层是d1层的2倍下采样（即img的2*1.5倍下采样），d3层是d2层的2倍下采样，以此类推。

则ci、di层与原图像的尺度关系用t表示为：![技术分享](https://img-blog.csdn.net/20150724194447261)，![技术分享](https://img-blog.csdn.net/20150724194459180)

ci、di层与原图像大小关系为：

![技术分享](https://img-blog.csdn.net/20150724195813954)

        由于n=4，所以一共可以得到8张图，octave层之间尺度（缩放因子）是2倍关系，intra-octave层之间尺度（缩放因子）也是2倍关系。

### 特征点检测

        对这8张图进行FAST9-16角点检测，得到具有角点信息的8张图，对原图像img进行一次FAST5-8角点检测（当做d(-1)层，虚拟层），总共会得到9幅有角点信息的图像。

### 非极大值抑制

        对这9幅图像，进行空间上的非极大值抑制（同SIFT算法 的非极大值抑制）：特征点在位置空间（8邻域点）和尺度空间（上下层2x9个点），共26个邻域点的FAST的得分值要是最大，否则不能当做特征点；此时得到的极值点还比较粗糙，需要进一步精确定位。

### 亚像素插值

        进过上面步骤，得到了图像特征点的位置和尺度，在极值点所在层及其上下层所对应的位置，对FAST得分值（共3个）进行二维二次函数插值（x、y方向），得到真正意义上的得分极值点及其精确的坐标位置（作为特征点位置）；再对尺度方向进行一维插值，得到极值点所对应的尺度（作为特征点尺度）。

![技术分享](https://img-blog.csdn.net/20150724202752705)

## 特征点描述

### 高斯滤波

         现在，我们得到了特征点的位置和尺度（t）后，要对特征点赋予其描述符。为避免降采样引起的混叠效应，以特征点为中心，在其周围采样N个点（包括特征点），采样模式如下图，蓝圈表示；以采样点为中心，![技术分享](https://img-blog.csdn.net/20150724203701931)为方差进行高斯滤波，滤波半径大小对应方差的大小，红圈表示。最终用到的N个采样点是经过高斯平滑后的采样点。（文章中：N=60）

![技术分享](https://img-blog.csdn.net/20150724203102693)

### 局部梯度计算

         由于有N个采样点，则采样点两两组合成一对，共有N(N-1)/2钟组合方式，所有组合方式的集合称作采样点对，用集合![技术分享](https://img-blog.csdn.net/20150724210354084)表示，其中像素分别是![技术分享](https://img-blog.csdn.net/20150724205455946)、![技术分享](https://img-blog.csdn.net/20150724205503761)，δ表示尺度。用![技术分享](https://img-blog.csdn.net/20150724205606629)表示特征点局部梯度集合，则有：

![技术分享](https://img-blog.csdn.net/20150724205737909)

定义短距离点对子集、长距离点对子集（L个）：

![技术分享](https://img-blog.csdn.net/20150724220105361)

其中，![技术分享](https://img-blog.csdn.net/20150724221534361)，![技术分享](https://img-blog.csdn.net/20150724221422050)，t是特征点所在的尺度。

现在要利用上面得到的信息，来计算特征点的主方向（注意：此处只用到了长距离点对子集），如下：

![技术分享](https://img-blog.csdn.net/20150724221720328)

![技术分享](https://img-blog.csdn.net/20150724222152069)

### 特征描述符

         要解决旋转不变性，则需要对特征点周围的采样区域进行旋转到主方向，旋转后得到新的采样区域，采样模式同上。BRISK描述子是二进制的特征，由采样点集合可得到N(N-1)/2对采样点对，就可以得到N(N-1)/2个距离的集合（包含长、短距离子集），考虑其中短距离子集中的512个短距离点对，进行二进制编码，判断方式如下：

![技术分享](https://img-blog.csdn.net/20150724223506558)

其中，![技术分享](https://img-blog.csdn.net/20150724223723500)带有上标，表示经过旋转a角度后的，新的采样点。由此可得到，512Bit的二进制编码，也就是64个字节（BRISK64）。

## 匹配方法

汉明距离进行比较，与其他二进制描述子的匹配方式一样。



```cpp
#include <cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <Windows.h>

using namespace cv;
using namespace std;

int main()
{
	//Load Image
	Mat c_src1 = imread("box.png");
	Mat c_src2 = imread("box_in_scene.png");
	Mat src1 = imread("box.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat src2 = imread("box_in_scene.png", CV_LOAD_IMAGE_GRAYSCALE);
	if (!src1.data || !src2.data)
	{
		cout << "Error reading images " << std::endl;
		return -1;
	}
	//feature detect
	BRISK detector;
	vector<KeyPoint> kp1, kp2;
	double start = GetTickCount();
	detector.detect(src1, kp1);
	detector.detect(src2, kp2);
	//cv::BRISK extractor;
	Mat des1, des2;//descriptor
	detector.compute(src1, kp1, des1);
	detector.compute(src2, kp2, des2);
	Mat res1, res2;
	int drawmode = DrawMatchesFlags::DRAW_RICH_KEYPOINTS;
	drawKeypoints(c_src1, kp1, res1, Scalar::all(-1), drawmode);//画出特征点
	drawKeypoints(c_src2, kp2, res2, Scalar::all(-1), drawmode);
	cout << "size of description of Img1: " << kp1.size() << endl;
	cout << "size of description of Img2: " << kp2.size() << endl;

	BFMatcher matcher(NORM_HAMMING);
	vector<DMatch> matches;
	matcher.match(des1, des2, matches);
	double end = GetTickCount();
	cout << "耗时：" << (end - start) << "ms" << endl;
	Mat img_match;
	drawMatches(src1, kp1, src2, kp2, matches, img_match);
	cout << "number of matched points: " << matches.size() << endl;
	imshow("matches", img_match);
	cvWaitKey(0);
	cvDestroyAllWindows();
	return 0;
}
```









