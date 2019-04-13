
# 光流Optical Flow介绍与OpenCV实现 - zouxy09的专栏 - CSDN博客


2013年03月17日 15:53:00[zouxy09](https://me.csdn.net/zouxy09)阅读数：125383


**光流Optical Flow介绍与OpenCV实现**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
光流（optic flow）是什么呢？名字很专业，感觉很陌生，但本质上，我们是最熟悉不过的了。因为这种视觉现象我们每天都在经历。从本质上说，光流就是你在这个运动着的世界里感觉到的明显的视觉运动（呵呵，相对论，没有绝对的静止，也没有绝对的运动）。例如，当你坐在火车上，然后往窗外看。你可以看到树、地面、建筑等等，他们都在往后退。这个运动就是光流。而且，我们都会发现，他们的运动速度居然不一样？这就给我们提供了一个挺有意思的信息：通过不同目标的运动速度判断它们与我们的距离。一些比较远的目标，例如云、山，它们移动很慢，感觉就像静止一样。但一些离得比较近的物体，例如建筑和树，就比较快的往后退，然后离我们的距离越近，它们往后退的速度越快。一些非常近的物体，例如路面的标记啊，草地啊等等，快到好像在我们耳旁发出嗖嗖的声音。
光流除了提供远近外，还可以提供角度信息。与咱们的眼睛正对着的方向成90度方向运动的物体速度要比其他角度的快，当小到0度的时候，也就是物体朝着我们的方向直接撞过来，我们就是感受不到它的运动（光流）了，看起来好像是静止的。当它离我们越近，就越来越大（当然了，我们平时看到感觉还是有速度的，因为物体较大，它的边缘还是和我们人眼具有大于0的角度的）。
呵呵，说了那么多，好像还没进入比较官方的，研究性的定义。那就贴上一个吧。
光流的概念是Gibson在1950年首先提出来的。它是空间运动物体在观察成像平面上的像素运动的瞬时速度，是利用图像序列中像素在时间域上的变化以及相邻帧之间的相关性来找到上一帧跟当前帧之间存在的对应关系，从而计算出相邻帧之间物体的运动信息的一种方法。一般而言，光流是由于场景中前景目标本身的移动、相机的运动，或者两者的共同运动所产生的。
当人的眼睛观察运动物体时，物体的景象在人眼的视网膜上形成一系列连续变化的图像，这一系列连续变化的信息不断“流过”视网膜（即图像平面），好像一种光的“流”，故称之为光流（optical flow）。光流表达了图像的变化，由于它包含了目标运动的信息，因此可被观察者用来确定目标的运动情况。
研究光流场的目的就是为了从图片序列中近似得到不能直接得到的运动场。运动场，其实就是物体在三维真实世界中的运动；光流场，是运动场在二维图像平面上（人的眼睛或者摄像头）的投影。
那通俗的讲就是通过一个图片序列，把每张图像中每个像素的运动速度和运动方向找出来就是光流场。那怎么找呢？咱们直观理解肯定是：第t帧的时候A点的位置是(x1, y1)，那么我们在第t+1帧的时候再找到A点，假如它的位置是(x2,y2)，那么我们就可以确定A点的运动了：(ux, vy) = (x2, y2) - (x1,y1)。
那怎么知道第t+1帧的时候A点的位置呢？ 这就存在很多的光流计算方法了。
1981年，Horn和Schunck创造性地将二维速度场与灰度相联系，引入光流约束方程，得到光流计算的基本算法。人们基于不同的理论基础提出各种光流计算方法，算法性能各有不同。Barron等人对多种光流计算技术进行了总结，按照理论基础与数学方法的区别把它们分成四种：基于梯度的方法、基于匹配的方法、基于能量的方法、基于相位的方法。近年来神经动力学方法也颇受学者重视。
其他的咱们先不说了，回归应用吧（呵呵，太高深了，自己说不下去了）。OpenCV中实现了不少的光流算法。
**1）calcOpticalFlowPyrLK**
通过金字塔Lucas-Kanade 光流方法计算某些点集的光流（稀疏光流）。理解的话，可以参考这篇论文：”Pyramidal Implementation of the Lucas Kanade Feature TrackerDescription of the algorithm”
**2）calcOpticalFlowFarneback**
用Gunnar Farneback 的算法计算稠密光流（即图像上所有像素点的光流都计算出来）。它的相关论文是："Two-Frame Motion Estimation Based on PolynomialExpansion"
**3）CalcOpticalFlowBM**
通过块匹配的方法来计算光流。
**4）CalcOpticalFlowHS**
用Horn-Schunck 的算法计算稠密光流。相关论文好像是这篇：”Determining Optical Flow”
**5）calcOpticalFlowSF**
这一个是2012年欧洲视觉会议的一篇文章的实现："SimpleFlow: A Non-iterative, Sublinear Optical FlowAlgorithm"，工程网站是：[http://graphics.berkeley.edu/papers/Tao-SAN-2012-05/](http://graphics.berkeley.edu/papers/Tao-SAN-2012-05/)在OpenCV新版本中有引入。
稠密光流需要使用某种插值方法在比较容易跟踪的像素之间进行插值以解决那些运动不明确的像素，所以它的计算开销是相当大的。而对于稀疏光流来说，在他计算时需要在被跟踪之前指定一组点（容易跟踪的点，例如角点），因此在使用LK方法之前我们需要配合使用cvGoodFeatureToTrack()来寻找角点，然后利用金字塔LK光流算法，对运动进行跟踪。但个人感觉，对于少纹理的目标，例如人手，LK稀疏光流就比较容易跟丢。
至于他们的API的使用说明，我们直接参考OpenCV的官方手册就行：
http://www.opencv.org.cn/opencvdoc/2.3.2/html/modules/video/doc/motion_analysis_and_object_tracking.html\#calcopticalflowfarneback
IJCV2011有一篇文章，《A Database and Evaluation Methodology for Optical Flow》里面对主流的光流算法做了简要的介绍和对不同算法进行了评估。网址是：
http://vision.middlebury.edu/flow/
感觉这个文章在光流算法的解说上非常好，条例很清晰。想了解光流的，推荐看这篇文章。另外，需要提到的一个问题是，光流场是图片中每个像素都有一个x方向和y方向的位移，所以在上面那些光流计算结束后得到的光流flow是个和原来图像大小相等的双通道图像。那怎么可视化呢？这篇文章用的是Munsell颜色系统来显示。
关于孟塞尔颜色系统（MunsellColor System），可以看[wikibaike](http://zh.wikipedia.org/wiki/%E5%AD%9F%E5%A1%9E%E5%B0%94%E9%A2%9C%E8%89%B2%E7%B3%BB%E7%BB%9F)。它是美国艺术家阿尔伯特孟塞尔（Albert H. Munsell，1858－1918）在1898年创制的颜色描述系统。
![](https://img-my.csdn.net/uploads/201303/17/1363506969_4561.jpg)
孟塞尔颜色系统的空间大致成一个圆柱形：
南北轴=明度（value），从全黑（1）到全白（10）。
经度=色相（hue）。把一周均分成五种主色调和五种中间色：红(R)、红黄(YR)、黄(Y)、黄绿(GY)、绿(G)、绿蓝(BG)、蓝(B)、蓝紫(PB)、紫(P)、紫红(RP)。相邻的两个位置之间再均分10份，共100份。
距轴的距离=色度（chroma），表示色调的纯度。其数值从中间（0）向外随着色调的纯度增加，没有理论上的上限（普通的颜色实际上限为10左右，反光、荧光等材料可高达30）。由于人眼对各种颜色的的敏感度不同，色度不一定与每个色调和明度组合相匹配。
具体颜色的标识形式为：色相+明度+色度。
在上面的那个评估的网站有这个从flow到color显示的Matlab和C++代码。但是感觉C++代码分几个文件，有点乱，然后我自己整理成两个函数了，并配合OpenCV的Mat格式。
下面的代码是用calcOpticalFlowFarneback来计算稠密光流并且用这个颜色系统来显示的。这个计算稠密光流的方法与其他几个相比还是比较快的，640x480的视频我的是200ms左右一帧，但其他的一般都需要一两秒以上。结果图中，不同颜色表示不同的运动方向，深浅就表示运动的快慢了。
void calcOpticalFlowFarneback(InputArray prevImg, InputArray nextImg,InputOutputArray flow, double pyrScale, int levels, int winsize, intiterations, int polyN, double polySigma, int flags)
大部分参数在论文中都有一套比较好的值的，直接采用他们的就好了。

```cpp
// Farneback dense optical flow calculate and show in Munsell system of colors
// Author : Zouxy
// Date   : 2013-3-15
// HomePage : http://blog.csdn.net/zouxy09
// Email  : zouxy09@qq.com
// API calcOpticalFlowFarneback() comes from OpenCV, and this
// 2D dense optical flow algorithm from the following paper:
// Gunnar Farneback. "Two-Frame Motion Estimation Based on Polynomial Expansion".
// And the OpenCV source code locate in ..\opencv2.4.3\modules\video\src\optflowgf.cpp
#include <iostream>
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;
#define UNKNOWN_FLOW_THRESH 1e9
// Color encoding of flow vectors from:
// http://members.shaw.ca/quadibloc/other/colint.htm
// This code is modified from:
// http://vision.middlebury.edu/flow/data/
void makecolorwheel(vector<Scalar> &colorwheel)
{
    int RY = 15;
    int YG = 6;
    int GC = 4;
    int CB = 11;
    int BM = 13;
    int MR = 6;
    int i;
	for (i = 0; i < RY; i++) colorwheel.push_back(Scalar(255,	   255*i/RY,	 0));
    for (i = 0; i < YG; i++) colorwheel.push_back(Scalar(255-255*i/YG, 255,		 0));
    for (i = 0; i < GC; i++) colorwheel.push_back(Scalar(0,		   255,		 255*i/GC));
    for (i = 0; i < CB; i++) colorwheel.push_back(Scalar(0,		   255-255*i/CB, 255));
    for (i = 0; i < BM; i++) colorwheel.push_back(Scalar(255*i/BM,	   0,		 255));
    for (i = 0; i < MR; i++) colorwheel.push_back(Scalar(255,	   0,		 255-255*i/MR));
}
void motionToColor(Mat flow, Mat &color)
{
	if (color.empty())
		color.create(flow.rows, flow.cols, CV_8UC3);
	static vector<Scalar> colorwheel; //Scalar r,g,b
	if (colorwheel.empty())
		makecolorwheel(colorwheel);
	// determine motion range:
    float maxrad = -1;
	// Find max flow to normalize fx and fy
	for (int i= 0; i < flow.rows; ++i) 
	{
		for (int j = 0; j < flow.cols; ++j) 
		{
			Vec2f flow_at_point = flow.at<Vec2f>(i, j);
			float fx = flow_at_point[0];
			float fy = flow_at_point[1];
			if ((fabs(fx) >  UNKNOWN_FLOW_THRESH) || (fabs(fy) >  UNKNOWN_FLOW_THRESH))
				continue;
			float rad = sqrt(fx * fx + fy * fy);
			maxrad = maxrad > rad ? maxrad : rad;
		}
	}
	for (int i= 0; i < flow.rows; ++i) 
	{
		for (int j = 0; j < flow.cols; ++j) 
		{
			uchar *data = color.data + color.step[0] * i + color.step[1] * j;
			Vec2f flow_at_point = flow.at<Vec2f>(i, j);
			float fx = flow_at_point[0] / maxrad;
			float fy = flow_at_point[1] / maxrad;
			if ((fabs(fx) >  UNKNOWN_FLOW_THRESH) || (fabs(fy) >  UNKNOWN_FLOW_THRESH))
			{
				data[0] = data[1] = data[2] = 0;
				continue;
			}
			float rad = sqrt(fx * fx + fy * fy);
			float angle = atan2(-fy, -fx) / CV_PI;
			float fk = (angle + 1.0) / 2.0 * (colorwheel.size()-1);
			int k0 = (int)fk;
			int k1 = (k0 + 1) % colorwheel.size();
			float f = fk - k0;
			//f = 0; // uncomment to see original color wheel
			for (int b = 0; b < 3; b++) 
			{
				float col0 = colorwheel[k0][b] / 255.0;
				float col1 = colorwheel[k1][b] / 255.0;
				float col = (1 - f) * col0 + f * col1;
				if (rad <= 1)
					col = 1 - rad * (1 - col); // increase saturation with radius
				else
					col *= .75; // out of range
				data[2 - b] = (int)(255.0 * col);
			}
		}
	}
}
int main(int, char**)
{
    VideoCapture cap;
	cap.open(0);
	//cap.open("test_02.wmv");
    if( !cap.isOpened() )
        return -1;
    Mat prevgray, gray, flow, cflow, frame;
    namedWindow("flow", 1);
	Mat motion2color;
    for(;;)
    {
		double t = (double)cvGetTickCount();
        cap >> frame;
        cvtColor(frame, gray, CV_BGR2GRAY);
		imshow("original", frame);
        if( prevgray.data )
        {
            calcOpticalFlowFarneback(prevgray, gray, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
			motionToColor(flow, motion2color);
            imshow("flow", motion2color);
        }
        if(waitKey(10)>=0)
            break;
        std::swap(prevgray, gray);
		t = (double)cvGetTickCount() - t;
		cout << "cost time: " << t / ((double)cvGetTickFrequency()*1000.) << endl;
    }
    return 0;
}
```

**这个是效果：**
一个挥动的手：
![](https://img-my.csdn.net/uploads/201303/17/1363507051_4161.jpg)
虽然也有背景在动，但是因为他们的运动方向不一样，所以还是可以辨认出来前面那个是手，在前景和背景运动不统一的时候，还是可以辨认出来的。
![](https://img-my.csdn.net/uploads/201303/17/1363507075_2048.jpg)



