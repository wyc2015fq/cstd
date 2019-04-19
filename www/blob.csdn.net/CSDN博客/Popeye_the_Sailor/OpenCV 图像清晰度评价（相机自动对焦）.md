# OpenCV 图像清晰度评价（相机自动对焦） - Popeye_the_Sailor - CSDN博客
2018年07月22日 18:58:44[_Sailor_](https://me.csdn.net/lz0499)阅读数：563
#                      OpenCV 图像清晰度评价（相机自动对焦）
相机的自动对焦要求相机根据拍摄环境和场景的变化，通过相机内部的微型驱动马达，自动调节相机镜头和CCD之间的距离，保证像平面正好投影到CCD的成像表面上。这时候物体的成像比较清晰，图像细节信息丰富。
相机自动对焦的过程，其实就是对成像清晰度评价的过程，对焦不准确，拍摄出来的图像清晰度低，视觉效果模糊，如果是在工业检测测量领域，对焦不准导致的后果可能是致命的；对焦准确的图像清晰度较高，层次鲜明，对比度高。
图像清晰度评价算法有很多种，在空域中，主要思路是考察图像的领域对比度，即相邻像素间的灰度特征的梯度差；在频域中，主要思路是考察图像的频率分量，对焦清晰的图像高频分量较多，对焦模糊的图像低频分量较多。
这里实现3种清晰度评价方法，分别是Tenengrad梯度方法、Laplacian梯度方法和方差方法。
## Tenengrad梯度方法
Tenengrad梯度方法利用Sobel算子分别计算水平和垂直方向的梯度，同一场景下梯度值越高，图像越清晰。以下是具体实现，这里衡量的指标是经过Sobel算子处理后的图像的平均灰度值，值越大，代表图像越清晰。
```cpp
#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>
 
using namespace std;
using namespace cv;
 
int main()
{
	Mat imageSource = imread("2.jpg");
	Mat imageGrey;
 
	cvtColor(imageSource, imageGrey, CV_RGB2GRAY);
	Mat imageSobel;
	Sobel(imageGrey, imageSobel, CV_16U, 1, 1);
 
	//图像的平均灰度
	double meanValue = 0.0;
	meanValue = mean(imageSobel)[0];
 
	//double to string
	stringstream meanValueStream;
	string meanValueString;
	meanValueStream << meanValue;
	meanValueStream >> meanValueString;
	meanValueString = "Articulation(Sobel Method): " + meanValueString;
	putText(imageSource, meanValueString, Point(20, 50), CV_FONT_HERSHEY_COMPLEX, 0.8, Scalar(255, 255, 25), 2);
	imshow("Articulation", imageSource);
	waitKey();
}
```
使用三张测试图片模拟不同对焦。第一张最清晰，得分最高，第二三张越来越模糊，得分依次降低。
![](https://img-blog.csdn.net/20161209200522391)
![](https://img-blog.csdn.net/20161209200535032)
![](https://img-blog.csdn.net/20161209200601329)
## Laplacian梯度方法：
Laplacian梯度是另一种求图像梯度的方法，在上例的OpenCV代码中直接替换Sobel算子即可。
```cpp
#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>
 
using namespace std;
using namespace cv;
 
int main()
{
	Mat imageSource = imread("1.jpg");
	Mat imageGrey;
 
	cvtColor(imageSource, imageGrey, CV_RGB2GRAY);
	Mat imageSobel;
 
	Laplacian(imageGrey, imageSobel, CV_16U);
	//Sobel(imageGrey, imageSobel, CV_16U, 1, 1);
 
	//图像的平均灰度
	double meanValue = 0.0;
	meanValue = mean(imageSobel)[0];
 
	//double to string
	stringstream meanValueStream;
	string meanValueString;
	meanValueStream << meanValue;
	meanValueStream >> meanValueString;
	meanValueString = "Articulation(Laplacian Method): " + meanValueString;
	putText(imageSource, meanValueString, Point(20, 50), CV_FONT_HERSHEY_COMPLEX, 0.8, Scalar(255, 255, 25), 2);
	imshow("Articulation", imageSource);
	waitKey();
}
```
同样的三张测试图片测试，结果一致，随着对焦模糊得分降低：
![](https://img-blog.csdn.net/20161209201327129)
![](https://img-blog.csdn.net/20161209201344582)
![](https://img-blog.csdn.net/20161209201356001)
## 方差方法：
方差是概率论中用来考察一组离散数据和其期望（即数据的均值）之间的离散（偏离）成都的度量方法。方差较大，表示这一组数据之间的偏差就较大，组内的数据有的较大，有的较小，分布不均衡；方差较小，表示这一组数据之间的偏差较小，组内的数据之间分布平均，大小相近。
对焦清晰的图像相比对焦模糊的图像，它的数据之间的灰度差异应该更大，即它的方差应该较大，可以通过图像灰度数据的方差来衡量图像的清晰度，**方差越大，表示清晰度越好**
```cpp
#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>
 
using namespace std;
using namespace cv;
 
int main()
{
	Mat imageSource = imread("2.jpg");
	Mat imageGrey;
 
	cvtColor(imageSource, imageGrey, CV_RGB2GRAY);
	Mat meanValueImage;
	Mat meanStdValueImage;
 
	//求灰度图像的标准差
	meanStdDev(imageGrey, meanValueImage, meanStdValueImage);
	double meanValue = 0.0;
	meanValue = meanStdValueImage.at<double>(0, 0);
 
	//double to string
	stringstream meanValueStream;
	string meanValueString;
	meanValueStream << meanValue*meanValue;
	meanValueStream >> meanValueString;
	meanValueString = "Articulation(Variance Method): " + meanValueString;
 
	putText(imageSource, meanValueString, Point(20, 50), CV_FONT_HERSHEY_COMPLEX, 0.8, Scalar(255, 255, 25), 2);
	imshow("Articulation", imageSource);
	waitKey();
}
```
方差数值随着清晰度的降低逐渐降低：
![](https://img-blog.csdn.net/20161209202308570)
![](https://img-blog.csdn.net/20161209202320482)
![](https://img-blog.csdn.net/20161209202330820)
**在工业应用中，最清晰的对焦拍摄出来的图像不一定是最好的，有可能出现摩尔纹（水波纹）现象，一般需要在最清晰对焦位置附件做一个微调。**
转载自：[OpenCV 图像清晰度评价（相机自动对焦）](https://blog.csdn.net/dcrmg/article/details/53543341)
