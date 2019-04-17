# 学习opencv 习题答案-第五章 - 勿在浮砂筑高台 - CSDN博客





2014年11月26日 13:22:25[勿在浮砂筑高台](https://me.csdn.net/luoshixian099)阅读数：1247








```cpp
/* 5_5     我放置了一包烟 */ 
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include "stdio.h"
using namespace std;
int main(int argc, char **argv)
{
	
	IplImage * m_image1 = cvLoadImage(argv[1],CV_LOAD_IMAGE_GRAYSCALE|CV_LOAD_IMAGE_ANYCOLOR|CV_LOAD_IMAGE_ANYDEPTH);
	IplImage * m_image2 = cvLoadImage(argv[2],CV_LOAD_IMAGE_GRAYSCALE|CV_LOAD_IMAGE_ANYCOLOR|CV_LOAD_IMAGE_ANYDEPTH);
	IplImage *Sub=cvCreateImage(cvGetSize(m_image1),m_image1->depth,m_image1->nChannels);
    IplImage *Sub_Smooth =cvCreateImage(cvGetSize(m_image1),m_image1->depth,m_image1->nChannels);
	IplImage *S_Open =cvCreateImage(cvGetSize(m_image1),m_image1->depth,m_image1->nChannels);
	cvAbsDiff(m_image1,m_image2,Sub);  
	cvThreshold(Sub,Sub_Smooth,140.0,255,CV_THRESH_BINARY);
	cvMorphologyEx(Sub_Smooth,S_Open,NULL,NULL,CV_MOP_OPEN);
	cvNamedWindow("Sub_Smooth");
	cvNamedWindow("Sub_Open");	
	cvShowImage("Sub_Smooth",Sub_Smooth);
	cvShowImage("Sub_Open",S_Open);
    
	cvWaitKey(0);
	cvReleaseImage(&m_image1);
	cvReleaseImage(&m_image2);
	cvReleaseImage(&Sub);
	cvDestroyWindow("Sub_Smooth");
	cvDestroyWindow("Sub_Open");

	return 0;
}
```

![](https://img-blog.csdn.net/20141126150922932?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVvc2hpeGlhbjA5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




```cpp
/* 5_6 */
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include "stdio.h"
#include <iostream>
#include "math.h"
using namespace std;

int main(int argc, char **argv)
{
	
	IplImage * m_image1 = cvLoadImage(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
	IplImage * m_image2 = cvLoadImage(argv[2],CV_LOAD_IMAGE_GRAYSCALE);
	IplImage *Sub=cvCreateImage(cvGetSize(m_image1),m_image1->depth,m_image1->nChannels);
    IplImage *Pit_Binary =cvCreateImage(cvGetSize(m_image1),m_image1->depth,m_image1->nChannels);
	IplImage *Pit_Open =cvCreateImage(cvGetSize(m_image1),m_image1->depth,m_image1->nChannels);
	cvAbsDiff(m_image1,m_image2,Sub);  
	cvThreshold(Sub,Pit_Binary,10.0,255,CV_THRESH_BINARY);
	cvMorphologyEx(Pit_Binary,Pit_Open,NULL,NULL,CV_MOP_OPEN);
	cvNamedWindow("Binary");
	cvNamedWindow("Photo_Open");	
	CvConnectedComp m_ConnetComp={0};   //保存填充属性
   uchar * ptr =  cvPtr2D(Pit_Open,0,0,NULL);

   CvPoint m_point = cvPoint(0,0);                  //保存填充的种子点
   double m_area=0.0;                                    //记录填充的区域面积
	for (int i =0; i <Pit_Open->height ;i++)
	{
          for (int j=0 ;j <Pit_Open->width ;j++)
          {
			 ptr =cvPtr2D(Pit_Open,0,0,NULL)+ i*(Pit_Open->widthStep)+j;
      
			 if ((int)(*ptr) == 255)
			 {
		   cvFloodFill(Pit_Open,cvPoint(j,i),cvScalar(100),cvScalarAll(0),cvScalarAll(0),&m_ConnetComp,(8|0<<8),NULL);     
		     if (m_ConnetComp.area > m_area)
		     {
				 cvFloodFill(Pit_Open,cvPoint(m_point.x,m_point.y),cvScalar(0,0,0),cvScalarAll(0),cvScalarAll(0),NULL,(8|0<<8),NULL); //把之前的填充区域的清除
				 m_point.x =j;                               
				 m_point.y =i;
				 m_area = m_ConnetComp.area;
		     }
			 else
			 {
			  cvFloodFill(Pit_Open,cvPoint(j,i),cvScalar(0,0,0),cvScalarAll(0),cvScalarAll(0),NULL,(8|0<<8),NULL);   //清除此次填充
			 }
			 }
          }
	}
    cout <<m_area;   //输出最大的填充面积
```

```cpp
cvShowImage("Binary",Pit_Binary);
	cvShowImage("Photo_Open",Pit_Open);

	cvWaitKey(0);
	cvReleaseImage(&m_image1);
	cvReleaseImage(&m_image2);
	cvReleaseImage(&Sub);
	cvDestroyWindow("Binary");
	cvDestroyWindow("Photo_Open");

	return 0;
}
```

```cpp
/* 5_7 */
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include "stdio.h"
#include <iostream>
#include "math.h"
using namespace std;

int main(int argc, char **argv)
{

	IplImage * m_image1 = cvLoadImage(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
	IplImage * m_image2 = cvLoadImage(argv[2],CV_LOAD_IMAGE_GRAYSCALE);
	IplImage *Sub=cvCreateImage(cvGetSize(m_image1),m_image1->depth,m_image1->nChannels);
	IplImage *Pit_Binary =cvCreateImage(cvGetSize(m_image1),m_image1->depth,m_image1->nChannels);
	IplImage *Pit_Open =cvCreateImage(cvGetSize(m_image1),m_image1->depth,m_image1->nChannels);
	cvAbsDiff(m_image1,m_image2,Sub);  
	cvThreshold(Sub,Pit_Binary,10.0,255,CV_THRESH_BINARY);
	cvMorphologyEx(Pit_Binary,Pit_Open,NULL,NULL,CV_MOP_OPEN);
	cvNamedWindow("Binary");
	cvNamedWindow("Photo_Open");	
	CvConnectedComp m_ConnetComp={0};   //保存填充属性
	uchar * ptr =  cvPtr2D(Pit_Open,0,0,NULL);

	CvPoint m_point = cvPoint(0,0);                  //保存填充的种子点
	double m_area=0.0;                                    //记录填充的区域面积
	for (int i =0; i <Pit_Open->height ;i++)
	{
		for (int j=0 ;j <Pit_Open->width ;j++)
		{
			ptr =cvPtr2D(Pit_Open,0,0,NULL)+ i*(Pit_Open->widthStep)+j;

			if ((int)(*ptr) == 255)
			{
				cvFloodFill(Pit_Open,cvPoint(j,i),cvScalar(100),cvScalarAll(0),cvScalarAll(0),&m_ConnetComp,(8|0<<8),NULL);     
				if (m_ConnetComp.area > m_area)
				{
					cvFloodFill(Pit_Open,cvPoint(m_point.x,m_point.y),cvScalar(0,0,0),cvScalarAll(0),cvScalarAll(0),NULL,(8|0<<8),NULL); //把之前的填充区域的清除
					m_point.x =j;                               
					m_point.y =i;
					m_area = m_ConnetComp.area;
				}
				else
			 {
				 cvFloodFill(Pit_Open,cvPoint(j,i),cvScalar(0,0,0),cvScalarAll(0),cvScalarAll(0),NULL,(8|0<<8),NULL);   //清除此次填充
			 }
			}
		}
	}
	cout <<m_area;


	IplImage *pic_mix =cvLoadImage(argv[3]);     //载入没有香烟的图像
	cvShowImage("sdf",pic_mix);
	IplImage *pic_src =cvLoadImage(argv[1]);      //载入带有香烟的图像
	cvCopy(pic_src,pic_mix,Pit_Open);   
	cvNamedWindow("Result");
	cvShowImage("Result",pic_mix);
	cvShowImage("Binary",Pit_Binary);
	cvShowImage("Photo_Open",Pit_Open);

	cvWaitKey(0);
	cvReleaseImage(&pic_mix);
	cvReleaseImage(&pic_src);
	cvReleaseImage(&m_image1);
	cvReleaseImage(&m_image2);
	cvReleaseImage(&Sub);
	cvDestroyWindow("Result");
	cvDestroyWindow("Binary");
	cvDestroyWindow("Photo_Open");

	return 0;
}
```

![](https://img-blog.csdn.net/20141126133836089)![](https://img-blog.csdn.net/20141126133903140)![](https://img-blog.csdn.net/20141126134728112?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVvc2hpeGlhbjA5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



```cpp
</pre><p></p><p><pre name="code" class="cpp">/* 5_9 */
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
	IplImage *m_image1=cvCloneImage(m_image);
    cvMorphologyEx(m_image,m_image1,NULL,NULL,CV_MOP_TOPHAT);
	IplImage *pict_mask = cvCreateImage(cvGetSize(m_image1),IPL_DEPTH_8U,1);
	cvThreshold(m_image1,pict_mask,30,255,CV_THRESH_BINARY);
	cvCopy(pict_mask,m_image1,pict_mask);
	cvShowImage("Picture_RGB",m_image);
	cvShowImage("Picture_Grey",m_image1);
	cvShowImage("Mask",pict_mask);
	cvWaitKey(0);
	cvDestroyWindow("Picture_RGB");
	cvDestroyWindow("Picture_Grey");
	cvReleaseImage(&m_image);
    cvReleaseImage(&m_image1);
	cvReleaseImage(&pict_mask);
	return 0;
}
```
![](https://img-blog.csdn.net/20141126151439706?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVvc2hpeGlhbjA5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20141126151416406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVvc2hpeGlhbjA5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





```cpp
/* 5_10 */
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
	IplImage *m_image1=cvCreateImage(cvSize(m_image->width/2,m_image->height/2),m_image->depth,m_image->nChannels);
	IplImage *m_image2=cvCreateImage(cvSize(m_image1->width/2,m_image1->height/2),m_image1->depth,m_image1->nChannels);
	IplImage *m_image3=cvCreateImage(cvSize(m_image2->width/2,m_image2->height/2),m_image2->depth,m_image2->nChannels);

	IplImage *m_down1=cvCreateImage(cvSize(m_image->width/2,m_image->height/2),m_image->depth,m_image->nChannels);
	IplImage *m_down2=cvCreateImage(cvSize(m_image1->width/2,m_image1->height/2),m_image1->depth,m_image1->nChannels);
	IplImage *m_down3=cvCreateImage(cvSize(m_image2->width/2,m_image2->height/2),m_image2->depth,m_image2->nChannels);
    cvResize(m_image,m_image1);
	cvResize(m_image1,m_image2);
	cvResize(m_image2,m_image3);
    
  cvPyrDown(m_image,m_down1);
  cvPyrDown(m_down1,m_down2);
  cvPyrDown(m_down2,m_down3);

	cvShowImage("Down3",m_down3);
	cvShowImage("Picture_RGB",m_image);
	cvShowImage("Resize3",m_image3);

	cvWaitKey(0);
	cvDestroyWindow("Down3");
	cvDestroyWindow("Picture_RGB");
	cvDestroyWindow("Resize3");	
	cvReleaseImage(&m_image);
	cvReleaseImage(&m_image1);
	cvReleaseImage(&m_image2);
    cvReleaseImage(&m_image3);
	cvReleaseImage(&m_down1);
	cvReleaseImage(&m_down2);
	cvReleaseImage(&m_down3);

	return 0;
}
```
![](https://img-blog.csdn.net/20141126155302848?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVvc2hpeGlhbjA5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20141126155309384?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHVvc2hpeGlhbjA5OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




