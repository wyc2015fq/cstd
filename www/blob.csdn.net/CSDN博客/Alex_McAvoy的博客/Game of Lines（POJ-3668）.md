# Game of Lines（POJ-3668） - Alex_McAvoy的博客 - CSDN博客





2018年06月08日 00:08:06[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：68
个人分类：[POJ																[基础算法——排序](https://blog.csdn.net/u011815404/article/category/7439201)](https://blog.csdn.net/u011815404/article/category/7504500)








# Problem Description

Farmer John has challenged Bessie to the following game: FJ has a board with dots marked at N (2 ≤ N ≤ 200) distinct lattice points. Dot i has the integer coordinates Xi and Yi (-1,000 ≤ Xi ≤ 1,000; -1,000 ≤ Yi ≤ 1,000).

Bessie can score a point in the game by picking two of the dots and drawing a straight line between them; however, she is not allowed to draw a line if she has already drawn another line that is parallel to that line. Bessie would like to know her chances of winning, so she has asked you to help find the maximum score she can obtain.

# **Input**

Line 1: A single integer: N

Lines 2..N+1: Line i+1 describes lattice point i with two space-separated integers: Xi and Yi.

# Output

Line 1: A single integer representing the maximal number of lines Bessie can draw, no two of which are parallel.

# Sample Input

**4**

**-1 1-2 00 01 1**

# Sample Output

**4**

———————————————————————————————————————————————————————

题意：给出n个点的坐标，求最多能确定多少个互不平行的直线。

思路：用atan()函数求出所有点的斜率，排序后进行判断即可。

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 1050
#define MOD 123
#define E 1e-6
using namespace std;
struct Point{
    double x;
    double y;
}point[N];
double a[N*N];
double Calculate(Point p1, Point p2)
{
    Point p;
    p.x=p2.x-p1.x;
    p.y=p2.y-p1.y;

    if(fabs(p.x)<E)//斜率不存在
        return 1;

    double temp=atan(p.y/p.x);
    if(temp<0)//注意斜率的范围
        return 1+temp;
    else
        return temp;
}

int main()
{
    int n;
    scanf("%d",&n);

    for(int i=0;i<n;i++)
        scanf("%lf%lf",&point[i].x,&point[i].y);

    int k=0;
    for(int i=0;i<n;i++)
        for(int j=i+1;j<n;j++)
        {
            a[k]=Calculate(point[i],point[j]);
            k++;
        }

    sort(a,a+k);

    int ans=1;
    for(int i=1;i<k;i++)
        if(a[i]!=a[i-1])
            ans++;

    printf("%d\n",ans);
    return 0;
}
```






