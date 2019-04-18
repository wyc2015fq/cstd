# opencv实现二值图像细化的算法 - 深之JohnChen的专栏 - CSDN博客

2010年06月02日 16:40:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：25118


opencv实现二值图像细化的算法

细化算法通常和骨骼化、骨架化算法是相同的意思，也就是thin算法或者skeleton算法。虽然很多图像处理的教材上不是这么写的，具体原因可以看这篇论文，Louisa Lam, Seong-Whan Lee, Ching Y. Suen，“Thinning Methodologies-A Comprehensive Survey ”，IEEE TRANSACTIONS ON PATTERN ANALYSIS AND MACHINE INTELLIGENCE, VOL. 14, NO. 9, SEPTEMBER 1992 ，总结了几乎所有92年以前的经典细化算法。

函数：void cvThin( IplImage* src, IplImage* dst, int iterations=1)
功能：将IPL_DEPTH_8U型二值图像进行细化
参数：src，原始IPL_DEPTH_8U型二值图像
dst，目标存储空间，必须事先分配好，且和原图像大小类型一致
iterations，迭代次数
参考文献：T. Y. Zhang and C. Y. Suen, “A fast parallel algorithm for thinning digital patterns,” Comm. ACM, vol. 27, no. 3, pp. 236-239, 1984. 

void cvThin( IplImage* src, IplImage* dst, int iterations=1)
{
 CvSize size = cvGetSize(src);

 cvCopy(src, dst);
    int n = 0,i = 0,j = 0;
 for(n=0; n<iterations; n++)
 {
  IplImage* t_image = cvCloneImage(dst);
  for(i=0; i<size.height;  i++)
  {
   for(j=0; j<size.width; j++)
   {
    if(CV_IMAGE_ELEM(t_image,byte,i,j)==1)
    {
     int ap=0;
     int p2 = (i==0)?0:CV_IMAGE_ELEM(t_image,byte, i-1, j);
     int p3 = (i==0 || j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte, i-1, j+1);
     if (p2==0 && p3==1)
     {
      ap++;
     }
     int p4 = (j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte,i,j+1);
     if(p3==0 && p4==1)
     {
      ap++;
     }
     int p5 = (i==size.height-1 || j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j+1);
     if(p4==0 && p5==1)
     {
      ap++;
     }
     int p6 = (i==size.height-1)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j);
     if(p5==0 && p6==1)
     {
      ap++;
     }
     int p7 = (i==size.height-1 || j==0)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j-1);
     if(p6==0 && p7==1)
     {
      ap++;
     }
     int p8 = (j==0)?0:CV_IMAGE_ELEM(t_image,byte,i,j-1);
     if(p7==0 && p8==1)
     {
      ap++;
     }
     int p9 = (i==0 || j==0)?0:CV_IMAGE_ELEM(t_image,byte,i-1,j-1);
     if(p8==0 && p9==1)
     {
      ap++;
     }
     if(p9==0 && p2==1)
     {
      ap++;
     }
     if((p2+p3+p4+p5+p6+p7+p8+p9)>1 && (p2+p3+p4+p5+p6+p7+p8+p9)<7)
     {
      if(ap==1)
      {
       if(!(p2 && p4 && p6))
       {
        if(!(p4 && p6 && p8)) 
        {
         CV_IMAGE_ELEM(dst,byte,i,j)=0;
        }
       }
      }
     }

    }
   }
  }
  cvReleaseImage(&t_image);
  t_image = cvCloneImage(dst);
  for(i=0; i<size.height;  i++)
  {
   for(int j=0; j<size.width; j++)
   {
    if(CV_IMAGE_ELEM(t_image,byte,i,j)==1)
    {
     int ap=0;
     int p2 = (i==0)?0:CV_IMAGE_ELEM(t_image,byte, i-1, j);
     int p3 = (i==0 || j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte, i-1, j+1);
     if (p2==0 && p3==1)
     {
      ap++;
     }
     int p4 = (j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte,i,j+1);
     if(p3==0 && p4==1)
     {
      ap++;
     }
     int p5 = (i==size.height-1 || j==size.width-1)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j+1);
     if(p4==0 && p5==1)
     {
      ap++;
     }
     int p6 = (i==size.height-1)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j);
     if(p5==0 && p6==1)
     {
      ap++;
     }
     int p7 = (i==size.height-1 || j==0)?0:CV_IMAGE_ELEM(t_image,byte,i+1,j-1);
     if(p6==0 && p7==1)
     {
      ap++;
     }
     int p8 = (j==0)?0:CV_IMAGE_ELEM(t_image,byte,i,j-1);
     if(p7==0 && p8==1)
     {
      ap++;
     }
     int p9 = (i==0 || j==0)?0:CV_IMAGE_ELEM(t_image,byte,i-1,j-1);
     if(p8==0 && p9==1)
     {
      ap++;
     }
     if(p9==0 && p2==1)
     {
      ap++;
     }
     if((p2+p3+p4+p5+p6+p7+p8+p9)>1 && (p2+p3+p4+p5+p6+p7+p8+p9)<7)
     {
      if(ap==1)
      {
       if(p2*p4*p8==0)
       {
        if(p2*p6*p8==0)
        {
         CV_IMAGE_ELEM(dst, byte,i,j)=0;
        }
       }
      }
     }                    
    }

   }

  }            
  cvReleaseImage(&t_image);
 }

}

//使用举例

#include "cxcore.h"
#include "cv.h"
#include "highgui.h"

int main(int argc, char* argv[])
{
 if(argc!=2)
 {
  return 0;
 }
 IplImage *pSrc = NULL,*pDst = NULL,*pTmp = NULL;

//传入一个灰度图像
 pSrc = cvLoadImage(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
 if(!pSrc)
 {
  return 0;
 }
 pTmp = cvCloneImage(pSrc);
    pDst = cvCreateImage(cvGetSize(pSrc),pSrc->depth,pSrc->nChannels);
 cvZero(pDst);
 cvThreshold(pSrc,pTmp,128,1,CV_THRESH_BINARY_INV);//做二值处理，将图像转换成0，1格式
 //cvSaveImage("c://Threshold.bmp",pTmp,0);
 cvThin(pTmp,pDst,8);//细化，通过修改iterations参数进一步细化
 cvNamedWindow("src",1);
 cvNamedWindow("dst",1);
 cvShowImage("src",pSrc);
 //将二值图像转换成灰度，以便显示
 int i = 0,j = 0;
 CvSize size = cvGetSize(pDst);
 for(i=0; i<size.height;  i++)
 {
  for(j=0; j<size.width; j++)
  {
   if(CV_IMAGE_ELEM(pDst,uchar,i,j)==1)
   {
    CV_IMAGE_ELEM(pDst,uchar,i,j) = 0;
   }
   else
   {
    CV_IMAGE_ELEM(pDst,uchar,i,j) = 255;
   }
  }
 }
 //cvSaveImage("c://thin.bmp",pDst);
 cvShowImage("dst",pDst);
 cvWaitKey(0);
    cvReleaseImage(&pSrc);
 cvReleaseImage(&pDst);
 cvReleaseImage(&pTmp);
 cvDestroyWindow("src");
 cvDestroyWindow("dst");
 return 0;
} 

