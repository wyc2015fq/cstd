
# 运动检测（前景检测）之（一）ViBe - zouxy09的专栏 - CSDN博客


2013年07月29日 21:26:12[zouxy09](https://me.csdn.net/zouxy09)阅读数：51984个人分类：[OpenCV																](https://blog.csdn.net/zouxy09/article/category/1218759)[图像处理																](https://blog.csdn.net/zouxy09/article/category/1218762)[计算机视觉																](https://blog.csdn.net/zouxy09/article/category/1218765)[
							](https://blog.csdn.net/zouxy09/article/category/1218762)
[
																								](https://blog.csdn.net/zouxy09/article/category/1218759)


**运动检测（前景检测）之（一）ViBe**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
因为监控发展的需求，目前前景检测的研究还是很多的，也出现了很多新的方法和思路。个人了解的大概概括为以下一些：
帧差、背景减除（GMM、CodeBook、 SOBS、 SACON、 VIBE、 W4、多帧平均……）、光流（稀疏光流、稠密光流）、运动竞争（Motion Competition）、运动模版（运动历史图像）、时间熵……等等。如果加上他们的改进版，那就是很大的一个家族了。
对于上一些方法的一点简单的对比分析可以参考下：
http://www.cnblogs.com/ronny/archive/2012/04/12/2444053.html
至于哪个最好，看使用环境吧，各有千秋，有一些适用的情况更多，有一些在某些情况下表现更好。这些都需要针对自己的使用情况作测试确定的。呵呵。
推荐一个牛逼的库：http://code.google.com/p/bgslibrary/里面包含了各种背景减除的方法，可以让自己少做很多力气活。
还有王先荣博客上存在不少的分析：
http://www.cnblogs.com/xrwang/archive/2010/02/21/ForegroundDetection.html
下面的博客上转载王先荣的上面几篇，然后加上自己分析了两篇：
http://blog.csdn.net/stellar0
本文主要关注其中的一种背景减除方法：ViBe。stellar0的博客上对ViBe进行了分析，我这里就不再啰嗦了，具体的理论可以参考：
http://www2.ulg.ac.be/telecom/research/vibe/
http://blog.csdn.net/stellar0/article/details/8777283
http://blog.csdn.net/yongshengsilingsa/article/details/6659859
http://www2.ulg.ac.be/telecom/research/vibe/download.html
http://www.cvchina.info/2011/12/25/vibe/
《ViBe: A universal background subtraction algorithm for video sequences》
《ViBe: a powerful technique for background detection and subtraction in video sequences》
ViBe是一种像素级视频背景建模或前景检测的算法，效果优于所熟知的几种算法，对硬件内存占用也少，很简单。我之前根据stellar0的代码（在这里，非常感谢stellar0）改写成一个Mat格式的代码了，现在摆上来和大家交流，具体如下：（在VS2010+OpenCV2.4.2中测试通过）
**ViBe.h**
```cpp
#pragma once
#include <iostream>
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;
#define NUM_SAMPLES 20		//每个像素点的样本个数
#define MIN_MATCHES 2		//#min指数
#define RADIUS 20		//Sqthere半径
#define SUBSAMPLE_FACTOR 16	//子采样概率

class ViBe_BGS
{
public:
	ViBe_BGS(void);
	~ViBe_BGS(void);
	void init(const Mat _image);   //初始化
	void processFirstFrame(const Mat _image);
	void testAndUpdate(const Mat _image);  //更新
	Mat getMask(void){return m_mask;};
private:
	Mat m_samples[NUM_SAMPLES];
	Mat m_foregroundMatchCount;
	Mat m_mask;
};
```

**ViBe.cpp**
```cpp
#include <opencv2/opencv.hpp>
#include <iostream>
#include "ViBe.h"
using namespace std;
using namespace cv;
int c_xoff[9] = {-1,  0,  1, -1, 1, -1, 0, 1, 0};  //x的邻居点
int c_yoff[9] = {-1,  0,  1, -1, 1, -1, 0, 1, 0};  //y的邻居点
ViBe_BGS::ViBe_BGS(void)
{
}
ViBe_BGS::~ViBe_BGS(void)
{
}
/**************** Assign space and init ***************************/
void ViBe_BGS::init(const Mat _image)
{
     for(int i = 0; i < NUM_SAMPLES; i++)
     {
		 m_samples[i] = Mat::zeros(_image.size(), CV_8UC1);
     }
	 m_mask = Mat::zeros(_image.size(),CV_8UC1);
	 m_foregroundMatchCount = Mat::zeros(_image.size(),CV_8UC1);
}
/**************** Init model from first frame ********************/
void ViBe_BGS::processFirstFrame(const Mat _image)
{
	RNG rng;
	int row, col;
	for(int i = 0; i < _image.rows; i++)
	{
		for(int j = 0; j < _image.cols; j++)
		{
             for(int k = 0 ; k < NUM_SAMPLES; k++)
             {
				 // Random pick up NUM_SAMPLES pixel in neighbourhood to construct the model
				 int random = rng.uniform(0, 9);
				 row = i + c_yoff[random];
				 if (row < 0) 
					 row = 0;
				 if (row >= _image.rows)
					 row = _image.rows - 1;
				 col = j + c_xoff[random];
				 if (col < 0) 
					 col = 0;
				 if (col >= _image.cols)
					 col = _image.cols - 1;
				 m_samples[k].at<uchar>(i, j) = _image.at<uchar>(row, col);
			 }
		}
	}
}
/**************** Test a new frame and update model ********************/
void ViBe_BGS::testAndUpdate(const Mat _image)
{
	RNG rng;
	for(int i = 0; i < _image.rows; i++)
	{
		for(int j = 0; j < _image.cols; j++)
		{
			int matches(0), count(0);
			float dist;
			while(matches < MIN_MATCHES && count < NUM_SAMPLES)
			{
				dist = abs(m_samples[count].at<uchar>(i, j) - _image.at<uchar>(i, j));
				if (dist < RADIUS)
					matches++;
				count++;
			}
			if (matches >= MIN_MATCHES)
			{
				// It is a background pixel
				m_foregroundMatchCount.at<uchar>(i, j) = 0;
				// Set background pixel to 0
				m_mask.at<uchar>(i, j) = 0;
				// 如果一个像素是背景点，那么它有 1 / defaultSubsamplingFactor 的概率去更新自己的模型样本值
				int random = rng.uniform(0, SUBSAMPLE_FACTOR);
				if (random == 0)
				{
					random = rng.uniform(0, NUM_SAMPLES);
					m_samples[random].at<uchar>(i, j) = _image.at<uchar>(i, j);
				}
				// 同时也有 1 / defaultSubsamplingFactor 的概率去更新它的邻居点的模型样本值
				random = rng.uniform(0, SUBSAMPLE_FACTOR);
				if (random == 0)
				{
					int row, col;
					random = rng.uniform(0, 9);
					row = i + c_yoff[random];
					if (row < 0) 
						row = 0;
					if (row >= _image.rows)
						row = _image.rows - 1;
					random = rng.uniform(0, 9);
					col = j + c_xoff[random];
					if (col < 0) 
						col = 0;
					if (col >= _image.cols)
						col = _image.cols - 1;
					random = rng.uniform(0, NUM_SAMPLES);
					m_samples[random].at<uchar>(row, col) = _image.at<uchar>(i, j);
				}
			}
			else
			{
				// It is a foreground pixel
				m_foregroundMatchCount.at<uchar>(i, j)++;
				// Set background pixel to 255
				m_mask.at<uchar>(i, j) = 255;
				//如果某个像素点连续N次被检测为前景，则认为一块静止区域被误判为运动，将其更新为背景点
				if (m_foregroundMatchCount.at<uchar>(i, j) > 50)
				{
					int random = rng.uniform(0, SUBSAMPLE_FACTOR);
					if (random == 0)
					{
						random = rng.uniform(0, NUM_SAMPLES);
						m_samples[random].at<uchar>(i, j) = _image.at<uchar>(i, j);
					}
				}
			}
		}
	}
}
```

**Main.cpp**
```cpp
// This is based on 
// "VIBE: A POWERFUL RANDOM TECHNIQUE TO ESTIMATE THE BACKGROUND IN VIDEO SEQUENCES"
// by Olivier Barnich and Marc Van Droogenbroeck
// Author : zouxy
// Date   : 2013-4-13
// HomePage : http://blog.csdn.net/zouxy09
// Email  : zouxy09@qq.com
#include "opencv2/opencv.hpp"
#include "ViBe.h"
#include <iostream>
#include <cstdio>
using namespace cv;
using namespace std;
int main(int argc, char* argv[])
{
	Mat frame, gray, mask;
	VideoCapture capture;
	capture.open("video.avi");
	if (!capture.isOpened())
	{
		cout<<"No camera or video input!\n"<<endl;
		return -1;
	}
	ViBe_BGS Vibe_Bgs;
	int count = 0;
	while (1)
	{
		count++;
		capture >> frame;
		if (frame.empty())
			break;
		cvtColor(frame, gray, CV_RGB2GRAY);
	
		if (count == 1)
		{
			Vibe_Bgs.init(gray);
			Vibe_Bgs.processFirstFrame(gray);
			cout<<" Training GMM complete!"<<endl;
		}
		else
		{
			Vibe_Bgs.testAndUpdate(gray);
			mask = Vibe_Bgs.getMask();
			morphologyEx(mask, mask, MORPH_OPEN, Mat());
			imshow("mask", mask);
		}
		imshow("input", frame);	
		if ( cvWaitKey(10) == 'q' )
			break;
	}
	return 0;
}
```


