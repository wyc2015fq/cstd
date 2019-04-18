# 在OpenCV中实现判断离散点 - 深之JohnChen的专栏 - CSDN博客

2010年07月22日 11:36:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3925


     如何判断离散点,利用递归算法统计连续点的个数，通过阈值来判定是否为离散点。opencv代码实现：

#include "cv.h"
#include "highgui.h"
#include <queue>

//点插入队列中
bool  flagpoint(POINT point,PointQue &que)
{

 for(int i=0;i<que.size();i++)
 {
  if(que[i].x==point.x && que[i].y==point.y)
  {
     return false;
  }
  else
  {
   que.push_back(point);
   return true;
  }

 }
}

//队列中是否有此点
bool HaveFindPoint(PointQue findque,int x,int y)
{

    for(int i=0;i<findque.size();i++)
    {
  if(findque[i].x==x && findque[i].y==y)
  {
   return false;
  }

    }
   return true;
}

 /* 
函数说明：判断离散点 [利用递归算法统计连续点的个数，通过阈值来判定是否为离散点]
目前只支持对2值图像进行处理 
参数说明：
psrc 图像  
lWidth,lHeight 图片的宽高
X，Y，当前的坐标点
pque收集到的点
m_lend点的个数，离散点的判定长度
m_number已经收集到的个数
返回值：bool类型；是离散点返回false 不是离散点返回true 
*/

bool GetNoisePoint(IplImage  *pSrc,RECT FindRect,int x,int y,PointQue  &pque,PointQue &FindQue,int m_lend,int m_number,bool bFirstPoint) 
{
  unsigned char *p = (unsigned char*)pSrc->imageData;
     #define PIX(x,y) p[pSrc->widthStep * y + x] 

     POINT point;

      m_number++;

   if(bFirstPoint)
   {  
    point.x=x;
       point.y=y;
    pque.push_back(point);
   }

  if(m_number>m_lend)
   return false;
  else
  {
           //考察上下左右以及左上、右上、左下、右下八个方向 
     //如果是黑色点，则调用函数自身进行递归 
     //考察下面点 
    ///上方///
   point.x=x;
   point.y=y-1;

   if(pque.size()>3&&pque[0].x==x&&pque[0].y==(y-1))
   {
    return true;///闭合区域，认为是杂点////
   }

   if((y-1>=FindRect.top)&&PIX(x,y-1)!=0&&(flagpoint(point,pque)))
     {
      GetNoisePoint(pSrc,FindRect,x,y,pque,FindQue,m_lend,m_number,false);
     }

   FindQue.push_back(point);

     //左上点 
     point.x=x-1;
     point.y=y-1;

     if(pque.size()>3&&pque[0].x==(x-1)&&pque[0].y==(y-1))
   {
    return true;///闭合区域，认为是杂点////
   }

     if((y-1>FindRect.top)&&(x-1>=FindRect.left)&&PIX(x-1,y-1)!=0&&(flagpoint(point,pque)))
     {
               GetNoisePoint(pSrc,FindRect, x-1, y-1,pque,FindQue,m_lend,m_number,false);

     }
     FindQue.push_back(point);

     //左边 

     point.x=x-1;
     point.y=y;

      if(pque.size()>3&&pque[0].x==(x-1)&&pque[0].y==(y))
   {
    return true;///闭合区域，认为是杂点////
   }

   if((x-1>=FindRect.left)&&PIX(x-1,y)!=0&&(flagpoint(point,pque)))
     {
               GetNoisePoint(pSrc, FindRect, x-1, y, pque,FindQue,m_lend,m_number,false);

     }
   FindQue.push_back(point);

     //左下边  
     point.x=x-1;
     point.y=y+1;

      if(pque.size()>3&&pque[0].x==(x-1)&&pque[0].y==(y+1))
   {
    return true;///闭合区域，认为是杂点////
   }

   if((x-1>FindRect.left)&&(y+1<FindRect.bottom)&&PIX(x-1,y+1)!=0&&(flagpoint(point,pque))) 
     {
               GetNoisePoint(pSrc, FindRect, x-1, y+1, pque,FindQue,m_lend,m_number,false);

     }
   FindQue.push_back(point);

     ///下面
      point.x=x;
   point.y=y+1;

      if(pque.size()>3&&pque[0].x==(x)&&pque[0].y==(y+1))
   {
    return true;///闭合区域，认为是杂点////
   }

   if((y+1<FindRect.bottom)&&PIX(x,y+1)!=0&&(flagpoint(point,pque))) 
     {
               GetNoisePoint(pSrc, FindRect, x, y+1,pque,FindQue,m_lend,m_number,false);

     }
   FindQue.push_back(point);

     //右下面   

     point.x=x+1;
     point.y=y+1;

      if(pque.size()>3&&pque[0].x==(x+1)&&pque[0].y==(y+1))
   {
    return true;///闭合区域，认为是杂点////
   }

   if((y+1<FindRect.bottom)&&(x+1<FindRect.right)&&PIX(x,y+1)!=0&&(flagpoint(point,pque)))
     {
               GetNoisePoint(pSrc,FindRect, x+1, y+1,pque,FindQue,m_lend,m_number,false);

     }
   FindQue.push_back(point);

  //右边 

      point.x=x+1;
      point.y=y;

      if(pque.size()>3&&pque[0].x==(x+1)&&pque[0].y==(y))
   {
    return true;///闭合区域，认为是杂点////
   }

   if((x+1<FindRect.right)&&PIX(x+1,y)!=0&&(flagpoint(point,pque)))
     {
               GetNoisePoint(pSrc, FindRect ,x+1, y, pque,FindQue,m_lend,m_number,false);

     }
   FindQue.push_back(point);

  //右上///

  point.x=x+1;
  point.y=y-1;

      if(pque.size()>3&&pque[0].x==(x+1)&&pque[0].y==(y-1))
   {
    return true;///闭合区域，认为是杂点////
   }

   if((x+1<FindRect.right)&&(y-1>FindRect.top)&&PIX(x+1,y-1)!=0&&(flagpoint(point,pque)))
     {
              GetNoisePoint(pSrc,FindRect, x+1, y-1, pque,FindQue,m_lend,m_number,false);

     }

  }
  FindQue.push_back(point);

 //如果递归结束，返回false，说明是离散点 
 return FALSE; 

}

