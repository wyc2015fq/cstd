# Connect the Cities（HDU-3371） - Alex_McAvoy的博客 - CSDN博客





2018年05月21日 21:40:41[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：44
个人分类：[图论——最小生成树																[HDU](https://blog.csdn.net/u011815404/article/category/7923257)](https://blog.csdn.net/u011815404/article/category/7813283)








> 
# Problem Description

In 2100, since the sea level rise, most of the cities disappear. Though some survived cities are still connected with others, but most of them become disconnected. The government wants to build some roads to connect all of these cities again, but they don’t want to take too much money.

# **Input**

The first line contains the number of test cases.

Each test case starts with three integers: n, m and k. n (3 <= n <=500) stands for the number of survived cities, m (0 <= m <= 25000) stands for the number of roads you can choose to connect the cities and k (0 <= k <= 100) stands for the number of still connected cities.

To make it easy, the cities are signed from 1 to n.

Then follow m lines, each contains three integers p, q and c (0 <= c <= 1000), means it takes c to connect p and q.

Then follow k lines, each line starts with an integer t (2 <= t <= n) stands for the number of this connected cities. Then t integers follow stands for the id of these cities.

# Output

For each case, output the least money you need to take, if it’s impossible, just output -1. 

# Sample Input

**1**

**6 4 31 4 22 6 12 3 53 4 332 1 22 1 33 4 5 6**

# Sample Output

**1**


题意：已知有n个城市，部分城市是连通的，可选择修的路有m条，给出了m条路的起点终点和修路花费，问最少可花多少钱能保证所有的城市连通。

思路：把边权排序然后并查集添加边，套用Kruskal算法模版即可。

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#define INF 999999999
#define N 100001
#define MOD 1000000007
#define E 1e-12
using namespace std;
int father[501];
struct node
{
    int x;
    int y;
    int w;
}a[25001];
int Find(int x)
{
    if(x!=father[x])
        father[x]=Find(father[x]);
    return x;
}
int cmp(node a,node b)
{
    return a.w<b.w;
}
int main()
{
    int t,n,m,q;

    scanf("%d",&t);
    while(t--)
    {
        scanf("%d%d%d",&n,&m,&q);
        for(int i=1;i<=n;i++)
            father[i]=i;
        int k=0;
        int num=0;
        for(int i=1;i<=m;i++)
        {
            int x,y,w;
            scanf("%d%d%d",&x,&y,&w);
            a[num].x=x;
            a[num].y=y;
            a[num].w=w;
            num++;
        }
        for(int i=1;i<=q;i++)
        {
            int x,y,l;
            scanf("%d%d",&l,&x);
            for(int j=1;j<=l-1;j++)
            {
                scanf("%d",&y);
                x=Find(x);
                y=Find(y);
                if(x!=y)
                {
                    father[x]=y;
                    k++;
                }
            }
        }
        sort(a,a+num,cmp);
        int MST=0;
        for(int i=0;i<num;i++)
        {
            int x=Find(a[i].x);
            int y=Find(a[i].y);
            if(x!=y)
            {
                father[x]=y;
                MST+=a[i].w;
                k++;
            }
            if(k==n-1)break;
        }
        if(k!=n-1)
            printf("-1\n");
        else
            printf("%d\n",MST);
   }
   return 0;
}
```






