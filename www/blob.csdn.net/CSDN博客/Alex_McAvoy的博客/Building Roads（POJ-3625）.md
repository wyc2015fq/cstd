# Building Roads（POJ-3625） - Alex_McAvoy的博客 - CSDN博客





2018年06月29日 01:17:51[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：59








> 
# Problem Description

Farmer John had just acquired several new farms! He wants to connect the farms with roads so that he can travel from any farm to any other farm via a sequence of roads; roads already connect some of the farms.

Each of the N (1 ≤ N ≤ 1,000) farms (conveniently numbered 1..N) is represented by a position (Xi, Yi) on the plane (0 ≤ Xi ≤ 1,000,000; 0 ≤ Yi ≤ 1,000,000). Given the preexisting M roads (1 ≤ M ≤ 1,000) as pairs of connected farms, help Farmer John determine the smallest length of additional roads he must build to connect all his farms.

# **Input**

Line 1: Two space-separated integers: N and M

Lines 2..N+1: Two space-separated integers: Xi and Yi 

Lines N+2..N+M+2: Two space-separated integers: i and j, indicating that there is already a road connecting the farm i and farm j.

# Output

Line 1: Smallest length of additional roads required to connect all farms, printed without rounding to two decimal places. Be sure to calculate distances as 64-bit floating point numbers.

# Sample Input

**4 1**

**1 13 12 34 31 4**

# Sample Output

**4.00**


题意：给出 n 个村庄及其坐标，m 条两村庄间已修好的路，求 n 个村庄互相连通需要修建的最短道路。

思路：最小生成树，套用模版即可，需要注意的地方是数据类型。

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
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 1001
#define MOD 123
#define E 1e-6
using namespace std;
double x[N],y[N];
double g[N][N];
double dis[N];
bool vis[N];
double calculate(double x1,double y1,double x2,double y2)
{
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

int main()
{
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF)
    {
        for(int i=0;i<n;i++)
            scanf("%lf%lf",&x[i],&y[i]);

        for(int i=0;i<n;i++)//记录任意两点距离
            for(int j=0;j<n;j++)
                g[i][j]=calculate(x[i],y[i],x[j],y[j]);

        while(m--)
        {
            int a,b;
            scanf("%d%d",&a,&b);
            a--;
            b--;
            g[a][b]=g[b][a]=0;//已建好的路赋0值
        }

        memset(vis,0,sizeof(vis));
        vis[0]=1;
        for(int i=1;i<n;i++)
            dis[i]=g[0][i];

        double sum=0;
        for(int i=1;i<n;i++)
        {
            int k=-1;
            double minn=9999999999.0;
            for(int j=0; j<n; j++)
                if(!vis[j]&&dis[j]<minn)
                {
                    minn=dis[j];
                    k=j;
                }

            sum+=minn;
            vis[k]=1;
            for(int j=0;j<n;j++)
                if(!vis[j]&&dis[j]>g[k][j])
                    dis[j]=g[k][j];
        }
        printf("%.2f\n",sum);
    }
    return 0;
}
```





