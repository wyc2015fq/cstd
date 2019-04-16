# POJ1066 Treasure Hunt 判断线段相交模板 【计算几何】 - 紫芝的博客 - CSDN博客





2018年10月26日 22:53:15[紫芝](https://me.csdn.net/qq_40507857)阅读数：54








Archeologists from the Antiquities and Curios Museum (ACM) have flown to Egypt to examine the great pyramid of Key-Ops. Using state-of-the-art technology they are able to determine that the lower floor of the pyramid is constructed from a series of straightline walls, which intersect to form numerous enclosed chambers. Currently, no doors exist to allow access to any chamber. This state-of-the-art technology has also pinpointed the location of the treasure room. What these dedicated (and greedy) archeologists want to do is blast doors through the walls to get to the treasure room. However, to minimize the damage to the artwork in the intervening chambers (and stay under their government grant for dynamite) they want to blast through the minimum number of doors. For structural integrity purposes, doors should only be blasted at the midpoint of the wall of the room being entered. You are to write a program which determines this minimum number of doors. 

An example is shown below: 

![](https://vj.e949.cn/00633d3443fee9be8a0e40bc2b640508?v=1540552955)

Input

The input will consist of one case. The first line will be an integer n (0 <= n <= 30) specifying number of interior walls, followed by n lines containing integer endpoints of each wall x1 y1 x2 y2 . The 4 enclosing walls of the pyramid have fixed endpoints at (0,0); (0,100); (100,100) and (100,0) and are not included in the list of walls. The interior walls always span from one exterior wall to another exterior wall and are arranged such that no more than two walls intersect at any point. You may assume that no two given walls coincide. After the listing of the interior walls there will be one final line containing the floating point coordinates of the treasure in the treasure room (guaranteed not to lie on a wall).

Output

Print a single line listing the minimum number of doors which need to be created, in the format shown below.

Sample Input

```
7 
20 0 37 100 
40 0 76 100 
85 0 0 75 
100 90 0 90 
0 71 100 61 
0 14 100 38 
100 47 47 100 
54.5 55.4
```

Sample Output
`Number of doors = 2 `
**题意是最少穿过多少个墙壁，其实枚举起点和终点，看有多少条线段相交 **

**对于tan(θ) = y / x:**

**θ = ATan(y / x)求出的θ取值范围是[-PI/2, PI/2]。**

**θ = ATan2(y, x)求出的θ取值范围是[-PI, PI]。**

```cpp
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

const int MAXN = 110;
Line line[MAXN];

Point s;

Point p[MAXN];
int n;
int count(Line L)
{
    int num=0;
    for(int i=1;i<=n;i++){
        if(inter(L,line[i]))
            num++;
    }
    return num;
}
int main()
{
    while(scanf("%d",&n)!=EOF){
        double x1,x2,y1,y2;
        for(int i=1;i<=n;i++){
            scanf("%lf%lf%lf%lf",&x1,&y1,&x2,&y2);
            line[i]=Line(Point(x1,y1),Point(x2,y2));
            p[2*i-1]=Point(x1,y1);
            p[2*i]=Point(x2,y2);
        }
        scanf("%lf%lf",&x1,&y1);
        s=Point(x1,y1);
        int ans=1e9;
        for(int i=1;i<=n*2;i++){
            Line tmp=Line(s,p[i]);
            ans=min(ans,count(tmp));
        }

        Line tmp=Line(s,Point(0,0));
        ans=min(ans,count(tmp)+1);

        tmp=Line(s,Point(0,100));
        ans=min(ans,count(tmp)+1);

        tmp=Line(s,Point(100,0));
        ans=min(ans,count(tmp)+1);

        tmp=Line(s,Point(100,100));
        ans=min(ans,count(tmp)+1);
        printf("Number of doors = %d\n",ans);
    }
    return 0;
}
```





