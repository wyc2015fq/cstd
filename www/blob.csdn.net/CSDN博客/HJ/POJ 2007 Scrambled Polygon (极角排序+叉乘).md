# POJ  2007  Scrambled Polygon  (极角排序+叉乘) - HJ - CSDN博客
2017年01月14日 20:23:17[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：329
个人分类：[====ACM====																[---------OJ---------																[POJ																[--------计算几何--------																[点和直线的关系](https://blog.csdn.net/feizaoSYUACM/article/category/7238016)](https://blog.csdn.net/feizaoSYUACM/article/category/7238091)](https://blog.csdn.net/feizaoSYUACM/article/category/6308043)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Scrambled Polygon
Description 
A closed polygon is a figure bounded by a finite number of line segments. The intersections of the bounding line segments are called the vertices of the polygon. When one starts at any vertex of a closed polygon and traverses each bounding line segment exactly once, one comes back to the starting vertex.
A closed polygon is called convex if the line segment joining any two points of the polygon lies in the polygon. Figure 1 shows a closed polygon which is convex and one which is not convex. (Informally, a closed polygon is convex if its border doesn’t have any “dents”.) 
![这里写图片描述](http://poj.org/images/2007_1.jpg)
The subject of this problem is a closed convex polygon in the coordinate plane, one of whose vertices is the origin (x = 0, y = 0). Figure 2 shows an example. Such a polygon will have two properties significant for this problem.
The first property is that the vertices of the polygon will be confined to three or fewer of the four quadrants of the coordinate plane. In the example shown in Figure 2, none of the vertices are in the second quadrant (where x < 0, y > 0).
To describe the second property, suppose you “take a trip” around the polygon: start at (0, 0), visit all other vertices exactly once, and arrive at (0, 0). As you visit each vertex (other than (0, 0)), draw the diagonal that connects the current vertex with (0, 0), and calculate the slope of this diagonal. Then, within each quadrant, the slopes of these diagonals will form a decreasing or increasing sequence of numbers, i.e., they will be sorted. Figure 3 illustrates this point. 
![这里写图片描述](http://poj.org/images/2007_2.jpg)
![这里写图片描述](http://poj.org/images/2007_3.jpg)
Input 
The input lists the vertices of a closed convex polygon in the plane. The number of lines in the input will be at least three but no more than 50. Each line contains the x and y coordinates of one vertex. Each x and y coordinate is an integer in the range -999..999. The vertex on the first line of the input file will be the origin, i.e., x = 0 and y = 0. Otherwise, the vertices may be in a scrambled order. Except for the origin, no vertex will be on the x-axis or the y-axis. No three vertices are colinear.
Output 
The output lists the vertices of the given polygon, one vertex per line. Each vertex from the input appears exactly once in the output. The origin (0,0) is the vertex on the first line of the output. The order of vertices in the output will determine a trip taken along the polygon’s border, in the counterclockwise direction. The output format for each vertex is (x,y) as shown below.
Sample Input
0 0 70 -50 60 30 -30 -50 80 20 50 -60 90 -20 -30 -40 -10 -60 90 10
Sample Output
(0,0) (-30,-40) (-30,-50) (-10,-60) (50,-60) (70,-50) (90,-20) (90,10) (80,20) (60,30)
题目大意： 
    从(0，0)点开始输入一个凸多边形，这个凸多边形，占有三个象限，按照逆时针的方式输出各定点。
解法： 
    叉乘排序。
```cpp
#include<iostream>
#include<algorithm>
#include<cstdio>
#include<cmath>
using namespace std;
#define eps 1e-8
struct point{
    double x,y;
}s[55];
int tf(double a)
{
    if(fabs(a)<eps)
      return 0;
    return a>0?1:-1;
}
double multi(point a,point p1,point p2)
{
    return (p1.x-a.x)*(p2.y-a.y)-(p1.y-a.y)*(p2.x-a.x);
}
bool cmp(point a,point b)                        //判断叉积  若叉积小于0 逆时针；若叉积大于零 顺时针；否则，三点共线 
{
    return tf(multi(s[0],a,b))>0;
}
int main()
{
    int n=0;
    while(scanf("%lf %lf",&s[n].x,&s[n].y)!=EOF)
      n++;
    sort(s+1,s+n,cmp);                             //极角排序 
    for(int i=0;i<n;i++)
      printf("(%.0f,%.0f)\n",s[i].x,s[i].y);
    return 0;
}
```
