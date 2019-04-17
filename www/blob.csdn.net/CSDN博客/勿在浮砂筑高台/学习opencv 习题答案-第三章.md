# 学习opencv 习题答案-第三章 - 勿在浮砂筑高台 - CSDN博客





2014年11月23日 22:57:08[勿在浮砂筑高台](https://me.csdn.net/luoshixian099)阅读数：716








**VS2008 Win32控制台应用程序  编译通过**
第三章
**3.2**


#include "stdafx.h"


#include "cv.h"

#include "highgui.h"

/*                 3_2                */

int _tmain(int argc, _TCHAR* argv[])

{

CvMat  *mat = cvCreateMat(100,100,CV_8SC3);

cvSetZero(mat);

cvCircle(mat,cvPoint(50,50),40,CV_RGB(255,0,0));

cvNamedWindow("ss",CV_WINDOW_AUTOSIZE);

cvShowImage("ss",mat);

cvWaitKey(0);

return 0;

}









/* 3_3 */








int _tmain(int argc, _TCHAR* argv[])

{

CvMat  *mat = cvCreateMat(100,100,CV_8UC3);            

cvSetZero(mat);

//cvSet(mat,cvScalar(255,255,255));                                     //cvScalar（R,G,B,A）

//cvCircle(mat,cvPoint(50,50),40,CV_RGB(255,0,0));







for (int i=20 ; i<=40 ; i++)

{

uchar * ptr =cvPtr2D(mat,5,i);   //BGR顺序存      cvPtr2D(y,x)

*(ptr+1) = 255;

}

for (int i=20 ; i<=40 ; i++)

{

uchar * ptr =cvPtr2D(mat,20,i);   //BGR顺序存      cvPtr2D(y,x)

*(ptr+1) = 255;

}

for (int i=5 ; i<=20 ; i++)

{

uchar * ptr =cvPtr2D(mat,i,20);   //BGR顺序存      cvPtr2D(y,x)

*(ptr+1) = 255;

}

for (int i=5 ; i<=20 ; i++)

{

uchar * ptr =cvPtr2D(mat,i,40);   //BGR顺序存      cvPtr2D(y,x)

*(ptr+1) = 255;

}

cvNamedWindow("ss",CV_WINDOW_AUTOSIZE);

cvShowImage("ss",mat);

cvWaitKey(0);

return 0;

}












/* 3_4 */








int _tmain(int argc, _TCHAR* argv[])

{

CvMat  *mat = cvCreateMat(100,100,CV_8UC3);            

cvSetZero(mat);

//cvSet(mat,cvScalar(255,255,255));                                     //cvScalar（R,G,B,A）

//cvCircle(mat,cvPoint(50,50),40,CV_RGB(255,0,0));




for (int x=20;x<=40;x++)

{

for (int y=5;y<=20;y++)

{

        uchar * ptr =cvPtr2D(mat,y,x);   //BGR顺序存      cvPtr2D(y,x)

        *(ptr+1) = 255;

}



}



cvNamedWindow("ss",CV_WINDOW_AUTOSIZE);

cvShowImage("ss",mat);

cvWaitKey(0);

return 0;

}










**/* 3_6 */**




#include "stdafx.h"

#include "cv.h"

#include "highgui.h"

int  main(int argc, char ** argv)

{

//CvMat  *mat = cvCreateMat(210,210,CV_8UC1);            

//cvSetZero(mat);

//cvSet(mat,cvScalar(255,255,255));                                     //cvScalar（R,G,B,A）

//cvCircle(mat,cvPoint(50,50),40,CV_RGB(255,0,0));

    IplImage *m_imag = cvLoadImage(argv[1]);

IplImage *m_imag1= cvCreateImage(cvSize(20,30),m_imag->depth,m_imag->nChannels);

m_imag1->origin = m_imag->origin ;

m_imag1->widthStep =m_imag->widthStep;

//m_imag1->imageData = m_imag->imageData +m_imag->widthStep*10 + m_imag->nChannels*5 ;    //(5,10)

m_imag1->imageData =(char *)cvPtr2D(m_imag,10,5);




IplImage *m_imag2= cvCreateImage(cvSize(20,30),m_imag->depth,m_imag->nChannels);

m_imag2->origin = m_imag->origin ;

m_imag2->widthStep =m_imag->widthStep;

//m_imag2->imageData = m_imag->imageData +m_imag->widthStep*60 + m_imag->nChannels*50 ;//(50,60)

m_imag2->imageData =(char *)cvPtr2D(m_imag,60,50);

cvNot(m_imag1,m_imag1);

cvNot(m_imag2,m_imag2);

cvNamedWindow("Picture",CV_WINDOW_AUTOSIZE);

cvShowImage("Picture",m_imag);

cvWaitKey(0);

cvReleaseImage(&m_imag);

cvDestroyWindow("Picture");

return 0;

}





**/* 3_7*/**


#include "stdafx.h"

#include "cv.h"

#include "highgui.h"

int  main(int argc, char ** argv)

{



    IplImage *m_imag = cvLoadImage(argv[1],CV_LOAD_IMAGE_ANYCOLOR|CV_LOAD_IMAGE_ANYDEPTH);

IplImage *m_imag1= cvCreateImage(cvSize(m_imag->width,m_imag->height),m_imag->depth,1);

cvSplit(m_imag,NULL,m_imag1,NULL,NULL);




cvNamedWindow("Green",CV_WINDOW_AUTOSIZE);

cvShowImage("Green",m_imag1);



    IplImage *clone1 = cvCloneImage(m_imag1);

IplImage *clone2 = cvCloneImage(m_imag1);



double max,min;

cvMinMaxLoc(clone1,&min,&max);

unsigned char  thresh=(unsigned char)((max-min)/2.0);

cvSet(clone1,cvScalar( thresh));



   cvSetZero(clone2);

   cvCmp(m_imag1,clone1,clone2,CV_CMP_GE);




   cvSubS(m_imag1,cvScalar(thresh/2),m_imag1,clone2);




   cvNamedWindow("Out",CV_WINDOW_AUTOSIZE);

   cvShowImage("Out",m_imag1);




cvWaitKey(0);

cvReleaseImage(&m_imag1);

cvDestroyWindow("Picture");

return 0;

}







#include "stdafx.h"

#include "cv.h"

#include "highgui.h"

**/* 3_8         读取数据那块有问题，求指教*/**

struct  My_struct

{

   CvPoint  m_point;

   CvRect   m_rect;

};

void write_my_struct(CvFileStorage *fs ,const char * name , My_struct *ms)

{

   cvStartWriteStruct(fs,name,CV_NODE_SEQ);

   for ( int i =0; i<5 ;i++)

   {

      cvWriteInt(fs,0,(ms+i)->m_point.x);

      cvWriteInt(fs,0,(ms+i)->m_point.y);

  cvWriteInt(fs,0,(ms+i)->m_rect.x);

  cvWriteInt(fs,0,(ms+i)->m_rect.y);

  cvWriteInt(fs,0,(ms+i)->m_rect.width);

  cvWriteInt(fs,0,(ms+i)->m_rect.height);

   }



   cvEndWriteStruct(fs);

}

void read_my_struct(CvFileStorage *fs,CvFileNode *ms_node , My_struct *ms)

{



      ms->m_point.x = cvReadInt(ms_node);

  ms->m_point.y = cvReadInt(ms_node);

  ms->m_rect.x = cvReadInt(ms_node);

  ms->m_rect.y = cvReadInt(ms_node);

      ms->m_rect.width = cvReadInt(ms_node);

      ms->m_rect.height= cvReadInt(ms_node);

}

int  main(int argc, char ** argv)

{

My_struct example[5]={{cvPoint(1,1),cvRect(1,1,2,2)},{cvPoint(2,2),cvRect(2,2,3,3)}

                                  ,{cvPoint(3,3),cvRect(3,3,4,4)},{cvPoint(4,4),cvRect(4,4,5,5)},{cvPoint(5,5),cvRect(5,5,6,6)}};

     CvFileStorage * fs= cvOpenFileStorage("cfg.xml",0,CV_STORAGE_WRITE);

 My_struct *ptr1=&example[0];

     write_my_struct(fs,"my_struct",ptr1);

     cvReleaseFileStorage(&fs);






     CvFileStorage * fb= cvOpenFileStorage("cfg.xml",0,CV_STORAGE_READ);

     CvSeq * p = cvGetFileNodeByName(fb,NULL,"my_struct")->data.seq;

 My_struct ptr={0};

 CvFileNode *ms_node1 = (CvFileNode *)(cvGetSeqElem(p,0));

 read_my_struct(fb,ms_node1,&ptr);

 cvReleaseFileStorage(&fs);

return 0;

}




