# POJ 1410 Intersection 判断线段交和点在矩形内 【计算几何】 - 紫芝的博客 - CSDN博客





2018年10月28日 12:19:56[紫芝](https://me.csdn.net/qq_40507857)阅读数：71








[ACM博客_kuangbin](https://www.cnblogs.com/kuangbin/)

## [POJ 1410 Intersection（判断线段交和点在矩形内）](https://www.cnblogs.com/kuangbin/p/3192039.html)

Intersection
|**Time Limit:** 1000MS||**Memory Limit:** 10000K|
|----|----|----|
|**Total Submissions:** 9996||**Accepted:** 2632|

Description

You are to write a program that has to decide whether a given line segment intersects a given rectangle. 


An example: 

line: start point: (4,9) 

end point: (11,2) 

rectangle: left-top: (1,5) 

right-bottom: (7,1) 

![](http://poj.org/images/1410_1.jpg)

Figure 1: Line segment does not intersect rectangle 


The line is said to intersect the rectangle if the line and the rectangle have at least one point in common. The rectangle consists of four straight lines and the area in between. Although all input values are integer numbers, valid intersection points do not have to lay on the integer grid. 

Input

The input consists of n test cases. The first line of the input file contains the number n. Each following line contains one test case of the format: 

xstart ystart xend yend xleft ytop xright ybottom 


where (xstart, ystart) is the start and (xend, yend) the end point of the line and (xleft, ytop) the top left and (xright, ybottom) the bottom right corner of the rectangle. The eight numbers are separated by a blank. The terms top left and bottom right do not imply any ordering of coordinates.

Output

For each test case in the input file, the output file should contain a line consisting either of the letter "T" if the line segment intersects the rectangle or the letter "F" if the line segment does not intersect the rectangle.

Sample Input

```
1
4 9 11 2 1 5 7 1
```

Sample Output
`F`
Source

[Southwestern European Regional Contest 1995](http://poj.org/searchproblem?field=source&key=Southwestern+European+Regional+Contest+1995)

### 题意：

**给了一个线段和矩形。如果线段和矩形的边相交，或者线段在矩形内。输出T，否则输出F**

### **思路：**

**先判断矩形4条边是否与线段相交，然后判断线段两个端点是否在矩形内**

**AC代码1：**

**转角法判断点在多边形内：实现简单，效率较低**

```cpp
//判断线段相交和点在矩形内
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <queue>
#include <map>
#include <vector>
#include <set>
#include <string>
#include <math.h>

using namespace std;
const double eps = 1e-8;
int sgn(double x)
{
    if(fabs(x) < eps)return 0;
    if(x < 0)return -1;
    else return 1;
}
struct Point
{
    double x,y;
    Point(){}
    Point(double _x,double _y)
    {
        x = _x;y = _y;
    }
    Point operator -(const Point &b)const
    {
        return Point(x - b.x,y - b.y);
    }
    //叉积
    double operator ^(const Point &b)const
    {
        return x*b.y - y*b.x;
    }
    //点积
    double operator *(const Point &b)const
    {
        return x*b.x + y*b.y;
    }
    //绕原点旋转角度B（弧度值），后x,y的变化
    void transXY(double B)
    {
        double tx = x,ty = y;
        x = tx*cos(B) - ty*sin(B);
        y = tx*sin(B) + ty*cos(B);
    }
};
struct Line
{
    Point s,e;
    double k;
    Line(){}
    Line(Point _s,Point _e)
    {
        s = _s;e = _e;
        k = atan2(e.y - s.y,e.x - s.x);
    }
    //两条直线求交点，
    //第一个值为0表示直线重合，为1表示平行，为2是相交
    //只有第一个值为2时，交点才有意义
    pair<int,Point> operator &(const Line &b)const
    {
        Point res = s;
        if(sgn((s-e)^(b.s-b.e)) == 0)
        {
            if(sgn((s-b.e)^(b.s-b.e)) == 0)
                return make_pair(0,res);//重合
            else return make_pair(1,res);//平行
        }
        double t = ((s-b.s)^(b.s-b.e))/((s-e)^(b.s-b.e));
        res.x += (e.x-s.x)*t;
        res.y += (e.y-s.y)*t;
        return make_pair(2,res);
    }
};
//两点间距离
double dist(Point a,Point b)
{
    return sqrt((a-b)*(a-b));
}
//判断线段相交
bool inter(Line l1,Line l2)
{
    return
        max(l1.s.x,l1.e.x) >= min(l2.s.x,l2.e.x) &&
        max(l2.s.x,l2.e.x) >= min(l1.s.x,l1.e.x) &&
        max(l1.s.y,l1.e.y) >= min(l2.s.y,l2.e.y) &&
        max(l2.s.y,l2.e.y) >= min(l1.s.y,l1.e.y) &&
        sgn((l2.s-l1.s)^(l1.e-l1.s))*sgn((l2.e-l1.s)^(l1.e-l1.s)) <= 0 &&
        sgn((l1.s-l2.s)^(l2.e-l1.s))*sgn((l1.e-l2.s)^(l2.e-l2.s)) <= 0;
}
//判断点在线段上
bool OnSeg(Point p,Line L)
{
    return
    sgn((L.s-p)^(L.e-p))==0&&
    sgn((p.x-L.s.x)*(p.x-L.e.x))<=0&&
    sgn((p.y-L.s.y)*(p.y-L.e.y))<=0;
}

//判断点在凸多边形内，点形成一个凸包，而且按逆时针排序
//如果是顺时针，就把<0改成>0
//点的编号：0~n-1
//返回值：-1点在凸多边形外，0点在凸多边形边上，1点在凸多边形内
int inConvexPoly(Point a,Point p[],int n)
{
    for(int i=0;i<n;i++)
    {
        if(sgn((p[i]-a)^(p[(i+1)%n]-a))<0)
           return -1;
        else if(OnSeg(a,Line(p[i],p[(i+1)%n])))
            return 0;
    }
    return 1;
}

//判断点在任意多边形内
//射线法，poly[]的顶点数要大于等于3，点的编号：0~n-1
//返回值
//-1：点在多边形外
//0：点在多边形边界上
//1：点在多边形内
int inPoly(Point p,Point poly[],int n)
{
    int cnt=0;
    Line ray,side;
    ray.s=p;
    ray.s.y=p.y;
    ray.e.x=-100000000000.0;//-INF,注意取值，防止越界

    for(int i=0;i<n;i++)
    {
        side.s=poly[i];
        side.e=poly[(i+1)%n];
        if(OnSeg(p,side))   return 0;
        //如果平行轴，则不考虑
        if(sgn(side.s.y-side.e.y)==0)
            continue;
        if(OnSeg(side.s,ray)){
            if(sgn(side.s.y-side.e.y)>0)
                cnt++;
        }
        else if(OnSeg(side.e,ray)){
            if(sgn(side.e.y-side.s.y)>0)
                cnt++;
        }
        else if(inter(ray,side))
            cnt++;
    }
    if(cnt%2==1)
        return 1;
    else
        return -1;
}
int main()
{
    int T;
    scanf("%d",&T);
    while(T--){
        Line line;
        scanf("%lf%lf%lf%lf",&line.s.x,&line.s.y,&line.e.x,&line.e.y);
            double x1,y1,x2,y2;
            scanf("%lf%lf%lf%lf",&x1,&y1,&x2,&y2);
            if(x1>x2)   swap(x1,x2);
            if(y1>y2)   swap(y1,y2);
            Point p[5];//0~3
            p[0]=Point(x1,y1);//left_bottom
            p[1]=Point(x2,y1);//right_bottom
            p[2]=Point(x2,y2);//right_top
            p[3]=Point(x1,y2);//left_top

            Line bottom=Line(p[1],p[0]);
            if(inter(line,bottom)){
                printf("T\n");
                continue;
            }

            Line right=Line(p[2],p[1]);
            if(inter(line,right)){
                printf("T\n");
                continue;
            }

            Line top=Line(p[3],p[2]);
            if(inter(line,top)){
                printf("T\n");
                continue;
            }

            Line left=Line(p[3],p[0]);
            if(inter(line,left)){
                printf("T\n");
                continue;
            }

            if(inConvexPoly(line.s,p,4)>=0||inConvexPoly(line.e,p,4)>=0){
                printf("T\n");
                continue;
            }

            printf("F\n");
            continue;
    }
    return 0;
}
```

**AC代码2：**

**射线法判断点是否在多边形内，貌似要快一点：0 ms**

```cpp
//判断线段相交和点在矩形内
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <queue>
#include <map>
#include <vector>
#include <set>
#include <string>
#include <math.h>

using namespace std;
const double eps = 1e-8;
int sgn(double x)
{
    if(fabs(x) < eps)return 0;
    if(x < 0)return -1;
    else return 1;
}
struct Point
{
    double x,y;
    Point(){}
    Point(double _x,double _y)
    {
        x = _x;y = _y;
    }
    Point operator -(const Point &b)const
    {
        return Point(x - b.x,y - b.y);
    }
    //叉积
    double operator ^(const Point &b)const
    {
        return x*b.y - y*b.x;
    }
    //点积
    double operator *(const Point &b)const
    {
        return x*b.x + y*b.y;
    }
    //绕原点旋转角度B（弧度值），后x,y的变化
    void transXY(double B)
    {
        double tx = x,ty = y;
        x = tx*cos(B) - ty*sin(B);
        y = tx*sin(B) + ty*cos(B);
    }
};
struct Line
{
    Point s,e;
    double k;
    Line(){}
    Line(Point _s,Point _e)
    {
        s = _s;e = _e;
        k = atan2(e.y - s.y,e.x - s.x);
    }
    //两条直线求交点，
    //第一个值为0表示直线重合，为1表示平行，为2是相交
    //只有第一个值为2时，交点才有意义
    pair<int,Point> operator &(const Line &b)const
    {
        Point res = s;
        if(sgn((s-e)^(b.s-b.e)) == 0)
        {
            if(sgn((s-b.e)^(b.s-b.e)) == 0)
                return make_pair(0,res);//重合
            else return make_pair(1,res);//平行
        }
        double t = ((s-b.s)^(b.s-b.e))/((s-e)^(b.s-b.e));
        res.x += (e.x-s.x)*t;
        res.y += (e.y-s.y)*t;
        return make_pair(2,res);
    }
};
//两点间距离
double dist(Point a,Point b)
{
    return sqrt((a-b)*(a-b));
}
//判断线段相交
bool inter(Line l1,Line l2)
{
    return
        max(l1.s.x,l1.e.x) >= min(l2.s.x,l2.e.x) &&
        max(l2.s.x,l2.e.x) >= min(l1.s.x,l1.e.x) &&
        max(l1.s.y,l1.e.y) >= min(l2.s.y,l2.e.y) &&
        max(l2.s.y,l2.e.y) >= min(l1.s.y,l1.e.y) &&
        sgn((l2.s-l1.s)^(l1.e-l1.s))*sgn((l2.e-l1.s)^(l1.e-l1.s)) <= 0 &&
        sgn((l1.s-l2.s)^(l2.e-l1.s))*sgn((l1.e-l2.s)^(l2.e-l2.s)) <= 0;
}
//判断点在线段上
bool OnSeg(Point p,Line L)
{
    return
    sgn((L.s-p)^(L.e-p))==0&&
    sgn((p.x-L.s.x)*(p.x-L.e.x))<=0&&
    sgn((p.y-L.s.y)*(p.y-L.e.y))<=0;
}

//判断点在凸多边形内，点形成一个凸包，而且按逆时针排序
//如果是顺时针，就把<0改成>0
//点的编号：0~n-1
//返回值：-1点在凸多边形外，0点在凸多边形边上，1点在凸多边形内
int inConvexPoly(Point a,Point p[],int n)
{
    for(int i=0;i<n;i++)
    {
        if(sgn((p[i]-a)^(p[(i+1)%n]-a))<0)
           return -1;
        else if(OnSeg(a,Line(p[i],p[(i+1)%n])))
            return 0;
    }
    return 1;
}

//判断点在任意多边形内
//射线法，poly[]的顶点数要大于等于3，点的编号：0~n-1
//返回值
//-1：点在多边形外
//0：点在多边形边界上
//1：点在多边形内
int inPoly(Point p,Point poly[],int n)
{
    int cnt=0;
    Line ray,side;
    ray.s=p;
    ray.s.y=p.y;
    ray.e.x=-100000000000.0;//-INF,注意取值，防止越界

    for(int i=0;i<n;i++)
    {
        side.s=poly[i];
        side.e=poly[(i+1)%n];
        if(OnSeg(p,side))   return 0;
        //如果平行轴，则不考虑
        if(sgn(side.s.y-side.e.y)==0)
            continue;
        if(OnSeg(side.s,ray)){
            if(sgn(side.s.y-side.e.y)>0)
                cnt++;
        }
        else if(OnSeg(side.e,ray)){
            if(sgn(side.e.y-side.s.y)>0)
                cnt++;
        }
        else if(inter(ray,side))
            cnt++;
    }
    if(cnt%2==1)
        return 1;
    else
        return -1;
}
int main()
{
    int T;
    scanf("%d",&T);
    while(T--){
        Line line;
        scanf("%lf%lf%lf%lf",&line.s.x,&line.s.y,&line.e.x,&line.e.y);
            double x1,y1,x2,y2;
            scanf("%lf%lf%lf%lf",&x1,&y1,&x2,&y2);
            if(x1>x2)   swap(x1,x2);
            if(y1>y2)   swap(y1,y2);
            Point p[5];//0~3
            p[0]=Point(x1,y1);//left_bottom
            p[1]=Point(x2,y1);//right_bottom
            p[2]=Point(x2,y2);//right_top
            p[3]=Point(x1,y2);//left_top

            Line bottom=Line(p[1],p[0]);
            if(inter(line,bottom)){
                printf("T\n");
                continue;
            }

            Line right=Line(p[2],p[1]);
            if(inter(line,right)){
                printf("T\n");
                continue;
            }

            Line top=Line(p[3],p[2]);
            if(inter(line,top)){
                printf("T\n");
                continue;
            }

            Line left=Line(p[3],p[0]);
            if(inter(line,left)){
                printf("T\n");
                continue;
            }

            if(inPoly(line.s,p,4)>=0||inPoly(line.e,p,4)>=0){
                printf("T\n");
                continue;
            }

            printf("F\n");
            continue;
    }
    return 0;
}
```





