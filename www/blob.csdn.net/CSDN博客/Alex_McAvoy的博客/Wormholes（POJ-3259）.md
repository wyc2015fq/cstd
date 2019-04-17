# Wormholes（POJ-3259） - Alex_McAvoy的博客 - CSDN博客





2018年07月24日 22:47:33[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：44
个人分类：[POJ																[图论——BellmanFord_SPFA](https://blog.csdn.net/u011815404/article/category/8312431)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

While exploring his many farms, Farmer John has discovered a number of amazing wormholes. A wormhole is very peculiar because it is a one-way path that delivers you to its destination at a time that is BEFORE you entered the wormhole! Each of FJ's farms comprises N (1 ≤ N ≤ 500) fields conveniently numbered 1..N, M (1 ≤ M ≤ 2500) paths, and W (1 ≤ W ≤ 200) wormholes.

As FJ is an avid time-traveling fan, he wants to do the following: start at some field, travel through some paths and wormholes, and return to the starting field a time before his initial departure. Perhaps he will be able to meet himself :) .

To help FJ find out whether this is possible or not, he will supply you with complete maps to F (1 ≤ F ≤ 5) of his farms. No paths will take longer than 10,000 seconds to travel and no wormhole can bring FJ back in time by more than 10,000 seconds.

# **Input**

Line 1: A single integer, F. F farm descriptions follow. 

Line 1 of each farm: Three space-separated integers respectively: N, M, and W 

Lines 2..M+1 of each farm: Three space-separated numbers (S, E, T) that describe, respectively: a bidirectional path between S and E that requires T seconds to traverse. Two fields might be connected by more than one path. 

Lines M+2..M+W+1 of each farm: Three space-separated numbers (S, E, T) that describe, respectively: A one way path from S to E that also moves the traveler back T seconds.

# Output

Lines 1..F: For each farm, output "YES" if FJ can achieve his goal, otherwise output "NO" (do not include the quotes).

# Sample Input

**23 3 11 2 21 3 42 3 13 1 33 2 11 2 32 3 43 1 8**

# Sample Output

**NOYES**


题意：给出 n 个点，m 条双向路，w 条单向虫洞，求是否存在负环。

思路：以始点为源点，使用 Bellman-Ford 算法，即可判断图中是否存在负权回路。

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
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 10001
#define MOD 123
#define E 1e-6
using namespace std;
int n,w,m;
int dis[N];
int k;
struct Node{
    int start;
    int endd;
    int value;
}edge[N];
void add(int u,int v,int w)
{
    edge[k].start=u;
    edge[k].endd=v;
    edge[k].value=w;
    k++;
}
bool Ford(int s)
{
    memset(dis,INF,sizeof(dis));
    dis[s]=0;

    for(int i=0;i<n;i++)
    {
        bool flag=true;
        for(int j=0;j<k;j++)
        {
            int u=edge[j].start;
            int v=edge[j].endd;
            int w=edge[j].value;
            if(dis[u]+w<dis[v])
            {
                flag=false;
                dis[v]=dis[u]+w;
                if(i==n-1)
                    return true;
            }
        }

        if(flag)
            return false;
    }
    return false;
}
int main()
{
    int t;
    scanf("%d",&t);
    while(t--)
    {
        k=0;//初始化
        scanf("%d%d%d",&n,&m,&w);
        while(m--)//添加双向路到邻接表中
        {
            int u,v,w;
            scanf("%d%d%d",&u,&v,&w);
            add(u,v,w);
            add(v,u,w);
        }
        while(w--)//添加单向路到邻接表中
        {
            int u,v,w;
            scanf("%d%d%d",&u,&v,&w);
            add(u,v,-w);
        }

        if(Ford(1))
            printf("YES\n");
        else
            printf("NO\n");
    }
    return 0;
}
```






