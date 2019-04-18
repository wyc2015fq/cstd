# OpenCV直方图均衡化 - wishchinYang的专栏 - CSDN博客
2016年04月20日 17:16:07[wishchin](https://me.csdn.net/wishchin)阅读数：807
对于排除一个点的直方图均衡化，可以用以下代码实现
代码：
```cpp
namespace cvWish
{
	int stretch2(const cv::Mat& image,cv::Mat &result,int v) 
	{
		cv::Mat hist(1, 256, CV_32SC1);
		int histSize = 256;  
		float range[] = {0, 255};  
		const float *ranges = { range };  
		const int  channels = 0;  
		cv::calcHist(&image, 1, 0,cv::Mat(), hist,1, &histSize, &ranges, true, false);
		float scale[256];
		float lookupF[256];
		cv::Mat lookup(cv::Size(1, 256), CV_8U);
		int pixNum = image.cols * image.rows;
		for (int i =0; i <256; i++) {
			scale[i] = hist.at<float>(i) / pixNum *255;
		}
		float  numMedian = scale[v];//
		numMedian /= 256;
		scale[v] = 0;
		for (int i =0; i <256; i++) {
			scale[i] += numMedian;
		}
		for (int i =0; i <256; i++) {
			//scale[i] = hist.at<float>(i) / pixNum *255;
			if (i ==0) 
			{
				lookupF[i] = scale[i];
			} 
			else 
			{
				lookupF[i] = lookupF[i -1] + scale[i];
			}
		}
		for (int i =0; i <256; i++) 
		{
			lookup.at<uchar>(i) = static_cast<uchar>(lookupF[i]);
		}
		//lookup.at<uchar>(v) = static_cast<uchar>(lookupF[v]);//写法是错误的，不必修改了！
		//cv::Mat result;
		cv::LUT(image, lookup, result);
		return 1;
	}
	void equalizeHistExcludeValue(const cv::Mat &mat_src, cv::Mat &mat_dst,int v)
	{
		stretch2(mat_src,mat_dst,v);
	}
}
```
