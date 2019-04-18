# OpenCV实现连通域 - wishchinYang的专栏 - CSDN博客
2015年09月15日 15:14:17[wishchin](https://me.csdn.net/wishchin)阅读数：1040
        借此代码实现连通域，原文链接：[http://blog.csdn.net/augusdi/article/details/8865694](http://blog.csdn.net/augusdi/article/details/8865694)
        如有疑问或者版权问题，请移步原作者或者告知本人。
        比较详细的其他方法，如 一般邻域遍历方法 或者 **两遍扫描法**，可在这篇文章中找到：
[OpenCV连通域分析与标记](http://www.360doc.com/content/13/1106/15/10724725_327170263.shtml)
1. 借用的代码：
```cpp
//连通区域结构  
    typedef struct MyConnectedComp  
    {  
     double area;  
     int value;  
     RECT rect;  
     list<POINT> listAllPoint;  
    }MyConnectedComp  
    /* 
    *功能说明：统计图像的连通区域 
    *参数说明：src,表示原始图象；rectSrc，表示图像区域；listData，表示连通域结构链表；nUnionColor，表示连通颜色(0,表示黑点；1，表示白点)；nUnionMode，表示连通类型（0，表示4连通；1，表示8连通） 
    *返回值：int类型。-1，表示参数错误；0，表示非二值/灰度图像 
    */  
    int ImageUnionByFloodFill(CxImage *src,RECT rectSrc,list<MyConnectedComp> &listData,int nUnionColor,int nUnionMode)  
    {  
     int nRet = 1;  
     //参数检测  
     if(src==NULL  || !(nUnionColor==0||nUnionColor==1) || !(nUnionMode==1||nUnionMode==2) || (rectSrc.left==0 && rectSrc.right==0 || rectSrc.top==0 && rectSrc.bottom==0 || rectSrc.left>rectSrc.right || rectSrc.top>rectSrc.bottom))  
     {  
      nRet = -1;  
      return nRet;  
     }  
     if(src->GetBpp()>8)  
     {  
      nRet = 0;  
      return  nRet;  
     }  
     listData.clear();  
     int nColor = 255;  
     if(nUnionColor==0)  
     {  
      nUnionColor = 255;  
     }  
     if(nUnionColor==1)  
     {  
      nUnionColor = 0;  
     }  
     int nFlag = 4;  
     if(nUnionMode==1)  
     {  
      nFlag = 4;  
     }  
     if(nUnionMode==2)  
     {  
      nFlag = 8;  
     }  
     int iBackColor = GetBlackColor(*src);  
     long i = 0,j = 0;  
     long m = 0,n = 0,newN = 0,newM = 0;  
     long nWidth = 0,nHeight = 0;  
      
     IplImage *pcvImageTmp = NULL;  
     if(!Cximage2IplImage(src,&pcvImageTmp))  
     {  
      return 0;  
     }  
     //如果是灰度图像，做二值化处理  
     if(src->GetBpp()!=1)  
     {  
      cvThreshold(pcvImageTmp,pcvImageTmp,128,255,CV_THRESH_BINARY);  
     }  
     CvSize size = cvGetSize(pcvImageTmp);  
     DatumnConnectedComp dccTmp;  
     CvConnectedComp comp;  
     CvPoint seed;  
     for(j=rectSrc.top;j<rectSrc.bottom;j++)  
     {  
      for(i=rectSrc.left;i<rectSrc.right;i++)  
      {  
       if(i<0 || i>=size.width || j<0 || j>=size.height)  
       {  
        continue;  
       }  
       uchar *p = (uchar *)&(pcvImageTmp->imageData+j*pcvImageTmp->widthStep)[i];   
       if(*p==nUnionColor || *p==100 || *p==101)  
       {  
        continue;  
       }  
       //处理为黑点，做连通  
       seed = cvPoint(i,j);  
       cvFloodFill(pcvImageTmp, seed,cvScalarAll(100), cvScalarAll(0),cvScalarAll(0), &comp, nFlag, 0 );     
       dccTmp.area = comp.area;  
       dccTmp.rect.left = comp.rect.x;  
       dccTmp.rect.top = comp.rect.y;  
       dccTmp.rect.right = comp.rect.x + comp.rect.width;  
       dccTmp.rect.bottom = comp.rect.y + comp.rect.height;  
       dccTmp.value = int(comp.value.val[1]);  
       dccTmp.listAllPoint.clear();  
       POINT ptInsert;  
       for(n=0;n<=comp.rect.height;n++)  
       {  
        for(m=0;m<comp.rect.width;m++)  
        {  
         newN = n + comp.rect.y;  
         newM = m + comp.rect.x;  
         if(newN>=size.height || newM>=size.width)  
         {  
          continue;  
         }  
         uchar *q = (uchar *)&(pcvImageTmp->imageData+newN*pcvImageTmp->widthStep)[newM];   
         if(*q==100)  
         {  
          ptInsert.x = newM;  
          ptInsert.y = newN;  
          dccTmp.listAllPoint.push_back(ptInsert);  
          *q = 101;//改变标记,说明已被连通  
         }  
        }  
       }  
       listData.push_back(dccTmp);  
      }  
     }  
     cvReleaseImage(&pcvImageTmp);  
     return nRet;  
    }
```

