# OpenCV 实现颜色直方图 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年11月10日 16:33:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：36









颜色直方图是在许多图像检索系统中被广泛采用的颜色特征。它所描述的是不同色彩在整幅图像中所占的比例，而并不关心每种色彩所处的空间位置，即无法描述图像中的对象或物体。颜色直方图特别适于描述那些难以进行自动分割的图像。

OpenCV对颜色直方图进行统计的代码如下所示，在图像检索，镜头分割等领域使用的还是比较多的。



```cpp
#include <cv.h>
#include <highgui.h>
#include <iostream>
using namespace std;



int main( int argc, char** argv )
{
	IplImage * src= cvLoadImage("F:\\test3.jpg");

	IplImage* hsv = cvCreateImage( cvGetSize(src), 8, 3 );
	IplImage* h_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* s_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* v_plane = cvCreateImage( cvGetSize(src), 8, 1 );
	IplImage* planes[] = { h_plane, s_plane };

	/** H 分量划分为16个等级，S分量划分为8个等级 */
	int h_bins = 16, s_bins = 8;
	int hist_size[] = {h_bins, s_bins};

	/** H 分量的变化范围 */
	float h_ranges[] = { 0, 180 }; 

	/** S 分量的变化范围*/
	float s_ranges[] = { 0, 255 };
	float* ranges[] = { h_ranges, s_ranges };

	/** 输入图像转换到HSV颜色空间 */
	cvCvtColor( src, hsv, CV_BGR2HSV );
	cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );

	/** 创建直方图，二维, 每个维度上均分 */
	CvHistogram * hist = cvCreateHist( 2, hist_size, CV_HIST_ARRAY, ranges, 1 );
	/** 根据H,S两个平面数据统计直方图 */
	cvCalcHist( planes, hist, 0, 0 );

	/** 获取直方图统计的最大值，用于动态显示直方图 */
	float max_value;
	cvGetMinMaxHistValue( hist, 0, &max_value, 0, 0 );


	/** 设置直方图显示图像 */
	int height = 240;
	int width = (h_bins*s_bins*6);
	IplImage* hist_img = cvCreateImage( cvSize(width,height), 8, 3 );
	cvZero( hist_img );

	/** 用来进行HSV到RGB颜色转换的临时单位图像 */
	IplImage * hsv_color = cvCreateImage(cvSize(1,1),8,3);
	IplImage * rgb_color = cvCreateImage(cvSize(1,1),8,3);
	int bin_w = width / (h_bins * s_bins);
	for(int h = 0; h < h_bins; h++)
	{
		for(int s = 0; s < s_bins; s++)
		{
			int i = h*s_bins + s;
			/** 获得直方图中的统计次数，计算显示在图像中的高度 */
			float bin_val = cvQueryHistValue_2D( hist, h, s );
			int intensity = cvRound(bin_val*height/max_value);

			/** 获得当前直方图代表的颜色，转换成RGB用于绘制 */
			cvSet2D(hsv_color,0,0,cvScalar(h*180.f / h_bins,s*255.f/s_bins,255,0));
			cvCvtColor(hsv_color,rgb_color,CV_HSV2BGR);
			CvScalar color = cvGet2D(rgb_color,0,0);

			cvRectangle( hist_img, cvPoint(i*bin_w,height),
				cvPoint((i+1)*bin_w,height - intensity),
				color, -1, 8, 0 );
		}
	}

	cvNamedWindow( "Source", 1 );
	cvShowImage( "Source", src );

	cvNamedWindow( "H-S Histogram", 1 );
	cvShowImage( "H-S Histogram", hist_img );

	cvWaitKey(0);
}
```



原文地址：[http://wiki.opencv.org.cn/index.php/图像颜色分布直方图](http://wiki.opencv.org.cn/index.php/%E5%9B%BE%E5%83%8F%E9%A2%9C%E8%89%B2%E5%88%86%E5%B8%83%E7%9B%B4%E6%96%B9%E5%9B%BE)






