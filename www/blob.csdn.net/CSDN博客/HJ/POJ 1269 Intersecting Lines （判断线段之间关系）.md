# POJ 1269  Intersecting Lines  （判断线段之间关系） - HJ - CSDN博客
2017年01月14日 20:02:05[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：400
Description 
We all know that a pair of distinct points on a plane defines a line and that a pair of lines on a plane will intersect in one of three ways: 1) no intersection because they are parallel, 2) intersect in a line because they are on top of one another (i.e. they are the same line), 3) intersect in a point. In this problem you will use your algebraic knowledge to create a program that determines how and where two lines intersect. 
Your program will repeatedly read in four points that define two lines in the x-y plane and determine how and where the lines intersect. All numbers required by this problem will be reasonable, say between -1000 and 1000.
Input 
The first line contains an integer N between 1 and 10 describing how many pairs of lines are represented. The next N lines will each contain eight integers. These integers represent the coordinates of four points on the plane in the order x1y1x2y2x3y3x4y4. Thus each of these input lines represents two lines on the plane: the line through (x1,y1) and (x2,y2) and the line through (x3,y3) and (x4,y4). The point (x1,y1) is always distinct from (x2,y2). Likewise with (x3,y3) and (x4,y4).
Output 
There should be N+2 lines of output. The first line of output should read INTERSECTING LINES OUTPUT. There will then be one line of output for each pair of planar lines represented by a line of input, describing how the lines intersect: none, line, or point. If the intersection is a point then your program should output the x and y coordinates of the point, correct to two decimal places. The final line of output should read “END OF OUTPUT”.
Sample Input
5 
0 0 4 4 0 4 4 0 
5 0 7 6 1 0 2 3 
5 0 7 6 3 -6 4 -3 
2 0 2 27 1 5 18 5 
0 3 4 0 1 2 2 5
Sample Output
INTERSECTING LINES OUTPUT 
POINT 2.00 2.00 
NONE 
LINE 
POINT 2.00 5.00 
POINT 1.07 2.20 
END OF OUTPUT
Source 
Mid-Atlantic 1996
题目大意： 
    给出四个点（两条直线），判断两条线段之间的关系（相交、平行、共线）
解法： 
   先判断是否共线，然后判断是否平行，否则便是相交，求出交点坐标
```
#include<iostream>
#include<cstdio>
using namespace std;
struct point{
    double x,y;
}p1,p2,q1,q2;                                                          //线段l1(两个端点p1p1) 线段l2(两个端点q1q2) 
point intersection(point u1,point u2,point v1,point v2)                 //求两条直线的交点坐标 
{
    point ret=u1;
    double t=((u1.x-v1.x)*(v1.y-v2.y)-(u1.y-v1.y)*(v1.x-v2.x))
            /((u1.x-u2.x)*(v1.y-v2.y)-(u1.y-u2.y)*(v1.x-v2.x));
    ret.x+=(u2.x-u1.x)*t;
    ret.y+=(u2.y-u1.y)*t;
    return ret;
}
bool judge(point a,point p1,point p2)                                    //外积 
{
    if((p1.x-a.x)*(p2.y-a.y)-(p1.y-a.y)*(p2.x-a.x)==0)
       return true;
    return false;
}
int main()
{
    int n;
    scanf("%d",&n);        
    printf("INTERSECTING LINES OUTPUT\n");
    while(n--)
    {
        scanf("%lf%lf%lf%lf",&p1.x,&p1.y,&p2.x,&p2.y);
        scanf("%lf%lf%lf%lf",&q1.x,&q1.y,&q2.x,&q2.y);
        if(judge(p1,p2,q1)&&judge(p1,p2,q2))                                    //外积为0，即三点共线 
          printf("LINE\n");
        else if((p1.x-p2.x)*(q1.y-q2.y)-(p1.y-p2.y)*(q1.x-q2.x)==0)            //两直线平行公式 x1x2-y1y2=0 
          printf("NONE\n");
        else                                                                   //否则求交点坐标 
        {
            point sum=intersection(p1,p2,q1,q2);
            printf("POINT %.2f %.2f\n",sum.x,sum.y);
        }
    }
    printf("END OF OUTPUT\n");
    return 0;
}
```
