# Points in Segments（CF-1015A） - Alex_McAvoy的博客 - CSDN博客





2018年08月06日 08:35:43[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：72








# Problem Description

You are given a set of n segments on the axis Ox, each segment has integer endpoints between 1 and mm inclusive. Segments may intersect, overlap or even coincide with each other. Each segment is characterized by two integers lili and riri (1≤li≤ri≤m) — coordinates of the left and of the right endpoints.

Consider all integer points between 1 and mm inclusive. Your task is to print all such points that don't belong to any segment. The point x belongs to the segment [l;r] if and only if l≤x≤r.

# Input

The first line of the input contains two integers nn and mm (1≤n,m≤100) — the number of segments and the upper bound for coordinates.

The next nn lines contain two integers each li and ri (1≤li≤ri≤m) — the endpoints of the i-th segment. Segments may intersect, overlap or even coincide with each other. Note, it is possible that li=ri, i.e. a segment can degenerate to a point.

# Output

In the first line print one integer kk — the number of points that don't belong to any segment.

In the second line print exactly kk integers in any order — the points that don't belong to any segment. All points you print should be distinct.

If there are no such points at all, print a single integer 0 in the first line and either leave the second line empty or do not print it at all.

# Examples

**Input**

3 5

2 2

1 2

5 5

**Output**

2

3 4 

**Input**

1 7

1 7

**Output**

0

————————————————————————————————————————————

题意：给出 n 和 m，再给出 n 组区间 [l,r]，从 [1,m]，依次删除给出的区间 [l,r]，输出最后还剩下几个点，以及区间范围

思路：桶排思想标记区间，然后枚举即可

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
#include<set>
#include<map>
#include<stack>
#include<ctime>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 10001
#define MOD 123
#define E 1e-6
using namespace std;
int a[N];
int vis[N];
int main()
{
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF)
    {
        memset(vis,0,sizeof(vis));
        for(int i=1;i<=m;i++)
            a[i]=i;
        for(int i=1;i<=n;i++)
        {
            int x,y;
            scanf("%d%d",&x,&y);
            for(int j=x;j<=y;j++)
                vis[j]=1;
        }

        int sum=0;
        for(int i=1;i<=m;i++)
            if(!vis[i])
                sum++;

        if(sum==0)
            printf("%d\n",sum);
        else
        {
            printf("%d\n",sum);
            for(int i=1;i<=m;i++)
            if(!vis[i])
                printf("%d ",a[i]);
            printf("\n");
        }

    }
    return 0;
}
```





