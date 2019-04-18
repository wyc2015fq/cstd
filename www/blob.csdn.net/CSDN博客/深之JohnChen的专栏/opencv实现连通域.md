# opencv实现连通域 - 深之JohnChen的专栏 - CSDN博客

2010年08月12日 18:03:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：10685


      在本文中使用图像连通域统计使用opencv中的cvFloodFill方法，但是在cvFloodFill方法中CvConnectedComp参数无法返回具体点坐标位置信息，找了些资料、给CvSeq分配空间但是还是contour内容没有值，估计是OPENCV2.00版本不支持。如果想获取点坐标信息有两种方法：一是通过源代码改写cvFloodFill函数来返回点坐标信息；二是cvFloodFill连通之后，根据返回区域统计多少个点，并统计之后对此点做标记，依次反复下一个连通区域。

   以下是方法二的代码实现：

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

 if(src==NULL  || !(nUnionColor==0||nUnionColor==1) || !(nUnionMode==1||nUnionMode==2) || (rectSrc.left==0 && rectSrc.right==0 || rectSrc.top==0 && rectSrc.bottom==0 || rectSrc.left>rectSrc.right || rectSrc.top>rectSrc.bottom))

 {

  nRet = -1;

  return nRet;

 }

 if(src->GetBpp()>8)

 {

  nRet = 0;

  return  nRet;

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

 CvConnectedComp dccTmp;

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

注：CxImage是一款优秀的图像操作类库。它可以快捷地存取、显示、转换各种图像。

下载地址:[http://www.codeproject.com/KB/graphics/cximage.aspx](http://www.codeproject.com/KB/graphics/cximage.aspx)

