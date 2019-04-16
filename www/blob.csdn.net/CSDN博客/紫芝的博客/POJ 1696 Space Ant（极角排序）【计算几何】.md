# POJ 1696 Space Ant（极角排序）【计算几何】 - 紫芝的博客 - CSDN博客





2018年10月28日 21:30:41[紫芝](https://me.csdn.net/qq_40507857)阅读数：11








[ACM博客_kuangbin](https://www.cnblogs.com/kuangbin/)

## [POJ 1696 Space Ant（极角排序）](https://www.cnblogs.com/kuangbin/p/3192286.html)

Space Ant
|**Time Limit:** 1000MS||**Memory Limit:** 10000K|
|----|----|----|
|**Total Submissions:** 2489||**Accepted:** 1567|

Description

The most exciting space discovery occurred at the end of the 20th century. In 1999, scientists traced down an ant-like creature in the planet Y1999 and called it M11. It has only one eye on the left side of its head and just three feet all on the right side of its body and suffers from three walking limitations: 
- It can not turn right due to its special body structure. 
- It leaves a red path while walking. 
- It hates to pass over a previously red colored path, and never does that.



The pictures transmitted by the Discovery space ship depicts that plants in the Y1999 grow in special points on the planet. Analysis of several thousands of the pictures have resulted in discovering a magic coordinate system governing the grow points of the plants. In this coordinate system with x and y axes, **no two plants share the same x or y**. 

An M11 needs to eat exactly one plant in each day to stay alive. When it eats one plant, it remains there for the rest of the day with no move. Next day, it looks for another plant to go there and eat it. If it can not reach any other plant it dies by the end of the day. Notice that it can reach a plant in any distance. 

The problem is to find a path for an M11 to let it live longest. 

Input is a set of (x, y) coordinates of plants. Suppose A with the coordinates (xA, yA) is the plant with the least y-coordinate. M11 starts from point (0,yA) heading towards plant A. Notice that the solution path should not cross itself and all of the turns should be counter-clockwise. Also note that the solution may visit more than two plants located on a same straight line. 

![](http://poj.org/images/1696_1.jpg)

Input

The first line of the input is M, the number of test cases to be solved (1 <= M <= 10). For each test case, the first line is N, the number of plants in that test case (1 <= N <= 50), followed by N lines for each plant data. Each plant data consists of three integers: the first number is the unique plant index (1..N), followed by two positive integers x and y representing the coordinates of the plant. Plants are sorted by the increasing order on their indices in the input file. Suppose that the values of coordinates are at most 100.

Output

Output should have one separate line for the solution of each test case. A solution is the number of plants on the solution path, followed by the indices of visiting plants in the path in the order of their visits.

Sample Input

```
2
10
1 4 5
2 9 8
3 5 9
4 1 7
5 3 2
6 6 3
7 10 10
8 8 1
9 2 4
10 7 6
14
1 6 11
2 11 9
3 8 7
4 12 8
5 9 20
6 3 2
7 1 6
8 2 13
9 15 1
10 14 17
11 13 19
12 5 18
13 7 3
14 10 16
```

Sample Output

```
10 8 7 3 4 9 5 6 2 1 10
14 9 10 11 5 12 8 7 6 13 4 14 1 3 2
```

Source

[Tehran 1999](http://poj.org/searchproblem?field=source&key=Tehran+1999)

题意：

一只蚂蚁去吃菜，只能向左转弯，并且走过的路径会被标记，不能再走，问能吃到最多的卷心菜，输出吃菜顺序

思路：

这只蚂蚁可以像卷心菜那样一圈一圈的走，最后一定可以吃完n个卷心菜，接下来考虑先吃哪个点，肯定是先吃左下角的点，然后每次都选择拐角最小的点，就是每次以当前点为起点，对剩下的点进行极角坐标的排序；极角排序最简单的方法就是使用叉积

```cpp
#include<cstdio>
#include<cmath>
#include<cstring>
#include<algorithm>
using namespace std;
const double EPS=1e-8;
int sgn(double x)
{
    if(fabs(x)<EPS) return 0;
    if(x<0) return -1;
    else    return 1;
}

struct Point{
    double x,y;
    int index;
    Point(){}
    Point(int _x,int _y){
        x=_x;   y=_y;
    }
    Point operator -(const Point &b)const{
        return Point(x-b.x,y-b.y);
    }
    //叉积
    double operator ^(const Point &b)const{
        return x*b.y-y*b.x;
    }
    //点积
    double operator *(const Point &b)const{
        return x*b.x+y*b.y;
    }
};
double dist(Point a,Point b)
{
    return sqrt((a-b)*(a-b));
}
int pos;
Point p[100];
bool cmp(Point a,Point b)
{
    double tmp=(a-p[pos])^(b-p[pos]);
    if(sgn(tmp)==0)
        return dist(p[pos],a)<dist(p[pos],b);
    else if(sgn(tmp)<0) return false;
    else    return true;
}
int main()
{
    int T;
    scanf("%d",&T);
    while(T--){
        int n;
        scanf("%d",&n);
        for(int i=0;i<n;i++){
            scanf("%d%lf%lf",&p[i].index,&p[i].x,&p[i].y);
            if(p[0].y>p[i].y||p[0].y==p[i].y&&p[0].x>p[i].x)
                swap(p[0],p[i]);
        }
        pos=0;
        for(int i=1;i<n;i++){
            sort(p+i,p+n,cmp);
            pos++;
        }
        printf("%d",n);
        for(int i=0;i<n;i++)
            printf(" %d",p[i].index);
        printf("\n");
    }
    return 0;
}
```





