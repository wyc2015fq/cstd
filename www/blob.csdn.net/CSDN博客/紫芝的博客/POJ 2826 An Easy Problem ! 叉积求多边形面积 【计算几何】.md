# POJ 2826 An Easy Problem?! 叉积求多边形面积 【计算几何】 - 紫芝的博客 - CSDN博客





2018年10月29日 16:02:27[紫芝](https://me.csdn.net/qq_40507857)阅读数：21
个人分类：[计算几何](https://blog.csdn.net/qq_40507857/article/category/7631777)









[ACM博客_kuangbin](https://www.cnblogs.com/kuangbin/)

## [POJ 2826 An Easy Problem?!](https://www.cnblogs.com/kuangbin/p/3192511.html)

An Easy Problem?!
|**Time Limit:** 1000MS||**Memory Limit:** 65536K|
|----|----|----|
|**Total Submissions:** 7837||**Accepted:** 1145|

Description

It's raining outside. Farmer Johnson's bull Ben wants some rain to water his flowers. Ben nails two wooden boards on the wall of his barn. Shown in the pictures below, the two boards on the wall just look like two segments on the plane, as they have the same width. 

![](http://poj.org/images/2826_1.gif)



Your mission is to calculate how much rain these two boards can collect. 

Input

The first line contains the number of test cases. 

Each test case consists of 8 integers not exceeding 10,000 by absolute value, *x*1, *y*1, *x*2, *y*2, *x*3, *y*3, *x*4, *y*4. (*x*1, *y*1), (*x*2, *y*2) are the endpoints of one board, and (*x*3, *y*3), (*x*4, *y*4) are the endpoints of the other one. 

Output

For each test case output a single line containing a real number with precision up to two decimal places - the amount of rain collected. 

Sample Input

```
2
0 1 1 0
1 0 2 1

0 1 2 1
1 0 1 2
```

Sample Output

```
1.00
0.00
```

Source

[POJ Monthly--2006.04.28](http://poj.org/searchproblem?field=source&key=POJ+Monthly--2006.04.28), Dagger@PKU_RPWT

题意：求狭义开口向上的三角形面积

[计算几何基础知识](https://blog.csdn.net/qq_40507857/article/details/83507254)

面积为0.00的三种特殊情况：
- 一条线段平行于X轴，面积为0
- 两条线段不相交，面积为0

![](https://img-blog.csdnimg.cn/2018102918351129.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3,size_16,color_FFFFFF,t_70)

       3.开口被一条线段覆盖掉的情况

![](https://img-blog.csdnimg.cn/20181029183539384.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3,size_16,color_FFFFFF,t_70)

两个线段叉积的几何意义就是两向量围成的平行四边形的有向面积

![](https://img-blog.csdnimg.cn/20181029185615994.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3,size_16,color_FFFFFF,t_70)



```cpp
#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <string.h>
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
};
struct Line
{
    Point s,e;
    Line(){}
    Line(Point _s,Point _e)
    {
        s = _s;e = _e;
    }
    //两直线相交求交点
    //第一个值为0表示直线重合，为1表示平行，为0表示相交,为2是相交
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

//*判断线段相交
bool inter(Line l1,Line l2)
{
    return
    max(l1.s.x,l1.e.x) >= min(l2.s.x,l2.e.x) &&
    max(l2.s.x,l2.e.x) >= min(l1.s.x,l1.e.x) &&
    max(l1.s.y,l1.e.y) >= min(l2.s.y,l2.e.y) &&
    max(l2.s.y,l2.e.y) >= min(l1.s.y,l1.e.y) &&
    sgn((l2.s-l1.e)^(l1.s-l1.e))*sgn((l2.e-l1.e)^(l1.s-l1.e)) <= 0 &&
    sgn((l1.s-l2.e)^(l2.s-l2.e))*sgn((l1.e-l2.e)^(l2.s-l2.e)) <= 0;
}

int main()
{
    int x1,y1,x2,y2,x3,y3,x4,y4;
    int T;
    Line l1,l2;
    scanf("%d",&T);
    while(T--)
    {
        scanf("%d%d%d%d%d%d%d%d",&x1,&y1,&x2,&y2,&x3,&y3,&x4,&y4);
        l1 = Line(Point(x1,y1),Point(x2,y2));
        l2 = Line(Point(x3,y3),Point(x4,y4));

        if(sgn(l1.s.y-l1.e.y) < 0)swap(l1.s,l1.e);
        if(sgn(l2.s.y-l2.e.y) < 0)swap(l2.s,l2.e);

        //一条线段平行于X轴
        if(sgn(l1.s.y-l1.e.y)==0 || sgn(l2.s.y-l2.e.y) == 0)
        {
            printf("0.00\n");
            continue;
        }

        //两条线段不相交
        if(inter(l1,l2) == false)
        {
            printf("0.00\n");
            continue;
        }
        //口被封掉的情况
        if(inter(Line(l1.s,Point(l1.s.x,100000)),l2) )
        {
            printf("0.00\n");
            continue;
        }
        //口被封掉
        if(inter(Line(l2.s,Point(l2.s.x,100000)),l1) )
        {
            printf("0.00\n");
            continue;
        }
        pair<int,Point>pr;
        pr = l1 & l2;
        Point p = pr.second;//两条线段的交点

        double ans1=1e9;
        pr = l1 & Line(Point(100000,l2.s.y),l2.s);
        if(pr.first==2){//相交
            Point p1 = pr.second;
            ans1 = fabs( (l2.s-p)^(p1-p) )/2;
        }

        double ans2=1e9;
        pr = l2 & Line(Point(100000,l1.s.y),l1.s);
        if(pr.first==2){//相交
            Point p2 = pr.second;
            ans2 = fabs( (l1.s-p)^(p2-p) )/2;
        }

        printf("%.2lf\n",min(ans1,ans2));
    }
    return 0;
}
```





