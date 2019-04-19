# 图像处理（九）：特征点检测之Harris角点检测 - sinat_31425585的博客 - CSDN博客
2018年06月15日 18:12:28[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：325
    图像可以看做是一个WxHxC（width x height x channels）的矩阵来表示。同一个物体，在不同观测角度、光照强度时，图像上的像素强度会发生改变，因此考虑用特征点来对图像进行描述。特征点指的就是能够显著描述图像特征的点。
    Harris角点检测原理可以参考博客：https://blog.csdn.net/lwzkiller/article/details/54633670
    角点即像素无论在梯度方向和幅值上都存在较大变化的点，这些点为轮廓的交点，对于视角、光照强度等因素都具备较强鲁棒性。检测角点的基本思想为：验证像素点灰度值是否在任意方向上都存在较大变化。
**    1、灰度值变化计算：**
![](https://img-blog.csdn.net/20180615122606252?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
    使用一阶泰勒展开有：
![](https://img-blog.csdn.net/20180615122851101?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
    所以，灰度值的变化量为：
![](https://img-blog.csdn.net/20180615123023841?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**   2、具体计算方式：**
    通常像素点梯度变化，一般考虑的是利用自身像素与所在位置的领域内像素点之间像素值的偏差来计算。因此，计算偏差时，通常会设置一个以当前像素点坐标为中心的计算区域W，即统计像素在区域W中的变化，因此计算公式就变成：
![](https://img-blog.csdn.net/20180615173541733?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
    写成矩阵形式有：
![](https://img-blog.csdn.net/20180615174123972?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
    变化大小取决于矩阵M：
![](https://img-blog.csdn.net/20180615174643116?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
    矩阵M的特征值![](https://img-blog.csdn.net/20180615174856920?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)表示矩阵M在对应特征向量方向V上变化大小，![](https://img-blog.csdn.net/20180615174856920?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)越大表示变化越大。因此，通过计算矩阵M的两个特征值，如果两个特征值都比较大，就表示该点为角点；如果一个特征值大，一个特征小，就表示为边缘；如果都比较小就表示为平坦区域。
    特征值的计算方式为：
![](https://img-blog.csdn.net/20180615180315535?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
     定义一个衡量角点强度函数：
![](https://img-blog.csdn.net/20180615180630851?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
    为什么R是这个形式？这个在作者论文中有一个图，至于怎么得到的就不得而知了,  k为系数，一般取值为0.04~0.06，用于调节函数形状。
3、代码
这个代码不是原创，是在github上面发现的一段代码，表示代码写的非常简洁明了，代码链接地址为：https://github.com/ronnyyoung/ImageFeatures/blob/master/source/harris.cpp
```cpp
#include "imgFeat.h"
void feat::detectHarrisCorners(const Mat& imgSrc, Mat& imgDst, double alpha)
{
	Mat gray;
	if (imgSrc.channels() == 3)
	{
		cvtColor(imgSrc, gray, CV_BGR2GRAY);
	}
	else
	{
		gray = imgSrc.clone();
	}
	gray.convertTo(gray, CV_64F);
	Mat xKernel = (Mat_<double>(1,3) << -1, 0, 1);
	Mat yKernel = xKernel.t();
	Mat Ix,Iy;
	filter2D(gray, Ix, CV_64F, xKernel);
	filter2D(gray, Iy, CV_64F, yKernel);
	Mat Ix2,Iy2,Ixy;
	Ix2 = Ix.mul(Ix);
	Iy2 = Iy.mul(Iy);
	Ixy = Ix.mul(Iy);
	Mat gaussKernel = getGaussianKernel(7, 1);
	filter2D(Ix2, Ix2, CV_64F, gaussKernel);
	filter2D(Iy2, Iy2, CV_64F, gaussKernel);
	filter2D(Ixy, Ixy, CV_64F, gaussKernel);
	
	Mat cornerStrength(gray.size(), gray.type());
	for (int i = 0; i < gray.rows; i++)
	{
		for (int j = 0; j < gray.cols; j++)
		{
			double det_m = Ix2.at<double>(i,j) * Iy2.at<double>(i,j) - Ixy.at<double>(i,j) * Ixy.at<double>(i,j);
			double trace_m = Ix2.at<double>(i,j) + Iy2.at<double>(i,j);
			cornerStrength.at<double>(i,j) = det_m - alpha * trace_m *trace_m;
		}
	}
	// threshold
	double maxStrength;
	minMaxLoc(cornerStrength, NULL, &maxStrength, NULL, NULL);
	Mat dilated;
	Mat localMax;
	dilate(cornerStrength, dilated, Mat());
	compare(cornerStrength, dilated, localMax, CMP_EQ);
	
	Mat cornerMap;
	double qualityLevel = 0.01;
	double thresh = qualityLevel * maxStrength;
	cornerMap = cornerStrength > thresh;
	bitwise_and(cornerMap, localMax, cornerMap);
	
	imgDst = cornerMap.clone();
	
}
```
效果如下：
![](https://img-blog.csdn.net/20180615181204965?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
