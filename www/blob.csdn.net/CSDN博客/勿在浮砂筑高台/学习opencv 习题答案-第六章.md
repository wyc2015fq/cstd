# 学习opencv 习题答案-第六章 - 勿在浮砂筑高台 - CSDN博客





2014年11月30日 10:32:25[勿在浮砂筑高台](https://me.csdn.net/luoshixian099)阅读数：1687








/*6_2*/



```cpp
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include "stdio.h"
#include <iostream>
#include "math.h"
using namespace std;


int main(int argc, char **argv)
{

   IplImage * m_image = cvLoadImage(argv[1]);
   IplImage * image_dst = cvCloneImage(m_image);
   IplImage * image_dst2 =cvCloneImage(m_image);
   float m_num[9] = {0.0625,0.0625*2,0.0625,0.0625*2,0.0625*4,0.0625*2,0.0625,0.0625*2,0.0625};
  CvMat m_mat =cvMat(3,3,CV_32FC1,m_num);
  CvMat *m_mat1=cvCreateMat(1,3,CV_32FC1);
  CvMat *m_mat2=cvCreateMat(3,1,CV_32FC1);
  cvSetReal2D(m_mat1,0,0,0.25);
  cvSetReal2D(m_mat1,0,1,0.5);
  cvSetReal2D(m_mat1,0,2,0.25);
  cvSetReal2D(m_mat2,0,0,0.25);
  cvSetReal2D(m_mat2,1,0,0.5);
  cvSetReal2D(m_mat2,2,0,0.25);
  cvFilter2D(m_image,image_dst,&m_mat);
  cvFilter2D(m_image,image_dst2,m_mat1);
  cvFilter2D(image_dst2,image_dst2,m_mat2);

  cvShowImage("Filter",image_dst);
  cvShowImage("Src",m_image);
  cvShowImage("Filter2",image_dst2);
  cvWaitKey(0);
  cvReleaseImage(&m_image);
  cvReleaseImage(&image_dst);
  cvReleaseImage(&image_dst2);
  cvReleaseMat(&m_mat1);
  cvReleaseMat(&m_mat2);
  cvDestroyWindow("Filter");
  cvDestroyWindow("Src");
  cvDestroyWindow("Filter2");
	return 0;
}
```

![](https://img-blog.csdn.net/20141130103432374?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVvc2hpeGlhbjA5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20141130103523932?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVvc2hpeGlhbjA5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20141130103501093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVvc2hpeGlhbjA5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



/**********效果同a的效果一样************************************/




/*****6_4**********/



```cpp
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include "stdio.h"
#include <iostream>
#include "math.h"
using namespace std;


int main(int argc, char **argv)
{

	IplImage * m_image = cvLoadImage(argv[1]);
	IplImage * Sobel_3 = cvCreateImage(cvGetSize(m_image),IPL_DEPTH_8U,3);
	IplImage * Sobel_5 = cvCreateImage(cvGetSize(m_image),IPL_DEPTH_8U,3);
	IplImage * Sobel_7 = cvCreateImage(cvGetSize(m_image),IPL_DEPTH_8U,3);
	cvSobel(m_image,Sobel_3,1,1,3);
	cvSobel(m_image,Sobel_5,1,1,5);
	cvSobel(m_image,Sobel_7,1,1,7);

	cvShowImage("Soble_3",Sobel_3);
	cvShowImage("Soble_5",Sobel_5);
	cvShowImage("Soble_7",Sobel_7);
	cvShowImage("Src",m_image);

	cvWaitKey(0);
	cvReleaseImage(&m_image);
	cvReleaseImage(&Sobel_3);
	cvReleaseImage(&Sobel_5);
	cvReleaseImage(&Sobel_7);
	cvDestroyWindow("Soble_3");
	cvDestroyWindow("Soble_5");
	cvDestroyWindow("Soble_7");
	cvDestroyWindow("Src");

	return 0;
}
```
![](https://img-blog.csdn.net/20141130121448992?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVvc2hpeGlhbjA5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20141130121514873?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVvc2hpeGlhbjA5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20141130121538226?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVvc2hpeGlhbjA5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20141130121556072?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVvc2hpeGlhbjA5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

/**********************x方向的导数，会把圆的上下边界切平，y方向的导数会把圆的左右边界切平**************************/






/***************************6_5***********************************/



```cpp
<pre name="code" class="cpp">#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include "stdio.h"
#include <iostream>
#include "math.h"
using namespace std;
int main(int argc, char **argv)
{

	IplImage * m_image = cvLoadImage(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
	IplImage * Sobel_3_x = cvCreateImage(cvGetSize(m_image),IPL_DEPTH_32F,1);//存放x方向导数
	IplImage * Sobel_3_y = cvCreateImage(cvGetSize(m_image),IPL_DEPTH_32F,1);//存放y方向导数

	
	cvSobel(m_image,Sobel_3_x,1,0,3);
	cvSobel(m_image,Sobel_3_y,0,1,3);

	CvMat* m_mat = cvCreateMat(m_image->height,m_image->width,CV_32FC1);  //存放幅值
	cvZero(m_mat);
    double val_max=0.0;
   double theta =0.0;
	for (int i=0; i<m_image->height; i++)
	{
		for(int j=0; j<m_image->width;j++)
		{
			double x=cvGetReal2D(Sobel_3_x,i,j);
			double y=cvGetReal2D(Sobel_3_y,i,j);
		    double val =(double)sqrt((x)*(x)+y*y);              //计算幅值并赋值
	        cvSetReal2D(m_mat,i,j,val);
			if (val>=val_max&&val >1)                         
			{
				val_max = val;
		    }

        
	}
}
	for (int i=0; i<m_image->height; i++)
	{
		for(int j=0; j<m_image->width;j++)
		{
			double x=cvGetReal2D(Sobel_3_x,i,j);
			double y=cvGetReal2D(Sobel_3_y,i,j);
		    double val =(double)sqrt((x)*(x)+y*y);              //计算幅值并赋值
	        cvSetReal2D(m_mat,i,j,val);
			if (abs(val-val_max)<100)                         
			{		
				if (theta != 0.0)
				{
					theta=theta+180/3.14*atan2(abs(y),abs(x));      //找幅值附近的值求平均，并记录对应的角度
					theta=theta/2.0;                                   
				}
				else
                theta=180/3.14*atan2(abs(y),abs(x)); 
			}
	
		}

        
	}
	




	cout<<"3*3中孔："<<endl;
	cout<<"最大幅值为"<<val_max<<endl;
    cout<<"对应的角度"<<theta<<endl;

    cvShowImage("CMAT",m_mat);
	cvShowImage("Src",m_image);
    
	cvWaitKey(0);
	cvReleaseImage(&m_image);
    cvReleaseImage(&Sobel_3_x);
	cvReleaseImage(&Sobel_3_y);
	cvReleaseMat(&m_mat);
	cvDestroyWindow("Src");

	return 0;
}
```



![](https://img-blog.csdn.net/20141130170157990?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVvc2hpeGlhbjA5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20141130170222123)

![](https://img-blog.csdn.net/20141130170251202)![](https://img-blog.csdn.net/20141130170341371)

![](https://img-blog.csdn.net/20141130170506265)





```cpp
/* 6_6 */
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include "stdio.h"
#include <iostream>
#include "math.h"
using namespace std;
int main(int argc, char **argv)
{

	IplImage * m_image = cvLoadImage(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
    IplImage *laplace =cvCreateImage(cvGetSize(m_image),IPL_DEPTH_32F,1);
	cvLaplace(m_image,laplace,7);
	cvShowImage("eye",m_image);
	cvShowImage("LapLace",laplace);
	cvWaitKey(0);
	cvReleaseImage(&m_image);
    cvReleaseImage(&laplace);
	cvDestroyWindow("eye");
    cvDestroyWindow("Laplace");
	return 0;
}
```
![](https://img-blog.csdn.net/20141130185223680)






```cpp
/* 6_7 */
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include "stdio.h"
#include <iostream>
#include "math.h"
using namespace std;
int main(int argc, char **argv)
{

	IplImage * m_image = cvLoadImage(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
    IplImage *caany_3_2 =cvCreateImage(cvGetSize(m_image),IPL_DEPTH_8U,1);
	IplImage *caany_11_4 =cvCreateImage(cvGetSize(m_image),IPL_DEPTH_8U,1);
	IplImage *caany_4_1 =cvCreateImage(cvGetSize(m_image),IPL_DEPTH_8U,1);
	cvCanny(m_image,caany_3_2,30,45);  
	cvCanny(m_image,caany_11_4,16,45);
	cvCanny(m_image,caany_4_1 ,11,45);
    
	cvShowImage("Canny3_2",caany_3_2);
	cvShowImage("Canny11_4",caany_11_4);
	cvShowImage("Canny4_1",caany_4_1);
	cvWaitKey(0);
	cvReleaseImage(&m_image);
    cvReleaseImage(&caany_3_2);
	cvReleaseImage(&caany_11_4);
	cvReleaseImage(&caany_4_1);

    cvDestroyWindow("Canny3_2");
	cvDestroyWindow("Canny11_4");
	cvDestroyWindow("Canny4_1");
	return 0;
}
```

```cpp
</pre><pre name="code" class="cpp"><span style="font-size:24px;color:#ff0000;">7_a</span>
```

![](https://img-blog.csdn.net/20141130194329741?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVvc2hpeGlhbjA5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


7_b

```cpp
</pre><img src="https://img-blog.csdn.net/20141130194634882?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVvc2hpeGlhbjA5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center" alt="" /><p><span style="color:rgb(255,0,0); font-size:18px">7_c</span></p><p></p><pre name="code" class="cpp">
```
![](https://img-blog.csdn.net/20141130194825265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVvc2hpeGlhbjA5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



纵向看可以看到，边界随着高阈值的增大，噪声越来越小，但是当达到180时，边界开始失真，消除除了部分真实的边界。横向看，随着比例变小，噪声多，但是到C后，比例小，又成了优点。






```cpp
/* 6_8 */
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include "stdio.h"
#include <iostream>
#include "math.h"
using namespace std;
int main(int argc, char **argv)
{

	IplImage * m_image = cvLoadImage("bike.jpg",CV_LOAD_IMAGE_GRAYSCALE);
    IplImage *image_BGR =cvCreateImage(cvGetSize(m_image),m_image->depth,3);
	cvCvtColor(m_image,image_BGR,CV_GRAY2BGR);

	CvMemStorage * m_storage_cirlce = cvCreateMemStorage(0);
	cvSmooth(m_image,m_image);  //霍夫圆变换之前做平滑处理
	CvSeq * m_seq2 = cvHoughCircles(m_image,m_storage_cirlce,CV_HOUGH_GRADIENT,2,100,150,450,100,500);

	for ( int i =0 ; i<m_seq2->total ; i++)
	{
		float* ptr =(float*) cvGetSeqElem(m_seq2,i);
		CvPoint pt = cvPoint(cvRound(ptr[0]),cvRound(ptr[1]));
		cvCircle(image_BGR,pt,cvRound(ptr[2]),cvScalar(0,255),3);
	}

	cvCanny(m_image,m_image,80,180);  //霍夫直线变换之前做边缘检测
    CvMemStorage * m_storage_line = cvCreateMemStorage(0);
	CvSeq * m_seq = cvHoughLines2(m_image,m_storage_line,CV_HOUGH_PROBABILISTIC,1,CV_PI/180,240,100,10);
	
   for ( int i =0 ; i<m_seq->total ; i++)
   {
	   int * ptr =(int *) cvGetSeqElem(m_seq,i);
	   cvLine(image_BGR,cvPoint(ptr[0],ptr[1]),cvPoint(ptr[2],ptr[3]),cvScalar(255),3);
   }
	cvShowImage("Canny3_2",image_BGR);

	cvWaitKey(0);
    cvReleaseImage(&m_image);
    cvReleaseImage(&image_BGR);
    cvReleaseMemStorage(&m_storage_cirlce);
    cvReleaseMemStorage(&m_storage_line);
    cvDestroyWindow("Canny3_2");

	return 0;
}
```

![](https://img-blog.csdn.net/20141201194315265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVvc2hpeGlhbjA5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



```cpp
/* 6_10 */
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include "stdio.h"
#include <iostream>
#include "math.h"
using namespace std;
int main(int argc, char **argv)
{

	IplImage * m_image = cvLoadImage("lena.jpg",1);
    IplImage *image_dst =cvCreateImage(cvGetSize(m_image),8,3);
	IplImage *src_back =cvCreateImage(cvGetSize(m_image),8,3);
	cvLogPolar(m_image,image_dst,cvPoint2D32f(m_image->width/4,m_image->height/2),40,CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS);
	cvLogPolar(image_dst,src_back,cvPoint2D32f(m_image->width/4,m_image->height/2),40,CV_WARP_INVERSE_MAP|CV_INTER_LINEAR);
	cvShowImage("Polar",image_dst);
	cvShowImage("Inverse_back",src_back);
	cvWaitKey(0);
	cvReleaseImage(&src_back);
	cvReleaseImage(&m_image);
	cvReleaseImage(&image_dst);
	cvDestroyWindow("Polar");

	return 0;
}
```
![](https://img-blog.csdn.net/20141201204824835)
![](https://img-blog.csdn.net/20141201204858609)





```cpp
/* 6_12 */
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include "stdio.h"
#include <iostream>
#include "math.h"
using namespace std;
int main(int argc, char **argv)
{

	IplImage * m_image = cvLoadImage("compar.jpg",1);
    IplImage *image_dst =cvCreateImage(cvGetSize(m_image),8,3);
	IplImage *src_back =cvCreateImage(cvGetSize(m_image),8,3);
	cvLogPolar(m_image,image_dst,cvPoint2D32f(m_image->width/2,m_image->height/2),40,CV_INTER_LINEAR+CV_WARP_FILL_OUTLIERS);
	cvLogPolar(image_dst,src_back,cvPoint2D32f(m_image->width/2,m_image->height/2),40,CV_WARP_INVERSE_MAP|CV_INTER_LINEAR);
	cvShowImage("Polar",image_dst);
	cvShowImage("Inverse_back",src_back);
	cvWaitKey(0);
	cvReleaseImage(&src_back);
	cvReleaseImage(&m_image);
	cvReleaseImage(&image_dst);
	cvDestroyWindow("Polar");
	return 0;
}
```
![](https://img-blog.csdn.net/20141201211242000)

![](https://img-blog.csdn.net/20141201211301065)



```cpp
/* 6_16 */
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include "stdio.h"
#include <iostream>
#include "math.h"
using namespace std;
int main(int argc, char **argv)
{

	IplImage * m_image = cvLoadImage("example.jpg");
	IplImage *dst =cvCloneImage(m_image);
    IplImage *mag_R = cvCreateImage(cvSize(m_image->width,m_image->height),IPL_DEPTH_8U,1);
	IplImage *mag_G = cvCreateImage(cvSize(m_image->width,m_image->height),IPL_DEPTH_8U,1);
	IplImage *mag_B = cvCreateImage(cvSize(m_image->width,m_image->height),IPL_DEPTH_8U,1);
	cvSplit(m_image,mag_B,mag_G,mag_R,NULL);
    cvEqualizeHist(mag_B,mag_B);
	cvEqualizeHist(mag_R,mag_R);
	cvEqualizeHist(mag_G,mag_G);
	cvMerge(mag_B,mag_G,mag_R,NULL,dst);
	cvShowImage("Src",m_image);
	cvShowImage("Dst",dst);
	cvWaitKey(0);
	
	cvReleaseImage(&m_image);
	cvReleaseImage(&mag_B);
	cvReleaseImage(&mag_G);
	cvReleaseImage(&mag_R);
	cvReleaseImage(&dst);
	cvDestroyWindow("Src");
	cvDestroyWindow("Dst");
	return 0;
}
```

![](https://img-blog.csdn.net/20141201225445403?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVvc2hpeGlhbjA5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20141201225523781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVvc2hpeGlhbjA5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





```cpp
/* 6_17 */
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include "stdio.h"
#include <iostream>
#include "math.h"
using namespace std;
int main(int argc, char **argv)
{

	IplImage * m_image = cvLoadImage("example.jpg");
	IplImage *dst =cvCloneImage(m_image);
  
	CvPoint2D32f srcQuad[4],dstQuad[4];
	CvMat* matrix = cvCreateMat(3,3,CV_32FC1);
	srcQuad[0].x=0;
	srcQuad[0].y=0;
	srcQuad[1].x=m_image->width;
	srcQuad[1].y=0;
	srcQuad[2].x=0;
	srcQuad[2].y=m_image->height;
	srcQuad[3].x=m_image->width;
	srcQuad[3].y=m_image->height;

	dstQuad[0].x=m_image->width*0.7;
	dstQuad[0].y=m_image->height*0.6;
	dstQuad[1].x=m_image->width*0.5;
	dstQuad[1].y=m_image->height*0.9;
	dstQuad[2].x=m_image->width*0.3;
	dstQuad[2].y=m_image->height*0.2;
	dstQuad[3].x=m_image->width*0.1;
	dstQuad[3].y=m_image->height*0.8;
	cvGetPerspectiveTransform(srcQuad,dstQuad,matrix);
	cvWarpPerspective(m_image,dst,matrix);
	
	cvShowImage("Src",m_image);
	cvShowImage("DST",dst);
	cvWaitKey(0);

	cvReleaseImage(&m_image);
	cvReleaseImage(&dst);
	cvReleaseMat(&matrix);
	cvDestroyWindow("Src");
	cvDestroyWindow("Dst");
	return 0;
}
```

![](https://img-blog.csdn.net/20141201230851564?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVvc2hpeGlhbjA5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20141201230903046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVvc2hpeGlhbjA5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





