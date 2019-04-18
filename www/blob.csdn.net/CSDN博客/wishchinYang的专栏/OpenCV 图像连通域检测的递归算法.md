# OpenCV: 图像连通域检测的递归算法 - wishchinYang的专栏 - CSDN博客
2013年07月17日 11:27:02[wishchin](https://me.csdn.net/wishchin)阅读数：1136
序言：清除链接边缘，可以使用数组进行递归运算;
        连通域检测的递归算法是定义级别的检测算法，且是无优化和无语义失误的。
同样可用于寻找连通域
```cpp
void ClearEdge(CvMat* MM,CvPoint pStart,float value)//清除边缘函数
{
    float m_value = value;
    int xNum[8] = {1,1,0,-1,-1,-1,0,1};
    int yNum[8] = {0,1,1,1,0,-1,-1,-1};
    //亮点，使用数组取代条件查找。。。
    CvPoint p;
    int x = p.x = pStart.x;
    int y = p.y = pStart.y;
    for(int k=0 ;k<8 ;k++)
    {
        int yy = y + yNum[k];
        int xx = x + xNum[k];
        if (cvmGet(MM, yy ,xx) == m_value)
          
        {
            pStart.x = xx;
            pStart.y = yy;
            //该点设为边界点        //进行清除！！！设为白点
            cvmSet( MM ,yy,xx,255.0);
         
            //以该点为中心再进行跟踪
            ClearEdge(MM,pStart,value);
            //TraceEdge(yy,xx,nThrLow,pResult,pMag,sz);
        }
    }
}
```
