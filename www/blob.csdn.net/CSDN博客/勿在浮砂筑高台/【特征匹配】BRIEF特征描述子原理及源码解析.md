# 【特征匹配】BRIEF特征描述子原理及源码解析 - 勿在浮砂筑高台 - CSDN博客





置顶2015年09月10日 09:17:59[勿在浮砂筑高台](https://me.csdn.net/luoshixian099)阅读数：12664
所属专栏：[特征匹配](https://blog.csdn.net/column/details/featuresdetect.html)









相关：[Fast原理及源码解析](http://blog.csdn.net/luoshixian099/article/details/48294967)

[Harris原理及源码解析](http://blog.csdn.net/luoshixian099/article/details/48244255)

[SIFT原理及源码解析](http://blog.csdn.net/luoshixian099/article/details/47377611)

[SURF原理及源码解析](http://blog.csdn.net/luoshixian099/article/details/47807103)

转载请注明出处： http://blog.csdn.net/luoshixian099/article/details/48338273

  传统的特征点描述子如SIFT,SURF描述子，每个特征点采用128维(SIFT)或者64维(SURF)向量去描述，每个维度上占用4字节，SIFT需要128×4=512字节内存，SURF则需要256字节。如果对于内存资源有限的情况下，这种描述子方法显然不适应。同时，在形成描述子的过程中，也比较耗时。后来有人提出采用PCA降维的方法，但没有解决计算描述子耗时的问题。

   鉴于上述的缺点Michael Calonder等人在论文提出BRIEF描述特征点的方法(BRIEF:Binary Robust Independent Elementary Features)。BRIEF描述子采用二进制码串(每一位非1即0)作为描述子向量，论文中考虑长度有128,256,512几种，同时形成描述子算法的过程简单，由于采用二进制码串，匹配上采用汉明距离，（一个串变成另一个串所需要的最小替换次数）。但由于BRIEF描述子不具有方向性，大角度旋转会对匹配上有很大的影响。

   BRIRF只提出了描述特征点的方法，所以特征点的检测部分必须结合其他的方法，如SIFT,SURF等，但论文中建议与Fast结合，因为会更能体现出Brirf速度快等优点。

--------------------------------------------------------------------------------------------------

BRIEF描述子原理简要为三个步骤，长度为N的二进制码串作为描述子(占用内存N/8)：

   1.以特征点P为中心，取一个S×S大小的Patch邻域；

   2.在这个邻域内随机取N对点，然后对这2×N点分别做高斯平滑。定义τ测试，比较N对像素点的灰度值的大小；

![](https://img-blog.csdn.net/20150909213516190)


   3.最后把步骤2得到的N个二进制码串组成一个N维向量即可；

![](https://img-blog.csdn.net/20150909213526906)

-----------------------------------------------------------------------------------------------------

原理解析：

__1.关于做τ测试前，需要对随机点做高斯平滑，由于采用单个的像素灰度值做比较，会对噪声很敏感；采用高斯平滑图像，会降低噪声的影响，使得         描述子更加稳定。论文中建议采用9×9的kernal。

__2.论文中对随机取N对点采用了5中不同的方法做测试，论文中建议采用G II的方法：

![](https://img-blog.csdn.net/20150909214955835)


             G I :（X,Y）~（-S/2,S/2）分布，X,Y即均匀分布；

             G II: ![](https://img-blog.csdn.net/20150909215651412)，X,Y均服从高斯分布；

             G III: ![](https://img-blog.csdn.net/20150909215812791)，先随机取X点，再以X点为中心，取Y点；

             G IV: 在空间量化极坐标系下，随机取2N个点；

             G V: X固定在中心，在Patch内，Y在极坐标系中尽可能取所有可能的值；

__3.最后汉明距离的计算，直接比较两二进制码串的距离，距离定义为：其中一个串变成另一个串所需要的最少操作。因而比欧氏距离运算速度快.

      如果取N=128,即每个特征点需要128/8=16个字节内存大小作为其描述子。

![](https://img-blog.csdn.net/20150909230120889)





**OPENCV源码解析：**



```cpp
#include <stdio.h>
#include <iostream>
#include "cv.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
using namespace std;
using namespace cv;
int main( int argc, char** argv )
{

	Mat img_1 = imread( "F:\\Picture\\book.jpg", CV_LOAD_IMAGE_GRAYSCALE );
	Mat img_2 = imread( "F:\\Picture\\book_2.jpg", CV_LOAD_IMAGE_GRAYSCALE );

	if( !img_1.data || !img_2.data )
	{ 
		return -1; }
	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;
	SurfFeatureDetector detector( minHessian);    //采用Surf特征点检测
	std::vector<KeyPoint> keypoints_1, keypoints_2;
	detector.detect( img_1, keypoints_1 );
	detector.detect( img_2, keypoints_2 );
	//-- Step 2: Calculate descriptors (feature vectors)
	BriefDescriptorExtractor  extractor(64);  //参数表示字节数,采用长度为64×8=512的向量表示，见下方分析
	Mat descriptors_1, descriptors_2;
	extractor.compute( img_1, keypoints_1, descriptors_1 );
	extractor.compute( img_2, keypoints_2, descriptors_2 );
	//-- Step 3: Matching descriptor vectors with a brute force matcher
	BFMatcher  matcher(NORM_HAMMING);   //汉明距离匹配特征点
	std::vector< DMatch > matches;
	matcher.match( descriptors_1, descriptors_2, matches );
	//-- Draw matches
	Mat img_matches;
	drawMatches( img_1, keypoints_1, img_2, keypoints_2, matches, img_matches );
	////-- Show detected matches
	imshow("Matches", img_matches );
	waitKey(0);
	return 0;
}
```

Brief描述子的类定义：

注意bytes参数表示的是描述子占用的字节数不是描述子长度，如默认采用32字节对应描述子长度为32×8=256；



```cpp
/*
 * BRIEF Descriptor
 */
class CV_EXPORTS BriefDescriptorExtractor : public DescriptorExtractor
{
public:
    static const int PATCH_SIZE = 48;  //邻域范围
    static const int KERNEL_SIZE = 9;//平滑积分核大小

    // bytes is a length of descriptor in bytes. It can be equal 16, 32 or 64 bytes.
    BriefDescriptorExtractor( int bytes = 32 );  //占用字节数32,对应描述子长度为32×8=256;

    virtual void read( const FileNode& );
    virtual void write( FileStorage& ) const;

    virtual int descriptorSize() const;
    virtual int descriptorType() const;

    /// @todo read and write for brief

    AlgorithmInfo* info() const;

protected:
    virtual void computeImpl(const Mat& image, vector<KeyPoint>& keypoints, Mat& descriptors) const; //计算特征描述子函数

    typedef void(*PixelTestFn)(const Mat&, const vector<KeyPoint>&, Mat&); //不同长度的描述子调用不同的函数

    int bytes_;//占用字节数
    PixelTestFn test_fn_;
};
```

计算特征描述子函数：



```cpp
void BriefDescriptorExtractor::computeImpl(const Mat& image, std::vector<KeyPoint>& keypoints, Mat& descriptors) const
{
    // Construct integral image for fast smoothing (box filter)
    Mat sum;

    Mat grayImage = image;
    if( image.type() != CV_8U ) cvtColor( image, grayImage, CV_BGR2GRAY );

    ///TODO allow the user to pass in a precomputed integral image
    //if(image.type() == CV_32S)
    //  sum = image;
    //else

    integral( grayImage, sum, CV_32S);  //计算积分图像

    //Remove keypoints very close to the border
    KeyPointsFilter::runByImageBorder(keypoints, image.size(), PATCH_SIZE/2 + KERNEL_SIZE/2);//剔除落在边界外的角点

    descriptors = Mat::zeros((int)keypoints.size(), bytes_, CV_8U);
    test_fn_(sum, keypoints, descriptors);  //计算特征点描述子
}
```



关于对随机点平滑，不采用论文中高斯平滑，而是采用随机点邻域内积分和代替，同样可以降低噪声的影响：





```cpp
inline int smoothedSum(const Mat& sum, const KeyPoint& pt, int y, int x)
{
    static const int HALF_KERNEL = BriefDescriptorExtractor::KERNEL_SIZE / 2;

    int img_y = (int)(pt.pt.y + 0.5) + y;
    int img_x = (int)(pt.pt.x + 0.5) + x;
    return   sum.at<int>(img_y + HALF_KERNEL + 1, img_x + HALF_KERNEL + 1)
           - sum.at<int>(img_y + HALF_KERNEL + 1, img_x - HALF_KERNEL)
           - sum.at<int>(img_y - HALF_KERNEL, img_x + HALF_KERNEL + 1)
           + sum.at<int>(img_y - HALF_KERNEL, img_x - HALF_KERNEL);
}
```
描述子向量的形成（以长度为16字节×8=128为例）：



数组des每一个元素占用一个字节，源码位置：...\modules\features2d\src\generated_16.i



```cpp
// Code generated with '$ scripts/generate_code.py src/test_pairs.txt 16'
#define SMOOTHED(y,x) smoothedSum(sum, pt, y, x)
    desc[0] = (uchar)(((SMOOTHED(-2, -1) < SMOOTHED(7, -1)) << 7) + ((SMOOTHED(-14, -1) < SMOOTHED(-3, 3)) << 6) + ((SMOOTHED(1, -2) < SMOOTHED(11, 2)) << 5) + ((SMOOTHED(1, 6) < SMOOTHED(-10, -7)) << 4) + ((SMOOTHED(13, 2) < SMOOTHED(-1, 0)) << 3) + ((SMOOTHED(-14, 5) < SMOOTHED(5, -3)) << 2) + ((SMOOTHED(-2, 8) < SMOOTHED(2, 4)) << 1) + ((SMOOTHED(-11, 8) < SMOOTHED(-15, 5)) << 0));
    desc[1] = (uchar)(((SMOOTHED(-6, -23) < SMOOTHED(8, -9)) << 7) + ((SMOOTHED(-12, 6) < SMOOTHED(-10, 8)) << 6) + ((SMOOTHED(-3, -1) < SMOOTHED(8, 1)) << 5) + ((SMOOTHED(3, 6) < SMOOTHED(5, 6)) << 4) + ((SMOOTHED(-7, -6) < SMOOTHED(5, -5)) << 3) + ((SMOOTHED(22, -2) < SMOOTHED(-11, -8)) << 2) + ((SMOOTHED(14, 7) < SMOOTHED(8, 5)) << 1) + ((SMOOTHED(-1, 14) < SMOOTHED(-5, -14)) << 0));
    desc[2] = (uchar)(((SMOOTHED(-14, 9) < SMOOTHED(2, 0)) << 7) + ((SMOOTHED(7, -3) < SMOOTHED(22, 6)) << 6) + ((SMOOTHED(-6, 6) < SMOOTHED(-8, -5)) << 5) + ((SMOOTHED(-5, 9) < SMOOTHED(7, -1)) << 4) + ((SMOOTHED(-3, -7) < SMOOTHED(-10, -18)) << 3) + ((SMOOTHED(4, -5) < SMOOTHED(0, 11)) << 2) + ((SMOOTHED(2, 3) < SMOOTHED(9, 10)) << 1) + ((SMOOTHED(-10, 3) < SMOOTHED(4, 9)) << 0));
    desc[3] = (uchar)(((SMOOTHED(0, 12) < SMOOTHED(-3, 19)) << 7) + ((SMOOTHED(1, 15) < SMOOTHED(-11, -5)) << 6) + ((SMOOTHED(14, -1) < SMOOTHED(7, 8)) << 5) + ((SMOOTHED(7, -23) < SMOOTHED(-5, 5)) << 4) + ((SMOOTHED(0, -6) < SMOOTHED(-10, 17)) << 3) + ((SMOOTHED(13, -4) < SMOOTHED(-3, -4)) << 2) + ((SMOOTHED(-12, 1) < SMOOTHED(-12, 2)) << 1) + ((SMOOTHED(0, 8) < SMOOTHED(3, 22)) << 0));
    desc[4] = (uchar)(((SMOOTHED(-13, 13) < SMOOTHED(3, -1)) << 7) + ((SMOOTHED(-16, 17) < SMOOTHED(6, 10)) << 6) + ((SMOOTHED(7, 15) < SMOOTHED(-5, 0)) << 5) + ((SMOOTHED(2, -12) < SMOOTHED(19, -2)) << 4) + ((SMOOTHED(3, -6) < SMOOTHED(-4, -15)) << 3) + ((SMOOTHED(8, 3) < SMOOTHED(0, 14)) << 2) + ((SMOOTHED(4, -11) < SMOOTHED(5, 5)) << 1) + ((SMOOTHED(11, -7) < SMOOTHED(7, 1)) << 0));
    desc[5] = (uchar)(((SMOOTHED(6, 12) < SMOOTHED(21, 3)) << 7) + ((SMOOTHED(-3, 2) < SMOOTHED(14, 1)) << 6) + ((SMOOTHED(5, 1) < SMOOTHED(-5, 11)) << 5) + ((SMOOTHED(3, -17) < SMOOTHED(-6, 2)) << 4) + ((SMOOTHED(6, 8) < SMOOTHED(5, -10)) << 3) + ((SMOOTHED(-14, -2) < SMOOTHED(0, 4)) << 2) + ((SMOOTHED(5, -7) < SMOOTHED(-6, 5)) << 1) + ((SMOOTHED(10, 4) < SMOOTHED(4, -7)) << 0));
    desc[6] = (uchar)(((SMOOTHED(22, 0) < SMOOTHED(7, -18)) << 7) + ((SMOOTHED(-1, -3) < SMOOTHED(0, 18)) << 6) + ((SMOOTHED(-4, 22) < SMOOTHED(-5, 3)) << 5) + ((SMOOTHED(1, -7) < SMOOTHED(2, -3)) << 4) + ((SMOOTHED(19, -20) < SMOOTHED(17, -2)) << 3) + ((SMOOTHED(3, -10) < SMOOTHED(-8, 24)) << 2) + ((SMOOTHED(-5, -14) < SMOOTHED(7, 5)) << 1) + ((SMOOTHED(-2, 12) < SMOOTHED(-4, -15)) << 0));
    desc[7] = (uchar)(((SMOOTHED(4, 12) < SMOOTHED(0, -19)) << 7) + ((SMOOTHED(20, 13) < SMOOTHED(3, 5)) << 6) + ((SMOOTHED(-8, -12) < SMOOTHED(5, 0)) << 5) + ((SMOOTHED(-5, 6) < SMOOTHED(-7, -11)) << 4) + ((SMOOTHED(6, -11) < SMOOTHED(-3, -22)) << 3) + ((SMOOTHED(15, 4) < SMOOTHED(10, 1)) << 2) + ((SMOOTHED(-7, -4) < SMOOTHED(15, -6)) << 1) + ((SMOOTHED(5, 10) < SMOOTHED(0, 24)) << 0));
    desc[8] = (uchar)(((SMOOTHED(3, 6) < SMOOTHED(22, -2)) << 7) + ((SMOOTHED(-13, 14) < SMOOTHED(4, -4)) << 6) + ((SMOOTHED(-13, 8) < SMOOTHED(-18, -22)) << 5) + ((SMOOTHED(-1, -1) < SMOOTHED(-7, 3)) << 4) + ((SMOOTHED(-19, -12) < SMOOTHED(4, 3)) << 3) + ((SMOOTHED(8, 10) < SMOOTHED(13, -2)) << 2) + ((SMOOTHED(-6, -1) < SMOOTHED(-6, -5)) << 1) + ((SMOOTHED(2, -21) < SMOOTHED(-3, 2)) << 0));
    desc[9] = (uchar)(((SMOOTHED(4, -7) < SMOOTHED(0, 16)) << 7) + ((SMOOTHED(-6, -5) < SMOOTHED(-12, -1)) << 6) + ((SMOOTHED(1, -1) < SMOOTHED(9, 18)) << 5) + ((SMOOTHED(-7, 10) < SMOOTHED(-11, 6)) << 4) + ((SMOOTHED(4, 3) < SMOOTHED(19, -7)) << 3) + ((SMOOTHED(-18, 5) < SMOOTHED(-4, 5)) << 2) + ((SMOOTHED(4, 0) < SMOOTHED(-20, 4)) << 1) + ((SMOOTHED(7, -11) < SMOOTHED(18, 12)) << 0));
    desc[10] = (uchar)(((SMOOTHED(-20, 17) < SMOOTHED(-18, 7)) << 7) + ((SMOOTHED(2, 15) < SMOOTHED(19, -11)) << 6) + ((SMOOTHED(-18, 6) < SMOOTHED(-7, 3)) << 5) + ((SMOOTHED(-4, 1) < SMOOTHED(-14, 13)) << 4) + ((SMOOTHED(17, 3) < SMOOTHED(2, -8)) << 3) + ((SMOOTHED(-7, 2) < SMOOTHED(1, 6)) << 2) + ((SMOOTHED(17, -9) < SMOOTHED(-2, 8)) << 1) + ((SMOOTHED(-8, -6) < SMOOTHED(-1, 12)) << 0));
    desc[11] = (uchar)(((SMOOTHED(-2, 4) < SMOOTHED(-1, 6)) << 7) + ((SMOOTHED(-2, 7) < SMOOTHED(6, 8)) << 6) + ((SMOOTHED(-8, -1) < SMOOTHED(-7, -9)) << 5) + ((SMOOTHED(8, -9) < SMOOTHED(15, 0)) << 4) + ((SMOOTHED(0, 22) < SMOOTHED(-4, -15)) << 3) + ((SMOOTHED(-14, -1) < SMOOTHED(3, -2)) << 2) + ((SMOOTHED(-7, -4) < SMOOTHED(17, -7)) << 1) + ((SMOOTHED(-8, -2) < SMOOTHED(9, -4)) << 0));
    desc[12] = (uchar)(((SMOOTHED(5, -7) < SMOOTHED(7, 7)) << 7) + ((SMOOTHED(-5, 13) < SMOOTHED(-8, 11)) << 6) + ((SMOOTHED(11, -4) < SMOOTHED(0, 8)) << 5) + ((SMOOTHED(5, -11) < SMOOTHED(-9, -6)) << 4) + ((SMOOTHED(2, -6) < SMOOTHED(3, -20)) << 3) + ((SMOOTHED(-6, 2) < SMOOTHED(6, 10)) << 2) + ((SMOOTHED(-6, -6) < SMOOTHED(-15, 7)) << 1) + ((SMOOTHED(-6, -3) < SMOOTHED(2, 1)) << 0));
    desc[13] = (uchar)(((SMOOTHED(11, 0) < SMOOTHED(-3, 2)) << 7) + ((SMOOTHED(7, -12) < SMOOTHED(14, 5)) << 6) + ((SMOOTHED(0, -7) < SMOOTHED(-1, -1)) << 5) + ((SMOOTHED(-16, 0) < SMOOTHED(6, 8)) << 4) + ((SMOOTHED(22, 11) < SMOOTHED(0, -3)) << 3) + ((SMOOTHED(19, 0) < SMOOTHED(5, -17)) << 2) + ((SMOOTHED(-23, -14) < SMOOTHED(-13, -19)) << 1) + ((SMOOTHED(-8, 10) < SMOOTHED(-11, -2)) << 0));
    desc[14] = (uchar)(((SMOOTHED(-11, 6) < SMOOTHED(-10, 13)) << 7) + ((SMOOTHED(1, -7) < SMOOTHED(14, 0)) << 6) + ((SMOOTHED(-12, 1) < SMOOTHED(-5, -5)) << 5) + ((SMOOTHED(4, 7) < SMOOTHED(8, -1)) << 4) + ((SMOOTHED(-1, -5) < SMOOTHED(15, 2)) << 3) + ((SMOOTHED(-3, -1) < SMOOTHED(7, -10)) << 2) + ((SMOOTHED(3, -6) < SMOOTHED(10, -18)) << 1) + ((SMOOTHED(-7, -13) < SMOOTHED(-13, 10)) << 0));
    desc[15] = (uchar)(((SMOOTHED(1, -1) < SMOOTHED(13, -10)) << 7) + ((SMOOTHED(-19, 14) < SMOOTHED(8, -14)) << 6) + ((SMOOTHED(-4, -13) < SMOOTHED(7, 1)) << 5) + ((SMOOTHED(1, -2) < SMOOTHED(12, -7)) << 4) + ((SMOOTHED(3, -5) < SMOOTHED(1, -5)) << 3) + ((SMOOTHED(-2, -2) < SMOOTHED(8, -10)) << 2) + ((SMOOTHED(2, 14) < SMOOTHED(8, 7)) << 1) + ((SMOOTHED(3, 9) < SMOOTHED(8, 2)) << 0));
#undef SMOOTHED
```

参考文章：

         Michael Calonder et.BRIEF:Binary Robust Independent Elementary Features

         http://www.cnblogs.com/ronny/p/4081362.html?utm_source=tuicool



