# 图像处理（八）：线段检测之Hough变换 - sinat_31425585的博客 - CSDN博客
2017年11月27日 15:12:30[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：1823
现阶段用最多的线段检测算法为Hough变换，对于Hough变换的原理部分已经有很多人写过，具体可以参考：[点击打开链接](http://m.blog.csdn.net/ws_20100/article/details/51159434)，这里我总结一下理解Hough变换算法的难点。
一、直线表示方法：
1）最常用的方法为斜截式：y=kx+b，已知直线的斜率及y轴上的截距，即可确定一条直线。
2）Hough变换中参数方程：rho = x * cos(theta) + y * sin(theta)，原理部分一般都会介绍，每一对(rho, theta)对应一条直线，为什么？可以看一下下面这个图
![](https://img-blog.csdn.net/20171127150337143?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzE0MjU1ODU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
可以看到，这里的rho和theta分别对应于原点到直线y=kx+b的距离以及这条垂线对应夹角。知道这两个东西，自然就可以知道一条直线。
二、实现步骤
1）灰度化，并使用Canny进行边缘检测；
2）参数空间离散化：确定检测精度；
3）Hough参数空间统计；
4）四邻域非极大值抑制；
三、代码如下：
```cpp
#include <opencv2/opencv.hpp>
#include <math.h>
using namespace cv;
using namespace std;
//用来表示极坐标参数空间里的数据
struct Para {
	int num;
	float angle;
	int rho;
};
int main()
{
	Mat src = imread("1.jpg");
	Mat gray;
	cvtColor(src, gray, CV_BGR2GRAY);
	Mat edges;
	Canny(gray, edges, 50, 200);
	int i, j, k;
	// hough transform
	int q_max = sqrt(pow(gray.rows, 2) + pow(gray.cols, 2));
	int nAngles = 180;  // 划分为180份
	// 为hough参数空间分配内存及初始化
	int ** hough_data = new int*[q_max];
	for (i = 0; i < q_max; i++)
	{
		hough_data[i] = new int[nAngles];
		for (j = 0; j < nAngles; j++)
		{
			hough_data[i][j] = 0;
		}
	}
	// Hough参数空间统计
	for (i = 0; i < edges.rows; i++)
		for (j = 0; j < edges.cols; j++)
		{
			if (edges.at<uchar>(i,j) == 255)
			{
				int index = 0;
				for (k = 0; k < nAngles; k++)
				{
					index = int(i * cos(CV_PI / nAngles * k) + 
						        j * sin(CV_PI / nAngles * k));
					index = int( (index + q_max) / 2 );
					hough_data[index][k]++;
				}
			}
		}
	// 统计之后进行非极大值抑制
	Para *houghSpace = new Para[q_max * nAngles];
	int thr = 200;
	int counts = 0;
	for (i = 0; i < q_max; i++)
		for (j = 0; j < nAngles; j++)
		{
			// 大于阈值且为四邻域的极大值
			if (hough_data[i][j] > thr &&
				hough_data[i][j] > hough_data[i + 1][j] && 
				hough_data[i][j] > hough_data[i - 1][j] &&
				hough_data[i][j] > hough_data[i][j + 1] &&
				hough_data[i][j] > hough_data[i][j - 1]
				)
			{
				houghSpace[counts].rho = i * 2 - q_max;
				houghSpace[counts].angle = j * CV_PI / nAngles;
				houghSpace[counts].num = hough_data[i][j];
				counts++;
			}
		}
	// 得到了参数之后，画线
	for (k = 0; k < counts; k++)
	{
		if (houghSpace[k].angle < 0.1)
		{
			line(src, Point(0, houghSpace[k].rho), Point(src.cols, houghSpace[k].rho), Scalar(255, 0, 0));
		}
		else
		{
			line(src, Point(houghSpace[k].rho / sin(houghSpace[k].angle), 0), Point( 0, (houghSpace[k].rho / cos(houghSpace[k].angle))), Scalar(255, 0, 0));
		}
	}
	delete[] hough_data;
	delete[] houghSpace;
	imshow("edges", edges);
	imshow("hough", src);
	waitKey();
	return 0;
}
```
效果如下：
![](https://img-blog.csdn.net/20171127150819973?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzE0MjU1ODU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
