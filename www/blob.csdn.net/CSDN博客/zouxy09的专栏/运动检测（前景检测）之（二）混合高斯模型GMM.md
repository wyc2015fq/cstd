
# 运动检测（前景检测）之（二）混合高斯模型GMM - zouxy09的专栏 - CSDN博客


2013年07月29日 21:40:11[zouxy09](https://me.csdn.net/zouxy09)阅读数：30657个人分类：[图像处理																](https://blog.csdn.net/zouxy09/article/category/1218762)[计算机视觉																](https://blog.csdn.net/zouxy09/article/category/1218765)[OpenCV																](https://blog.csdn.net/zouxy09/article/category/1218759)[
							](https://blog.csdn.net/zouxy09/article/category/1218765)
[
																								](https://blog.csdn.net/zouxy09/article/category/1218762)


**运动检测（前景检测）之（二）混合高斯模型GMM**
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
本文主要关注其中的一种背景减除方法：GMM。tornadomeet的博客上对ViBe进行了分析，我这里就不再啰嗦了，具体的理论分析可以参考：
http://www.cnblogs.com/tornadomeet/archive/2012/06/02/2531565.html
里面有了GMM的代码，并有了详细的注释。我之前根据这个代码（在这里，非常感谢tornadomeet）改写了一个Mat格式的版本，现在发上来和大家交流，具体如下：（在VS2010+OpenCV2.4.2中测试通过）。（当然了，OpenCV也已经提供了MOG的背景减除方法）
**MOG_BGS.h**
```cpp
#pragma once
#include <iostream>
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;
//定义gmm模型用到的变量
 #define GMM_MAX_COMPONT 6			//每个GMM最多的高斯模型个数
 #define GMM_LEARN_ALPHA 0.005  
 #define GMM_THRESHOD_SUMW 0.7
 #define TRAIN_FRAMES 60	// 对前 TRAIN_FRAMES 帧建模
class MOG_BGS
{
public:
	MOG_BGS(void);
	~MOG_BGS(void);
	void init(const Mat _image);
	void processFirstFrame(const Mat _image);
	void trainGMM(const Mat _image);
	void getFitNum(const Mat _image);
	void testGMM(const Mat _image);
	Mat getMask(void){return m_mask;};
 
private:
	Mat m_weight[GMM_MAX_COMPONT];  //权值
	Mat m_mean[GMM_MAX_COMPONT];    //均值
	Mat m_sigma[GMM_MAX_COMPONT];   //方差
	Mat m_mask;
	Mat m_fit_num;
};
```

**MOG_BGS.cpp**
```cpp
#include "MOG_BGS.h"
MOG_BGS::MOG_BGS(void)
{
}
MOG_BGS::~MOG_BGS(void)
{
}
// 全部初始化为0
void MOG_BGS::init(const Mat _image)
{
	/****initialization the three parameters ****/
     for(int i = 0; i < GMM_MAX_COMPONT; i++)
     {
		 m_weight[i] = Mat::zeros(_image.size(), CV_32FC1);
         m_mean[i] = Mat::zeros(_image.size(), CV_8UC1);
         m_sigma[i] = Mat::zeros(_image.size(), CV_32FC1);
     }
	 m_mask = Mat::zeros(_image.size(),CV_8UC1);
	 m_fit_num = Mat::ones(_image.size(),CV_8UC1);
}
//gmm第一帧初始化函数实现
//捕获到第一帧时对高斯分布进行初始化．主要包括对每个高斯分布的权值、期望和方差赋初值．
//其中第一个高斯分布的权值为1，期望为第一个像素数据．其余高斯分布权值为0，期望为0．
//每个高斯分布都被赋予适当的相等的初始方差 15
void MOG_BGS::processFirstFrame(const Mat _image)
{
	for(int i = 0; i < GMM_MAX_COMPONT; i++)
    {
		if (i == 0)
		{
			m_weight[i].setTo(1.0);
			_image.copyTo(m_mean[i]);
			m_sigma[i].setTo(15.0);
		}
		else
		{
			m_weight[i].setTo(0.0);
			m_mean[i].setTo(0);
			m_sigma[i].setTo(15.0);
		}
    }
}
 
// 通过新的帧来训练GMM
void MOG_BGS::trainGMM(const Mat _image)
{
	for(int i = 0; i < _image.rows; i++)
	{
		for(int j = 0; j < _image.cols; j++)
		{
             int num_fit = 0;
			 /**************************** Update parameters Start ******************************************/
             for(int k = 0 ; k < GMM_MAX_COMPONT; k++)
             {
				 int delm = abs(_image.at<uchar>(i, j) - m_mean[k].at<uchar>(i, j));
				 long dist = delm * delm;
				 // 判断是否匹配：采样值与高斯分布的均值的距离小于3倍方差（表示匹配）
                 if( dist < 3.0 * m_sigma[k].at<float>(i, j)) 
                 {
					 // 如果匹配
                     /****update the weight****/
                     m_weight[k].at<float>(i, j) += GMM_LEARN_ALPHA * (1 - m_weight[k].at<float>(i, j));
 
                     /****update the average****/
                     m_mean[k].at<uchar>(i, j) += (GMM_LEARN_ALPHA / m_weight[k].at<uchar>(i, j)) * delm;
 
                     /****update the variance****/
                     m_sigma[k].at<float>(i, j) += (GMM_LEARN_ALPHA / m_weight[k].at<float>(i, j)) * (dist - m_sigma[k].at<float>(i, j));
                 }
                 else
				 {
					// 如果不匹配。则该该高斯模型的权值变小
                     m_weight[k].at<float>(i, j) += GMM_LEARN_ALPHA * (0 - m_weight[k].at<float>(i, j));
                     num_fit++; // 不匹配的模型个数
                 }        
             }
 			 /**************************** Update parameters End ******************************************/	
		
			 /*********************** Sort Gaussian component by 'weight / sigma' Start ****************************/
             //对gmm各个高斯进行排序,从大到小排序,排序依据为 weight / sigma
             for(int kk = 0; kk < GMM_MAX_COMPONT; kk++)
             {
                 for(int rr=kk; rr< GMM_MAX_COMPONT; rr++)
                 {
                     if(m_weight[rr].at<float>(i, j)/m_sigma[rr].at<float>(i, j) > m_weight[kk].at<float>(i, j)/m_sigma[kk].at<float>(i, j))
                     {
                         //权值交换
                         float temp_weight = m_weight[rr].at<float>(i, j);
                         m_weight[rr].at<float>(i, j) = m_weight[kk].at<float>(i, j);
                         m_weight[kk].at<float>(i, j) = temp_weight;
 
                         //均值交换
                         uchar temp_mean = m_mean[rr].at<uchar>(i, j);
                         m_mean[rr].at<uchar>(i, j) = m_mean[kk].at<uchar>(i, j);
                         m_mean[kk].at<uchar>(i, j) = temp_mean;
 
                         //方差交换
                         float temp_sigma = m_sigma[rr].at<float>(i, j);
                         m_sigma[rr].at<float>(i, j) = m_sigma[kk].at<float>(i, j);
                         m_sigma[kk].at<float>(i, j) = temp_sigma;
                     }
                 }
             }
			 /*********************** Sort Gaussian model by 'weight / sigma' End ****************************/
 
			 /*********************** Create new Gaussian component Start ****************************/
             if(num_fit == GMM_MAX_COMPONT && 0 == m_weight[GMM_MAX_COMPONT - 1].at<float>(i, j))
             {
				 //if there is no exit component fit,then start a new component
				 //当有新值出现的时候，若目前分布个数小于M，新添一个分布，以新采样值作为均值，并赋予较大方差和较小权值
                  for(int k = 0 ; k < GMM_MAX_COMPONT; k++)
                 {
                     if(0 == m_weight[k].at<float>(i, j))
					 {
						 m_weight[k].at<float>(i, j) = GMM_LEARN_ALPHA;
						 m_mean[k].at<uchar>(i, j) = _image.at<uchar>(i, j);
						 m_sigma[k].at<float>(i, j) = 15.0;
                        
						 //normalization the weight,let they sum to 1
                         for(int q = 0; q < GMM_MAX_COMPONT && q != k; q++)
                         {
							//对其他的高斯模型的权值进行更新，保持权值和为1
                             /****update the other unfit's weight,u and sigma remain unchanged****/
                             m_weight[q].at<float>(i, j) *= (1 - GMM_LEARN_ALPHA);
                         }
                         break; //找到第一个权值不为0的即可
                      }                            
                  }
             }
             else if(num_fit == GMM_MAX_COMPONT && m_weight[GMM_MAX_COMPONT -1].at<float>(i, j) != 0)
             {
				 //如果GMM_MAX_COMPONT都曾经赋值过，则用新来的高斯代替权值最弱的高斯，权值不变，只更新均值和方差
                 m_mean[GMM_MAX_COMPONT-1].at<uchar>(i, j) = _image.at<uchar>(i, j);
                 m_sigma[GMM_MAX_COMPONT-1].at<float>(i, j) = 15.0;
             }
			 /*********************** Create new Gaussian component End ****************************/
         }
	}
}
 //对输入图像每个像素gmm选择合适的高斯分量个数
 //排序后最有可能是背景分布的排在最前面，较小可能的短暂的分布趋向于末端．我们将排序后的前fit_num个分布选为背景模型;
 //在排过序的分布中，累积概率超过GMM_THRESHOD_SUMW的前fit_num个分布被当作背景模型，剩余的其它分布被当作前景模型．
void MOG_BGS::getFitNum(const Mat _image)
{
	for(int i = 0; i < _image.rows; i++)
	{
		for(int j = 0; j < _image.cols; j++)
		{
			float sum_w = 0.0;	//重新赋值为0，给下一个像素做累积
			for(uchar k = 0; k < GMM_MAX_COMPONT; k++)
			{
				sum_w += m_weight[k].at<float>(i, j);
				if(sum_w >= GMM_THRESHOD_SUMW)	//如果这里THRESHOD_SUMW=0.6的话，那么得到的高斯数目都为1，因为每个像素都有一个权值接近1
                {
                     m_fit_num.at<uchar>(i, j) = k + 1;
                     break;
                }
			}
		}
	}
}
 //gmm测试函数的实现
void MOG_BGS::testGMM(const Mat _image)
{
	for(int i = 0; i < _image.rows; i++)
	{
		for(int j = 0; j < _image.cols; j++)
		{
			int k = 0;
			for( ; k < m_fit_num.at<uchar>(i, j); k++)
			{
				if(abs(_image.at<uchar>(i, j) - m_mean[k].at<uchar>(i, j)) < (uchar)( 2.5 * m_sigma[k].at<float>(i, j)))
				{
					m_mask.at<uchar>(i, j) = 0;
					break;
				}
			}
			if(k == m_fit_num.at<uchar>(i, j))
			{
				m_mask.at<uchar>(i, j) = 255;
			}
		}
	}
}
```

**Main.cpp**
```cpp
// This is based on the "An Improved Adaptive Background Mixture Model for
// Real-time Tracking with Shadow Detection" by P. KaewTraKulPong and R. Bowden
// Author : zouxy
// Date   : 2013-4-13
// HomePage : http://blog.csdn.net/zouxy09
// Email  : zouxy09@qq.com
#include "opencv2/opencv.hpp"
#include "MOG_BGS.h"
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
	MOG_BGS Mog_Bgs;
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
			Mog_Bgs.init(gray);
			Mog_Bgs.processFirstFrame(gray);
			cout<<" Using "<<TRAIN_FRAMES<<" frames to training GMM..."<<endl;
		}
		else if (count < TRAIN_FRAMES)
		{
			Mog_Bgs.trainGMM(gray);
		}
		else if (count == TRAIN_FRAMES)
		{
			Mog_Bgs.getFitNum(gray);
			cout<<" Training GMM complete!"<<endl;
		}
		else
		{
			Mog_Bgs.testGMM(gray);
			mask = Mog_Bgs.getMask();
			morphologyEx(mask, mask, MORPH_OPEN, Mat());
			erode(mask, mask, Mat(7, 7, CV_8UC1), Point(-1, -1));  // You can use Mat(5, 5, CV_8UC1) here for less distortion
			dilate(mask, mask, Mat(7, 7, CV_8UC1), Point(-1, -1));
			imshow("mask", mask);
		}
		imshow("input", frame);	
		if ( cvWaitKey(10) == 'q' )
			break;
	}
	return 0;
}
```


