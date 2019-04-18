# OpenCV中的模板匹配/Filter2d - wishchinYang的专栏 - CSDN博客
2016年04月06日 14:42:17[wishchin](https://me.csdn.net/wishchin)阅读数：2755

**1、模板匹配**
模板匹配是在图像中寻找目标的方法之一。Come On, Boy.我们一起来看看模板匹配到底是怎么回事。
    参考链接：[http://www.opencv.org.cn/opencvdoc/2.3.2/html/doc/tutorials/imgproc/histograms/template_matching/template_matching.html](http://www.opencv.org.cn/opencvdoc/2.3.2/html/doc/tutorials/imgproc/histograms/template_matching/template_matching.html)
[http://www.cnblogs.com/xrwang/archive/2010/02/05/MatchTemplate.html](http://www.cnblogs.com/xrwang/archive/2010/02/05/MatchTemplate.html)
**模板匹配的工作方式**
 模板匹配的工作方式跟直方图的反向投影基本一样，大致过程是这样的：通过在输入图像上滑动图像块对实际的图像块和输入图像进行匹配。
    假设我们有一张100x100的输入图像，有一张10x10的模板图像，查找的过程是这样的：
  （1）从输入图像的左上角(0,0)开始，切割一块(0,0)至(10,10)的临时图像；
  （2）用临时图像和模板图像进行对比，对比结果记为c；
  （3）对比结果c，就是结果图像(0,0)处的像素值；
  （4）切割输入图像从(0,1)至(10,11)的临时图像，对比，并记录到结果图像；
  （5）重复（1）～（4）步直到输入图像的右下角。
    大家可以看到，直方图反向投影对比的是直方图，而模板匹配对比的是图像的像素值；模板匹配比直方图反向投影速度要快一些，但是我个人认为直方图反向投影的鲁棒性会更好。
**模板匹配的匹配方式**
    在OpenCv和EmguCv中支持以下6种对比方式：
    CV_TM_SQDIFF 平方差匹配法：该方法采用平方差来进行匹配；最好的匹配值为0；匹配越差，匹配值越大。
    CV_TM_CCORR 相关匹配法：该方法采用乘法操作；数值越大表明匹配程度越好。
    CV_TM_CCOEFF 相关系数匹配法：1表示完美的匹配；-1表示最差的匹配。
    CV_TM_SQDIFF_NORMED 归一化平方差匹配法
    CV_TM_CCORR_NORMED 归一化相关匹配法
    CV_TM_CCOEFF_NORMED 归一化相关系数匹配法
    根据我的测试结果来看，上述几种匹配方式需要的计算时间比较接近（跟《学习OpenCv》书上说的不同），我们可以选择一个能适应场景的匹配方式。
**显示结果图像**
    模板匹配和直方图反向投影生成的结果图像都是32位浮点型单通道图像
![](https://img-blog.csdn.net/20160406144244207?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**模板匹配和直方图反向投影的效率**
    总的来说，模板匹配和直方图反向投影的效率都不高。在我的机器上，在1136*852大小的输入图像上匹配104*132的大小的模板图像（都是单通道灰度图像），大约需要700毫秒；而直方图反向投影大约需要75000毫秒（1.25分钟）。看来还需要继续学习，寻找更好的处理方法。
    另一方面，通过搜索OpenCv的源代码，发现OpenCv基本上没有使用并行计算。如果学习完之后，还有时间和热情，我准备尝试优化下OpenCv的并行计算；如果.net 4.0正式版推出了，也可以选择在这一方面做点优化。
代码：
```cpp
// 用于存储匹配结果的矩阵 
	cv::Mat result;
	int result_cols =  minM.cols - tempMat.cols + 1;  
	int result_rows =  minM.rows - tempMat.rows + 1;  
	result.create( result_cols, result_rows, CV_32FC1 );  
	// 进行模板匹配  
	int match_method  = CV_TM_SQDIFF;
	cv::matchTemplate( MatSrc, tempMat, result, match_method );  
	// 归一化结果（方便显示结果）  
	//cv::normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );  
	// 找到最佳匹配位置  
	double minVal  =0;   
	double maxVal  =0;   
	cv::Point minLoc(0,0);   
	cv::Point maxLoc(0,0);  
	cv::Point matchLoc(0,0);  
	cv::Point matchCenter(0,0);  
	cv::Point matchSrc(0,0);  
	cv::minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );   // 寻找result中的最大和最小值，以及它们所处的像素位置
	// 使用SQDIFF和SQDIFF_NORMED方法时：值越小代表越相似  
	// 使用其他方法时：值越大代表越相似  
	if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )  
	{  
		matchLoc = minLoc;   
	}  
	else  
	{   
		matchLoc = maxLoc;   
	}
```
**2、Filter2d**
OpenCv Filter2d进行二维卷积进行的是相关运算；
参考链接：[http://blog.csdn.net/superdont/article/details/6662365](http://blog.csdn.net/superdont/article/details/6662365)
高通和低通滤波
```cpp
#include "cv.h"
#include "highgui.h"
int main(int argc,char**argv) 
{ 
  IplImage* src, *dst, src_f;  
 
  float low[9] ={ 1.0/16, 2.0/16, 1.0/16, 2.0/16, 4.0/16, 2.0/16, 1.0/16, 2.0/16, 1.0/16 };//低通滤波核
  float high[9]={-1,-1,-1,-1,9,-1,-1,-1,-1};//高通滤波核
  CvMat km = cvMat( 3, 3, CV_32FC1, low);  //构造单通道浮点矩阵，将图像IplImage结构转换为图像数组 
  src = cvLoadImage( "lena.jpg" ); 
  st = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 3 );
  cvFilter2D( src, dst, &km, cvPoint( -1, -1 ) );  //设参考点为核的中心
  cvNamedWindow( "src", 0 );
  cvNamedWindow( "filtering", 0 );
  cvShowImage( "src", src );  
  cvShowImage( "filtering", dst ); 
  cvWaitKey(0); 
  cvReleaseImage( &src ); 
  cvReleaseImage( &dst ); 
  return 0; 
}
```
**3、通过傅里叶变换求图像卷积**
参考链接：[http://blog.csdn.net/lichengyu/article/details/18848281](http://blog.csdn.net/lichengyu/article/details/18848281)
各种滤波(平滑、锐化)的情况，基本方法都是将图像与一个核进行卷积实现。而卷积定理指出，两个函数的卷积的傅里叶变换等于各自的傅里叶变换的乘积，即：
![](https://img-blog.csdn.net/20140128215844546)[1]
那么，两个函数的卷积可以通过如下方式得到，对两个函数傅里叶变换的乘积做傅里叶反变换，即：
![](https://img-blog.csdn.net/20140128215857359)[1]
在进行卷积运算时，一般是将核沿着图像从左到右从上到下计算每一个像素处与核卷积后的值，这样的计算量较大，采用傅里叶变换的方法可以提高运算效率。
............................................
代码见原文章.....
需要注意的是，一般求法中，利用核游走整个图像进行卷积运算，实际上进行的是相关运算，真正意义上的卷积，应该首先把核翻转180度，再在整个图像上进行游走。OpenCV中的filter2D实际上做的也只是相关，而非卷积。"The function does actually compute correlation, not the convolution: ... That is, the kernel is not mirrored around the anchor point. If you need
 a real convolution, flip the kernel using flip() and set the new anchor to (kernel.cols - anchor.x - 1, kernel.rows - anchor.y - 1)
