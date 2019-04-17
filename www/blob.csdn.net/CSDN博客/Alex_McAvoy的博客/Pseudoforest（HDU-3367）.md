# Pseudoforest（HDU-3367） - Alex_McAvoy的博客 - CSDN博客





2018年05月21日 21:39:10[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：47
个人分类：[图论——最小生成树																[HDU](https://blog.csdn.net/u011815404/article/category/7923257)](https://blog.csdn.net/u011815404/article/category/7813283)








> 
# Problem Description

In graph theory, a pseudoforest is an undirected graph in which every connected component has at most one cycle. The maximal pseudoforests of G are the pseudoforest subgraphs of G that are not contained within any larger pseudoforest of G. A pesudoforest is larger than another if and only if the total value of the edges is greater than another one’s.

# **Input**

The input consists of multiple test cases. The first line of each test case contains two integers, n(0 < n <= 10000), m(0 <= m <= 100000), which are the number of the vertexes and the number of the edges. The next m lines, each line consists of three integers, u, v, c, which means there is an edge with value c (0 < c <= 10000) between u and v. You can assume that there are no loop and no multiple edges.

The last test case is followed by a line containing two zeros, which means the end of the input.

# Output

Output the sum of the value of the edges of the maximum pesudoforest.

# Sample Input

**3 3**

**0 1 11 2 12 0 14 50 1 11 2 12 3 13 0 10 2 20 0**

# Sample Output

**35**


题意：每一个联通子集顶多只能有一个环，在给定边里求满足条件的最大总长度

思路

    类似最大生成树，先将边按照从大到小排序。

    ①若两点不连通：当x、y都在环中时，合并后环数大于1，不合题意；当x或y在环中时，合并，并将x和y标记

    ②若两点连通：当x或y在环中时，合并后环数大于1，不合题意；当x、y都不在环中时，合并，构成一个新环，并将x和y标记。

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
#define N 1000001
#define MOD 1000000007
#define E 1e-12
using namespace std;
int father[N];
int roundd[N];
struct Node
{
    int s;
    int e;
    int w;
}a[N];

int cmp(Node a,Node b)
{
    return a.w>b.w;
}
int Find(int x)
{
    if(x!=father[x])
        father[x]=Find(father[x]);
    return father[x];
}

int main()
{
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF&&(n || m))
    {
        int MST=0;
        memset(roundd,0,sizeof(roundd));
        for(int i=0; i<=n; i++)
            father[i]=i;

        for(int i=0; i<m; i++)
            scanf("%d%d%d",&a[i].s,&a[i].e,&a[i].w);
        sort(a,a+m,cmp);

        for(int i=0;i<m;i++)
        {
            int x=Find(a[i].s);
            int y=Find(a[i].e);
            if(x!=y)
            {
                if(roundd[x]&&roundd[y])
                    continue;
                MST+=a[i].w;
                father[x]=y;
                if(roundd[x]||roundd[y])
                {
                    roundd[x]=1;
                    roundd[y]=1;
                }
            }
            else
            {
                if(roundd[x]||roundd[y])
                    continue;
                MST+=a[i].w;
                roundd[x]=1;
                roundd[y]=1;
            }
        }
        printf("%d\n",MST);
    }
    return 0;
}
```






