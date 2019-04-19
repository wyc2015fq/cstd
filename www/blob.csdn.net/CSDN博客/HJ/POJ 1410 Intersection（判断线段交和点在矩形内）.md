# POJ 1410 Intersection（判断线段交和点在矩形内） - HJ - CSDN博客
2017年01月14日 20:09:38[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：344
Description 
You are to write a program that has to decide whether a given line segment intersects a given rectangle. 
An example:  
line: start point: (4,9)  
end point: (11,2)  
rectangle: left-top: (1,5)  
right-bottom: (7,1) 
Figure 1: Line segment does not intersect rectangle 
The line is said to intersect the rectangle if the line and the rectangle have at least one point in common. The rectangle consists of four straight lines and the area in between. Although all input values are integer numbers, valid intersection points do not have to lay on the integer grid. 
Input 
The input consists of n test cases. The first line of the input file contains the number n. Each following line contains one test case of the format:  
xstart ystart xend yend xleft ytop xright ybottom 
where (xstart, ystart) is the start and (xend, yend) the end point of the line and (xleft, ytop) the top left and (xright, ybottom) the bottom right corner of the rectangle. The eight numbers are separated by a blank. The terms top left and bottom right do not imply any ordering of coordinates.
Output 
For each test case in the input file, the output file should contain a line consisting either of the letter “T” if the line segment intersects the rectangle or the letter “F” if the line segment does not intersect the rectangle.
Sample Input
1 
4 9 11 2 1 5 7 1
Sample Output
F
Source 
Southwestern European Regional Contest 1995
题目大意：  
     给了一个线段和矩形。如果线段和矩形的边相交，或者线段在矩形内。输出T，否则输出F。
解法： 
     判断线段是否与矩形四条边相交，然后判断线段两个端点是否在矩形内即可。
```
#include<iostream>
#include<cstdio>
using namespace std;
#define eps 1e-8
struct point{
    double x,y; 
}p,q,a[2][2],s[5];
double m(point a,point p1,point p2)
{
    return (p1.x-a.x)*(p2.y-a.y)-(p1.y-a.y)*(p2.x-a.x);
}
bool check(point u1,point u2,point v1,point v2)                            //判断两条线段是否相交 
{
    if(min(u1.x,u2.x)>max(v1.x,v2.x)||
       min(u1.y,u2.y)>max(v1.y,v2.y)||
       min(v1.x,v2.x)>max(u1.x,u2.x)||
       min(v1.y,v2.y)>max(u1.y,u2.y))
      return false;
    return m(u1,u2,v1)*m(u1,u2,v2)<=eps&&m(v1,v2,u1)*m(v1,v2,u2)<=eps; 
}
int main()
{
    int n;
    scanf("%d",&n);
    while(n--)
    {
        scanf("%lf%lf%lf%lf",&p.x,&p.y,&q.x,&q.y);
        scanf("%lf%lf%lf%lf",&a[0][0].x,&a[0][0].y,&a[1][0].x,&a[1][0].y);
        s[0].x=s[1].x=a[0][0].x;
        s[2].y=s[1].y=a[1][0].y;
        s[2].x=s[3].x=a[1][0].x;
        s[0].y=s[3].y=a[0][0].y;                                        //将矩形四个点由二维数组转换成一维数组，方便循环 
        s[4].x=s[0].x;s[4].y=s[0].y;                                    //多考虑最后一个点和第一个点还有一条线段  
        int blag=0;
        for(int i=1;i<=4;i++)
          if(check(p,q,s[i-1],s[i]))
          {
              blag=1;
              break;
          }
        double min_x=min(s[0].x,s[3].x),max_x=max(s[0].x,s[3].x);
        double min_y=min(s[0].y,s[1].y),max_y=max(s[0].y,s[1].y);       //找出矩形的边界数据 
        if(!blag)                                                       //如果线段与矩形没有交点 
        {   
            if(p.x>min_x&&p.y<max_y&&p.x<max_x&&p.y>min_y)              //需要考虑线段是在矩形内的情况 
              printf("T\n");
            else                                                       //考虑线段不在矩形内的情况 
              printf("F\n");
        }
        else
          printf("T\n");
    }
    return 0;
}
```
