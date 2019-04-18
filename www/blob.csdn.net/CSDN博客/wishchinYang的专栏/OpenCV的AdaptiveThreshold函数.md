# OpenCV的AdaptiveThreshold函数 - wishchinYang的专栏 - CSDN博客
2015年10月20日 10:29:11[wishchin](https://me.csdn.net/wishchin)阅读数：9311
        摘自于OpenCV Doc2.410，opencv2refman文档.
1.函数原型  
```cpp
adaptiveThreshold
//Applies an adaptive threshold to an array.
C++:     void adaptiveThreshold(InputArray src, OutputArray dst, double maxValue, int adaptiveMethod,int thresholdType, int blockSize, double C)
Python:  cv2.adaptiveThreshold(src, maxValue, adaptiveMethod, thresholdType, blockSize, C[, dst ]) !dst
C:       void cvAdaptiveThreshold(
                  const CvArr* src, CvArr* dst, double max_value,
                  int adaptive_method=CV_ADAPTIVE_THRESH_MEAN_C, int threshold_type=CV_THRESH_BINARY, 
                  int block_size=3, double param1=5)
Python:  cv.AdaptiveThreshold(src, dst, maxValue, adaptive_method=CV_ADAPTIVE_THRESH_MEAN_C,thresholdType=CV_THRESH_BINARY, blockSize=3, param1=5) !
None
Parameters
src – Source 8-bit single-channel image.
dst – Destination image of the same size and the same type as src .
maxValue – Non-zero value assigned to the pixels for which the condition is satisfied. See
the details below.
adaptiveMethod – Adaptive thresholding algorithm to use, ADAPTIVE_THRESH_MEAN_C or
ADAPTIVE_THRESH_GAUSSIAN_C . See the details below.
thresholdType – Thresholding type that must be either THRESH_BINARY or
THRESH_BINARY_INV .
```
2.图片处理效果
原始图片：
![](https://img-blog.csdn.net/20151020102604922?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
处理效果：
![](https://img-blog.csdn.net/20151020102420500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
对比Canny的处理效果：
![](https://img-blog.csdn.net/20151020105402442?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
 结果就差一个反色处理？
Canny+反色
![](https://img-blog.csdn.net/20151020113411729?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3.方法分析
参考链接：[http://blog.chinaunix.net/uid-20805029-id-1704893.html](http://blog.chinaunix.net/uid-20805029-id-1704893.html)
函数 cvAdaptiveThreshold 将灰度图像变换到二值图像；
     对方法 CV_ADAPTIVE_THRESH_MEAN_C，先求出块中的均值，再减掉param1。 
     对方法 CV_ADAPTIVE_THRESH_GAUSSIAN_C ，先求出块中的加权和(gaussian)， 再减掉param1。
参数不同会产生不同的转换效果.

