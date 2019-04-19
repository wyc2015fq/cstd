# 图像处理（一）：灰度化及RGB2HSV - sinat_31425585的博客 - CSDN博客
2017年11月13日 13:46:43[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：1315
图像灰度化，即将图像从RBG三通道彩色图像转换成灰度图像，现有的方法主要有三种：
1、取最大值：即从RBG三个彩色通道中，去亮度的最大值
2、取平均值：即取RBG三个彩色通道亮度的平均值
3、取加权平均值：RBG三通道占有比例分别为0.11,0.59,0.3
下面是实现代码：
```cpp
#include <opencv2/opencv.hpp>
using namespace cv;
int main()
{
	Mat src = imread("1.jpg");
	//1、 max
	Mat gray = Mat::zeros(src.rows, src.cols, CV_8UC1);
	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++)
		{
			gray.at<uchar>(i, j) = fmax(src.at<Vec3b>(i, j)[0], 
			fmax(src.at<Vec3b>(i, j)[1], src.at<Vec3b>(i, j)[2]));
		}
	//2、 average
	Mat gray1 = Mat::zeros(src.rows, src.cols, CV_8UC1);
	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++)
		{
			gray1.at<uchar>(i, j) = (src.at<Vec3b>(i, j)[0] +
				src.at<Vec3b>(i, j)[1] +  src.at<Vec3b>(i, j)[2]) / 3;
		}
	// 3、weighted mean
	Mat gray2 = Mat::zeros(src.rows, src.cols, CV_8UC1);
	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++)
		{
			gray2.at<uchar>(i, j) = 0.3 * src.at<Vec3b>(i, j)[0] + 
				0.59 * src.at<Vec3b>(i, j)[1] + 0.11 * src.at<Vec3b>(i, j)[2];
		}
	namedWindow("src", CV_WINDOW_AUTOSIZE);
	namedWindow("max", CV_WINDOW_AUTOSIZE);
	namedWindow("ave", CV_WINDOW_AUTOSIZE);
	namedWindow("wei", CV_WINDOW_AUTOSIZE);
	imshow("src", src);
	imshow("max", gray);
	imshow("ave", gray1);
	imshow("wei", gray2);
	waitKey();
	return 0;
}
```
效果如下：
![](https://img-blog.csdn.net/20171113134440263?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzE0MjU1ODU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
打完收工！
RGB颜色空间转HSV颜色空间，具体原理可以参考博客：[点击打开链接](http://blog.csdn.net/viewcode/article/details/8203728)，代码如下：
```cpp
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
using namespace cv;
using namespace std;
int main()
{
	Mat src = imread("1.jpg");
	vector<Mat> channels;
	// 分离出三个通道
	split(src, channels);
	Mat channel_b = channels.at(0);
	Mat channel_g = channels.at(1);
	Mat channel_r = channels.at(2);
	int i, j;
	Mat channel_h = Mat::zeros(src.rows, src.cols, CV_32FC1);
	Mat channel_s = Mat::zeros(src.rows, src.cols, CV_32FC1);
	Mat channel_v = Mat::zeros(src.rows, src.cols, CV_32FC1);
	float min_v = 0.0, max_v = 0.0;
	for (i = 0; i < src.rows; i++)
		for (j = 0; j < src.cols; j++)
		{
			min_v = channel_b.at<uchar>(i, j);
			max_v = channel_b.at<uchar>(i, j);
			//最小值
            if (min_v > channel_g.at<uchar>(i, j))
			{
				min_v = channel_g.at<uchar>(i, j);
			}
			if (min_v > channel_r.at<uchar>(i, j))
			{
				min_v = channel_r.at<uchar>(i, j);
			}
			// 最大值
			if (max_v < channel_g.at<uchar>(i, j))
			{
				max_v = channel_g.at<uchar>(i, j);
			}
			if (max_v < channel_r.at<uchar>(i, j))
			{
				max_v = channel_r.at<uchar>(i, j);
			}
			channel_v.at<float>(i, j) = max_v;
			if (abs(max_v) > 0)
			{
				channel_s.at<float>(i, j) = 1.0 * (max_v - min_v) / max_v;
			}
			else
			{
				channel_s.at<float>(i, j) = 0;
			}
			if (abs(max_v - channel_b.at<uchar>(i, j)) < 0.1)
			{
				channel_h.at<float>(i, j) = 60 * (channel_g.at<uchar>(i, j) - channel_b.at<uchar>(i, j)) / (max_v - min_v);
				if (channel_h.at<float>(i, j) < 0)
				{
					channel_h.at<float>(i, j) += 360;
				}
			}
			else if (abs(max_v - channel_g.at<uchar>(i, j)) < 0.1)
			{
				channel_h.at<float>(i, j) = 120 + 60 * (channel_b.at<uchar>(i, j) - channel_r.at<uchar>(i, j)) / (max_v - min_v);
				if (channel_h.at<float>(i, j) < 0)
				{
					channel_h.at<float>(i, j) += 360;
				}
			}
			else
			{
				channel_h.at<float>(i, j) = 240 + 60 * (channel_r.at<uchar>(i, j) - channel_g.at<uchar>(i, j)) / (max_v - min_v);	
				if (channel_h.at<float>(i, j) < 0)
				{
					channel_h.at<float>(i, j) += 360;
				}
			}
		}
	imshow("H", channel_h);
	imshow("S", channel_s);
	imshow("V", channel_v);
	waitKey();
	return 0;
}
```
效果如下：
![](https://img-blog.csdn.net/20171209112648620?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzE0MjU1ODU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
其中，H分量对应色相（RBG三个通道去最大值），S分量对应饱和度（取值范围0-1），V分量对应明度。
注意一下，下面这个点可能非常有用：
**H是色彩**
**S是深浅， S = 0时，只有灰度**
**V是明暗，表示色彩的明亮程度，但与光强无直接联系**
参考资料：
[http://lib.csdn.net/article/opencv/23601](http://lib.csdn.net/article/opencv/23601)
