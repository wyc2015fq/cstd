# 通过hough变换检测直线 - 深之JohnChen的专栏 - CSDN博客

2009年05月19日 09:44:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：5120


/*
函数功能：通过hough变换检测直线
参数说明：imgBinaryIn，表示二值图象
    width，表示图象宽
    height，表示图象高
    houghBuf，表示hough变换需要的缓冲区指针
    houghWidth，表示hough变换需要的缓冲区的宽
    houghHeight，表示hough变换需要的缓冲区的高
    radiusResolution，表示hough变换的极坐标半径的检测分辨率
    angleResolution，表示hough变换的角度检测分辨率
    radius，表示返回hough变换检测出来的最长直线的极半径
    angle，表示返回hough变换检测出来的最长直线的角度
*/
void ImageAlgorithm::HoughTransForm(unsigned char * imgBinaryIn,int width,int height,int *houghBuf,int houghWidth,int houghHeight,float radiusResolution,float angleResolution,float *radius,float *angle)
{
   int i,j;
   for(i=0;i<houghHeight;i++)
   {
    for (j=0;j<houghWidth;j++)
    {
     *(houghBuf+i*houghWidth+j) = 0;
    }
   }

   int r,a;
   float tempR,tempA;
   //遍历图象数据
   for(i=0;i<height;i++)
   {
  for(j=0;j<width;j++)
  {
   if(*(imgBinaryIn+i*width+j)==1)
   {
            //a代表角度的循环变量，在变换空间累加数组的垂直方向上
    for(a=0;a<houghHeight;a++)
    {
     //按照给定变换角度的分辨率，求取角度
     tempA = (a-houghHeight/2)*angleResolution;
     //根据当前遍历的角度及x,y值求取对应极坐标
     tempR = (j-width/2)*cos(tempA*2*PI/360) + (i-height/2)*sin(tempA*2*PI/360);
     r = tempR/radiusResolution;
     //累加数组
     *(houghBuf+a*houghWidth+r+houghWidth/2)+=1;
    }
   }
  }
   }

   //求累加数组的极大值，并记录此时的数组坐标
   int max,maxR,maxA;
   max = *(houghBuf+0*houghWidth+0);
   maxR = 0;
   maxA = 0;
   for(a=0;a<houghHeight;a++)
   {
    for(r=0;r<houghWidth;r++)
    {
     if(max<=*(houghBuf+a*houghWidth+r))
     {
      max = *(houghBuf+a*houghWidth+r);
      maxR = r;
      maxA = a;
     }
    }
   }

   //将极大值位置转换成极坐标半径和角度，并通过参数返回
   *radius = (maxR-houghWidth/2)*radiusResolution;
   *angle = (maxA-houghHeight/2)*angleResolution;

}

