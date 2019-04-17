# A Walk Through the Forest（HDU-1142） - Alex_McAvoy的博客 - CSDN博客





2018年05月21日 21:35:10[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：44








> 
# Problem Description

Jimmy experiences a lot of stress at work these days, especially since his accident made working difficult. To relax after a hard day, he likes to walk home. To make things even nicer, his office is on one side of a forest, and his house is on the other. A nice walk through the forest, seeing the birds and chipmunks is quite enjoyable. 

The forest is beautiful, and Jimmy wants to take a different route everyday. He also wants to get home before dark, so he always takes a path to make progress towards his house. He considers taking a path from A to B to be progress if there exists a route from B to his home that is shorter than any possible route from A. Calculate how many different routes through the forest Jimmy might take. 

# **Input**

Input contains several test cases followed by a line containing 0. Jimmy has numbered each intersection or joining of paths starting with 1. His office is numbered 1, and his house is numbered 2. The first line of each test case gives the number of intersections N, 1 < N ≤ 1000, and the number of paths M. The following M lines each contain a pair of intersections a b and an integer distance 1 ≤ d ≤ 1000000 indicating a path of length d between intersection a and a different intersection b. Jimmy may walk a path any direction he chooses. There is at most one path between any pair of intersections. 

# Output

For each test case, output a single integer indicating the number of different routes through the forest. You may assume that this number does not exceed 2147483647

# Sample Input

**5 6**

**1 3 21 4 23 4 31 5 124 2 345 2 247 81 3 11 4 13 7 17 4 17 5 16 7 15 2 16 2 10**

# Sample Output

**24**


题意：一个人办公室在点1，家在点2，要从办公室回家，已知从点A到点B当且仅当从B到家的距离一定比从任意一点A到家的都小，求他回家路线的方案数

思路：Dijkstra算法找出到家的最短路，然后从办公室开始深度优先搜索即可

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
#define INF 999999999
#define N 1001
#define MOD 1000000007
#define E 1e-12
using namespace std;
int n, m;
int g[N][N],dis[N];
bool vis[N];
int dp[N];

int Dfs(int x)
{
    if(dp[x]!=0)
        return dp[x];
    if(x==2)
        return 1;

    int temp=0;
    for(int i=1;i<=n;i++)
        if(dis[x]>dis[i] && g[x][i]!=INF)
            temp+=Dfs(i);
    dp[x]=temp;
    return dp[x];
}

int main()
{
    while(scanf("%d",&n)!= EOF&&n)
    {
        scanf("%d",&m);
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
                g[i][j]=INF;
        for(int i=0;i<m;i++)
        {
            int x,y,w;
            scanf("%d%d%d",&x,&y,&w);
            g[x][y]=w;
            g[y][x]=w;
        }

        /*Dijkstra求最短路*/
        memset(vis,0,sizeof(vis));
        for(int i=1;i<=n;i++)
            dis[i]=g[2][i];
        vis[2]=1;
        dis[2]=0;
        for(int i=1;i<n;i++)
        {
            int k=0,minn=INF;
            for(int j=1;j<=n;j++)
            {
                if(!vis[j] && dis[j]<minn)
                {
                    minn=dis[j];
                    k=j;
                }
            }
            vis[k]=1;
            for(int j=1;j<=n;j++)
                if(vis[j]==0)
                    dis[j]=min(dis[j],dis[k]+g[k][j]);
        }

        /*dfs统计回家路数*/
        int res;
        memset(dp,0,sizeof(dp));
        res=Dfs(1);

        printf("%d\n",res);
    }
    return 0;
}
```





