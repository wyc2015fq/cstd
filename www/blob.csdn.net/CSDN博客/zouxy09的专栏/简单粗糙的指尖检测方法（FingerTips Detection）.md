
# 简单粗糙的指尖检测方法（FingerTips Detection） - zouxy09的专栏 - CSDN博客


2013年03月23日 23:20:23[zouxy09](https://me.csdn.net/zouxy09)阅读数：16594


**简单粗糙的指尖检测方法（FingerTips Detection）**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
在人机交互领域，如果可以比较好的检测指尖，对于交互的丰富度、灵活性来说是有很大提升的。目前指尖检测的方法也很多，我这里稍微尝试了下简单了两种。这两种方法都借助了手的几何特征，简单但比较粗糙，鲁棒性不够。
**方法一：重心距离法**
见下图，红色点是手的重心，那么手的边缘的所有点与重心点的距离按顺时针方向或者逆时针方向遍历，就会出现五个峰值，分别是五个手指，这样我们就可以简单找到了。如果你是只伸出一两个手指，那么就只有一两个峰值了。
![](https://img-my.csdn.net/uploads/201303/23/1364052132_2012.jpg)
简单的代码如下：
1、对图像做高斯模糊；
2、肤色分割（背景不要有类肤色，如果有，就需要加其他信息来排除干扰）；
3、找到手轮廓；
4、计算矩，即重心；
5、寻找指尖。

```cpp
// Simple FingerTips Detection
// Author : Zouxy
// Date   : 2013-3-23
// HomePage : http://blog.csdn.net/zouxy09
// Email  : zouxy09@qq.com
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;
void skinExtract(const Mat &frame, Mat &skinArea);
int main(int argc, char* argv[])
{
	Mat frame, skinArea;
	VideoCapture capture;
	capture.open(0);
	if (!capture.isOpened())
	{
		cout<<"No camera!\n"<<endl;
		return -1;
	}
	while (1)
	{
		capture >> frame;
		//Mat frame = imread("fingertips(1).jpg");
		if (frame.empty())
			break;
		skinArea.create(frame.rows, frame.cols, CV_8UC1);
		skinExtract(frame, skinArea);
		Mat show_img;
		frame.copyTo(show_img, skinArea);
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		//寻找轮廓
		findContours(skinArea, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
		// 找到最大的轮廓
		int index;
		double area, maxArea(0);
		for (int i=0; i < contours.size(); i++)
		{
			area = contourArea(Mat(contours[i]));
			if (area > maxArea)
			{
				maxArea = area;
				index = i;
			}			
		}
		//drawContours(frame, contours, index, Scalar(0, 0, 255), 2, 8, hierarchy );
		
		Moments moment = moments(skinArea, true);
		Point center(moment.m10/moment.m00, moment.m01/moment.m00);
		circle(show_img, center, 8 ,Scalar(0, 0, 255), CV_FILLED);
		// 寻找指尖
		vector<Point> couPoint = contours[index];
		vector<Point> fingerTips;
		Point tmp;
		int max(0), count(0), notice(0);
		for (int i = 0; i < couPoint.size(); i++)
		{
			tmp = couPoint[i];
			int dist = (tmp.x - center.x) * (tmp.x - center.x) + (tmp.y - center.y) * (tmp.y - center.y);
			if (dist > max)
			{
				max = dist;
				notice = i;
			}
			// 计算最大值保持的点数，如果大于40（这个值需要设置，本来想根据max值来设置，
			// 但是不成功，不知道为何），那么就认为这个是指尖
			if (dist != max)
			{
				count++;
				if (count > 40)
				{
					count = 0;
					max = 0;
					bool flag = false;
					// 低于手心的点不算
					if (center.y < couPoint[notice].y )
						continue;
					// 离得太近的不算
					for (int j = 0; j < fingerTips.size(); j++)
					{
						if (abs(couPoint[notice].x - fingerTips[j].x) < 20)
						{
							flag = true;
							break;
						}
					}
					if (flag) continue;
					fingerTips.push_back(couPoint[notice]);
					circle(show_img, couPoint[notice], 6 ,Scalar(0, 255, 0), CV_FILLED);
					line(show_img, center, couPoint[notice], Scalar(255, 0, 0), 2);				
				}
			}
		}
		imshow("show_img", show_img);
		if ( cvWaitKey(20) == 'q' )
			break;
	}
	return 0;
}
//肤色提取，skinArea为二值化肤色图像
void skinExtract(const Mat &frame, Mat &skinArea)
{
	Mat YCbCr;
	vector<Mat> planes;
	//转换为YCrCb颜色空间
	cvtColor(frame, YCbCr, CV_RGB2YCrCb);
	//将多通道图像分离为多个单通道图像
	split(YCbCr, planes); 
	//运用迭代器访问矩阵元素
	MatIterator_<uchar> it_Cb = planes[1].begin<uchar>(),
						it_Cb_end = planes[1].end<uchar>();
	MatIterator_<uchar> it_Cr = planes[2].begin<uchar>();
	MatIterator_<uchar> it_skin = skinArea.begin<uchar>();
	//人的皮肤颜色在YCbCr色度空间的分布范围:100<=Cb<=127, 138<=Cr<=170
	for( ; it_Cb != it_Cb_end; ++it_Cr, ++it_Cb, ++it_skin)
	{
		if (138 <= *it_Cr &&  *it_Cr <= 170 && 100 <= *it_Cb &&  *it_Cb <= 127)
			*it_skin = 255;
		else
			*it_skin = 0;
	}
	//膨胀和腐蚀，膨胀可以填补凹洞（将裂缝桥接），腐蚀可以消除细的凸起（“斑点”噪声）
	dilate(skinArea, skinArea, Mat(5, 5, CV_8UC1), Point(-1, -1));
	erode(skinArea, skinArea, Mat(5, 5, CV_8UC1), Point(-1, -1));
}
```

**方法二：曲率分析法**
见下图，可以看到，在指尖和两指之间的凹槽的曲率是最大的。假设手的轮廓是通过一系列点{Pi}来保存的，那么我们可以通过[Pi, Pi-k]和[Pi, Pi+k]两个向量的内积（点乘）来寻找高曲率的点，如下图的Φ和β的计算结果，因为他们的两个向量的夹角趋向于90度，也就是它们的内积趋向于0，所以内积结果越小，曲率越大，我们只需要设置一个阈值，就可以把手的指尖和两指之间的凹槽都可以找到。
那么如何分辨指尖和两指之间的凹槽呢？我们可以通过两个向量的叉乘来计算，我们想象下手处于3D空间中，那么就还有一个z方向，β中两个向量的叉乘是大于0的（叉乘方向垂直纸面朝向你，z轴正方向），而Φ处的叉乘是小于0的（叉乘方向垂直纸面远离你，z轴负方向）。
综上，通过在每个点上构造两个向量，比较他们的点乘和叉乘就可以确定指尖了。
![](https://img-my.csdn.net/uploads/201303/23/1364052160_1284.jpg)
简单的代码如下：
1、对图像做高斯模糊；
2、肤色分割（背景不要有类肤色，如果有，就需要加其他信息来排除干扰）；
3、找到手轮廓；
4、寻找指尖。

```cpp
// Simple FingerTips Detection Using Curvature Determination
// Author : Zouxy
// Date   : 2013-3-23
// HomePage : http://blog.csdn.net/zouxy09
// Email  : zouxy09@qq.com
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;
void skinExtract(const Mat &frame, Mat &skinArea);
int main(int argc, char* argv[])
{
	Mat frame, skinArea;
	VideoCapture capture;
	capture.open(0);
	if (!capture.isOpened())
	{
		cout<<"No camera!\n"<<endl;
		return -1;
	}
	while (1)
	{
		capture >> frame;
		//Mat frame = imread("fingertips(1).jpg");
		if (frame.empty())
			break;
		GaussianBlur(frame, frame, Size(3, 3), 0);
		skinArea.create(frame.rows, frame.cols, CV_8UC1);
		skinExtract(frame, skinArea);
		Mat show_img;
		frame.copyTo(show_img, skinArea);
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		//寻找轮廓
		findContours(skinArea, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
		// 找到最大的轮廓
		int index;
		double area, maxArea(0);
		for (int i=0; i < contours.size(); i++)
		{
			area = contourArea(Mat(contours[i]));
			if (area > maxArea)
			{
				maxArea = area;
				index = i;
			}			
		}
		//drawContours(frame, contours, index, Scalar(0, 0, 255), 2, 8, hierarchy );
		
		Moments moment = moments(skinArea, true);
		Point center(moment.m10/moment.m00, moment.m01/moment.m00);
		circle(show_img, center, 8 ,Scalar(0, 0, 255), CV_FILLED);
		// 寻找指尖
		vector<Point> couPoint = contours[index];
		int max(0), count(0), notice(0);
		vector<Point> fingerTips;
		Point p, q, r;
		for (int i = 5; (i < (couPoint.size() - 5)) && couPoint.size(); i++)
		{
			q = couPoint[i - 5];
			p = couPoint[i];
			r = couPoint[i + 5];
			int dot = (q.x - p.x ) * (r.x - p.x) + (q.y - p.y ) * (r.y - p.y);
			if (dot < 20 && dot > -20)
			{
				int cross = (q.x - p.x ) * (r.y - p.y) - (r.x - p.x ) * (q.y - p.y);
				if (cross > 0)
				{
					fingerTips.push_back(p);
					circle(show_img, p, 5 ,Scalar(255, 0, 0), CV_FILLED);
					line(show_img, center, p, Scalar(255, 0, 0), 2);	
				}
			}
		}
		imshow("show_img", show_img);
		if ( cvWaitKey(20) == 'q' )
			break;
	}
	return 0;
}
//肤色提取，skinArea为二值化肤色图像
void skinExtract(const Mat &frame, Mat &skinArea)
{
	Mat YCbCr;
	vector<Mat> planes;
	//转换为YCrCb颜色空间
	cvtColor(frame, YCbCr, CV_RGB2YCrCb);
	//将多通道图像分离为多个单通道图像
	split(YCbCr, planes); 
	//运用迭代器访问矩阵元素
	MatIterator_<uchar> it_Cb = planes[1].begin<uchar>(),
						it_Cb_end = planes[1].end<uchar>();
	MatIterator_<uchar> it_Cr = planes[2].begin<uchar>();
	MatIterator_<uchar> it_skin = skinArea.begin<uchar>();
	//人的皮肤颜色在YCbCr色度空间的分布范围:100<=Cb<=127, 138<=Cr<=170
	for( ; it_Cb != it_Cb_end; ++it_Cr, ++it_Cb, ++it_skin)
	{
		if (138 <= *it_Cr &&  *it_Cr <= 170 && 100 <= *it_Cb &&  *it_Cb <= 127)
			*it_skin = 255;
		else
			*it_skin = 0;
	}
	//膨胀和腐蚀，膨胀可以填补凹洞（将裂缝桥接），腐蚀可以消除细的凸起（“斑点”噪声）
	dilate(skinArea, skinArea, Mat(5, 5, CV_8UC1), Point(-1, -1));
	erode(skinArea, skinArea, Mat(5, 5, CV_8UC1), Point(-1, -1));
}
```
**效果：**
![](https://img-my.csdn.net/uploads/201303/23/1364052289_5587.jpg)![](https://img-my.csdn.net/uploads/201303/23/1364052304_5749.jpg)
![](https://img-my.csdn.net/uploads/201303/23/1364052317_2357.jpg)
如要深入，我们还可以参考以下资源：
finger-detection-and-gesture-recognition [Code]
Hand and Finger Detection using JavaCV[Project]
Hand and fingers detection[Code]


