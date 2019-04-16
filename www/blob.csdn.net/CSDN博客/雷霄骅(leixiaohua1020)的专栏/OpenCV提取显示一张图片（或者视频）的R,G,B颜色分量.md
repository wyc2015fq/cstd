# OpenCV提取显示一张图片（或者视频）的R,G,B颜色分量 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年02月11日 00:28:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：45
个人分类：[OpenCV](https://blog.csdn.net/leixiaohua1020/article/category/1362937)










使用OpenCV可以提分别提取显示一张图片（或者视频）的R,G,B颜色分量。效果如下。



原图：

![](https://img-blog.csdn.net/20140209232327640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

R：

![](https://img-blog.csdn.net/20140209233038625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

G：

![](https://img-blog.csdn.net/20140209233042859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

B：

![](https://img-blog.csdn.net/20140209233047546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



示例代码如下，貌似很久以前网上找的的，逻辑很清晰，就是把R，G，B三个分量分开，然后显示出来，就不注释了。





```cpp
#include "cv.h"
#include "highgui.h"
#include <cxcore.h>


void main(int argc,char **argv)
{
    IplImage *img=cvLoadImage("test.bmp",1);
    IplImage *channel_r=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
    IplImage *channel_g=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
    IplImage *channel_b=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
    IplImage *img_r=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,3);
    IplImage *img_g=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,3);
    IplImage *img_b=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,3);

        cvSplit(img,channel_b,channel_g,channel_r,NULL);
	cvMerge(channel_b,0,0,0,img_b);
	cvMerge(0,channel_g,0,0,img_g);
	cvMerge(0,0,channel_r,0,img_r);

	cvNamedWindow("ImageOrigin",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Image_R",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Image_G",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Image_B",CV_WINDOW_AUTOSIZE);
	cvShowImage("ImageOrigin",img);
	cvShowImage("Image_R",img_r);
	cvShowImage("Image_G",img_g);
	cvShowImage("Image_B",img_b);

	cvSaveImage("img_red.bmp",img_r);
	cvSaveImage("img_green.bmp",img_g);
	cvSaveImage("img_blue.bmp",img_b);

	cvWaitKey(0);
	cvReleaseImage(&img);
	cvReleaseImage(&img_r);
	cvReleaseImage(&img_g);
	cvReleaseImage(&img_b);
	cvDestroyAllWindows();
}
```












