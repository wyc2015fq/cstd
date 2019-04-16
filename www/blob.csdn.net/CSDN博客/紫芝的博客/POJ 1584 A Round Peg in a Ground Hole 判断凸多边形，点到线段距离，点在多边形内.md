# POJ 1584 A Round Peg in a Ground Hole 判断凸多边形，点到线段距离，点在多边形内 - 紫芝的博客 - CSDN博客





2018年10月30日 10:34:23[紫芝](https://me.csdn.net/qq_40507857)阅读数：1149








[ACM博客_kuangbin](https://www.cnblogs.com/kuangbin/)

## [POJ 1584 A Round Peg in a Ground Hole（判断凸多边形，点到线段距离，点在多边形内）](https://www.cnblogs.com/kuangbin/p/3194599.html)
|A Round Peg in a Ground Hole|**Time Limit:** 1000MS||**Memory Limit:** 10000K||----|----|----||**Total Submissions:** 4438||**Accepted:** 1362|DescriptionThe DIY Furniture company specializes in assemble-it-yourself furniture kits. Typically, the pieces of wood are attached to one another using a wooden peg that fits into pre-cut holes in each piece to be attached. The pegs have a circular cross-section and so are intended to fit inside a round hole. 			A recent factory run of computer desks were flawed when an automatic grinding machine was mis-programmed. The result is an irregularly shaped hole in one piece that, instead of the expected circular shape, is actually an irregular polygon. You need to figure out whether the desks need to be scrapped or if they can be salvaged by filling a part of the hole with a mixture of wood shavings and glue. 			There are two concerns. First, if the hole contains any protrusions (i.e., if there exist any two interior points in the hole that, if connected by a line segment, that segment would cross one or more edges of the hole), then the filled-in-hole would not be structurally sound enough to support the peg under normal stress as the furniture is used. Second, assuming the hole is appropriately shaped, it must be big enough to allow insertion of the peg. Since the hole in this piece of wood must match up with a corresponding hole in other pieces, the precise location where the peg must fit is known. 			Write a program to accept descriptions of pegs and polygonal holes and determine if the hole is ill-formed and, if not, whether the peg will fit at the desired location. Each hole is described as a polygon with vertices (x1, y1), (x2, y2), . . . , (xn, yn). The edges of the polygon are (xi, yi) to (xi+1, yi+1) for i = 1 . . . n − 1 and (xn, yn) to (x1, y1).InputInput consists of a series of piece descriptions. Each piece description consists of the following data: 			Line 1 < nVertices > < pegRadius > < pegX > < pegY > 			number of vertices in polygon, n (integer) 			radius of peg (real) 			X and Y position of peg (real) 			n Lines < vertexX > < vertexY > 			On a line for each vertex, listed in order, the X and Y position of vertex The end of input is indicated by a number of polygon vertices less than 3.OutputFor each piece description, print a single line containing the string: 			HOLE IS ILL-FORMED if the hole contains protrusions 			PEG WILL FIT if the hole contains no protrusions and the peg fits in the hole at the indicated position 			PEG WILL NOT FIT if the hole contains no protrusions but the peg will not fit in the hole at the indicated positionSample Input```5 1.5 1.5 2.01.0 1.02.0 2.01.75 2.01.0 3.00.0 2.05 1.5 1.5 2.01.0 1.02.0 2.01.75 2.51.0 3.00.0 2.01```Sample Output```HOLE IS ILL-FORMEDPEG WILL NOT FIT```Source[Mid-Atlantic 2003](http://poj.org/searchproblem?field=source&key=Mid-Atlantic+2003)|**Time Limit:** 1000MS||**Memory Limit:** 10000K|**Total Submissions:** 4438||**Accepted:** 1362|
|----|----|----|----|----|----|----|
|**Time Limit:** 1000MS||**Memory Limit:** 10000K| | | | |
|**Total Submissions:** 4438||**Accepted:** 1362| | | | |

Description

The DIY Furniture company specializes in assemble-it-yourself furniture kits. Typically, the pieces of wood are attached to one another using a wooden peg that fits into pre-cut holes in each piece to be attached. The pegs have a circular cross-section and so are intended to fit inside a round hole. 

			A recent factory run of computer desks were flawed when an automatic grinding machine was mis-programmed. The result is an irregularly shaped hole in one piece that, instead of the expected circular shape, is actually an irregular polygon. You need to figure out whether the desks need to be scrapped or if they can be salvaged by filling a part of the hole with a mixture of wood shavings and glue. 

			There are two concerns. First, if the hole contains any protrusions (i.e., if there exist any two interior points in the hole that, if connected by a line segment, that segment would cross one or more edges of the hole), then the filled-in-hole would not be structurally sound enough to support the peg under normal stress as the furniture is used. Second, assuming the hole is appropriately shaped, it must be big enough to allow insertion of the peg. Since the hole in this piece of wood must match up with a corresponding hole in other pieces, the precise location where the peg must fit is known. 

			Write a program to accept descriptions of pegs and polygonal holes and determine if the hole is ill-formed and, if not, whether the peg will fit at the desired location. Each hole is described as a polygon with vertices (x1, y1), (x2, y2), . . . , (xn, yn). The edges of the polygon are (xi, yi) to (xi+1, yi+1) for i = 1 . . . n − 1 and (xn, yn) to (x1, y1).

Input

Input consists of a series of piece descriptions. Each piece description consists of the following data: 

			Line 1 < nVertices > < pegRadius > < pegX > < pegY > 

			number of vertices in polygon, n (integer) 

			radius of peg (real) 

			X and Y position of peg (real) 

			n Lines < vertexX > < vertexY > 

			On a line for each vertex, listed in order, the X and Y position of vertex The end of input is indicated by a number of polygon vertices less than 3.

Output

For each piece description, print a single line containing the string: 

			HOLE IS ILL-FORMED if the hole contains protrusions 

			PEG WILL FIT if the hole contains no protrusions and the peg fits in the hole at the indicated position 

			PEG WILL NOT FIT if the hole contains no protrusions but the peg will not fit in the hole at the indicated position

Sample Input

```
5 1.5 1.5 2.0
1.0 1.0
2.0 2.0
1.75 2.0
1.0 3.0
0.0 2.0
5 1.5 1.5 2.0
1.0 1.0
2.0 2.0
1.75 2.5
1.0 3.0
0.0 2.0
1
```

Sample Output

```
HOLE IS ILL-FORMED
PEG WILL NOT FIT
```

Source

[Mid-Atlantic 2003](http://poj.org/searchproblem?field=source&key=Mid-Atlantic+2003)

### 题意：

**给出N个点，一个圆的半径和圆心坐标：**
- 如果N个点构成的多边形不是凸多边形，输出：HOLE IS ILL-FORMED
- 如果N个点构成凸多边形，圆在凸多边形内，输出：PEG WILL FIT
- 如果N个点构成凸多边形，圆不在凸多边内，输出：PEG WILL NOT FIT

### 分析：

**首先是判断给出了多边形是不是凸多边形。然后判断圆包含在凸多边形中。**

**一定要保证圆心在凸多边形里面，然后判断圆心到每条线段的距离要大于等于半径。**

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
    void input(){
        scanf("%lf%lf",&x,&y);
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

//两点距离
double dist(Point a,Point b)
{
    return sqrt((a-b)*(a-b));
}

//判断凸多边形，允许共线边
//点的编号0~n-1，可以是顺时针和逆时针
bool isConvex(Point poly[],int n){
    bool s[3];
    memset(s,false,sizeof s);
    for(int i=0;i<n;i++){
        s[sgn( (poly[(i+1)%n]-poly[i])^(poly[(i+2)%n]-poly[i]) )+1]=true;
        if(s[0]&&s[2])  return false;
    }
    return true;
}

//点到线段的距离，返回点到线段最近的点
Point NearestPointToLineSeg(Point p,Line L)
{
    Point result;
    double t=((p-L.s)*(L.e-L.s))/((L.e-L.s)*(L.e-L.s));
    if(t>=0&&t<=1){
        result.x=L.s.x+(L.e.x-L.s.x)*t;
        result.y=L.s.y+(L.e.y-L.s.y)*t;
    }else{
        if(dist(p,L.s)<dist(p,L.e))
            result=L.s;
        else
            result=L.e;
    }
    return result;
}

//判断点在线段上
bool OnSeg(Point P,Line L)
{
    return
    sgn((L.s-P)^(L.e-P)) == 0 &&
    sgn((P.x - L.s.x) * (P.x - L.e.x)) <= 0 &&
    sgn((P.y - L.s.y) * (P.y - L.e.y)) <= 0;
}

//*判断点在凸多边形内
//点形成一个凸包，而且按逆时针排序（如果是顺时针把里面的<0改为>0）
//点的编号:0~n-1
//返回值：
//-1:点在凸多边形外
//0:点在凸多边形边界上
//1:点在凸多边形内
int inConvexPoly(Point a,Point p[],int n)
{
    for(int i = 0;i < n;i++)
    {
        if(sgn((p[i]-a)^(p[(i+1)%n]-a)) < 0)return -1;
        else if(OnSeg(a,Line(p[i],p[(i+1)%n])))return 0;
    }
    return 1;
}

//*判断点在任意多边形内
//射线法，poly[]的顶点数要大于等于3,点的编号0~n-1
//返回值
//-1:点在凸多边形外
//0:点在凸多边形边界上
//1:点在凸多边形内
int inPoly(Point p,Point poly[],int n)
{
    int cnt;
    Line ray,side;
    cnt = 0;
    ray.s = p;
    ray.e.y = p.y;
    ray.e.x = -100000000000.0;//-INF,注意取值防止越界

    for(int i = 0;i < n;i++)
    {
        side.s = poly[i];
        side.e = poly[(i+1)%n];

        if(OnSeg(p,side))return 0;

        //如果平行轴则不考虑
        if(sgn(side.s.y - side.e.y) == 0)
            continue;

        if(OnSeg(side.s,ray))
        {
            if(sgn(side.s.y - side.e.y) > 0)cnt++;
        }
        else if(OnSeg(side.e,ray))
        {
            if(sgn(side.e.y - side.s.y) > 0)cnt++;
        }
        else if(inter(ray,side))
            cnt++;
    }
    if(cnt % 2 == 1)return 1;
    else return -1;
}

int n;
Point o;//圆心
double R;//半径
Point p[110];
int main()
{
    while(scanf("%d",&n)!=EOF){
        if(n<3) break;
        scanf("%lf%lf%lf",&R,&o.x,&o.y);
        for(int i=0;i<n;i++)
            p[i].input();
        if(!isConvex(p,n)){//不是凸多边形
            printf("HOLE IS ILL-FORMED\n");
            continue;
        }
        //圆心不在凸多边形内
        if(inPoly(o,p,n)<0){
            printf("PEG WILL NOT FIT\n");
            continue;
        }

        bool flag=false;
        for(int i=0;i<n;i++)
        {
            if(sgn(dist(o,NearestPointToLineSeg(o,Line(p[i],p[(i+1)%n]))) -R ) <0 )
            {
                flag=true;
                break;
            }
        }
        if(flag)    printf("PEG WILL NOT FIT\n");
        else        printf("PEG WILL FIT\n");
    }
    return 0;
}
```





