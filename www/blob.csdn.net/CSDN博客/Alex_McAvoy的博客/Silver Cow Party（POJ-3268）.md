# Silver Cow Party（POJ-3268） - Alex_McAvoy的博客 - CSDN博客





2018年06月26日 20:35:00[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：57








> 
# Problem Description

One cow from each of N farms (1 ≤ N ≤ 1000) conveniently numbered 1..N is going to attend the big cow party to be held at farm #X (1 ≤ X ≤ N). A total of M (1 ≤ M ≤ 100,000) unidirectional (one-way roads connects pairs of farms; road i requires Ti (1 ≤ Ti ≤ 100) units of time to traverse.

Each cow must walk to the party and, when the party is over, return to her farm. Each cow is lazy and thus picks an optimal route with the shortest time. A cow’s return route might be different from her original route to the party since roads are one-way.

Of all the cows, what is the longest amount of time a cow must spend walking to the party and back?

# **Input**

Line 1: Three space-separated integers, respectively: N, M, and X 

Lines 2.. M+1: Line i+1 describes road i with three space-separated integers: Ai, Bi, and Ti. The described road runs from farm Ai to farm Bi, requiring Ti time units to traverse. 

# Output

Line 1: One integer: the maximum of time any one cow must walk. 


# Sample Input

**4 8 2 **

**1 2 4 1 3 2 1 4 7 2 1 1 2 3 5 3 1 2 3 4 4 4 2 3 **

# Sample Output

**10**


题意：给出 n 个点，m 条路，除 x 点外的其他点都要有一头牛到达 x 点，求往返路程的最小值。

思路：先以所给边为正向，x 为起点求最短路，再把边反着读一遍作为反向，以x为起点求最短路。把两次求最短路的数组加起来，找最大值即可。

# **Source Program**

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
int g[N][N];
int dis_positive[N],vis_positive[N];//各点到x
int dis_negative[N],vis_negative[N];//x到各点
int main()
{
    int n,m,x;
    scanf("%d%d%d",&n,&m,&x);

    memset(g,INF,sizeof(g));
    for(int i=1;i<=N;i++)
        for(int j=1;j<=N;j++)
            if(i==j)
                g[i][j]=0;

    while(m--)
    {
        int x,y,w;
        scanf("%d%d%d",&x,&y,&w);
        if(g[x][y]>w)
            g[x][y]=w;
    }

    for(int i=1;i<=n;i++)
    {
        /*各点到x*/
        dis_positive[i]=g[i][x];
        vis_positive[i]=0;

        /*x到各点*/
        dis_negative[i]=g[x][i];
        vis_negative[i]=0;
    }
    vis_positive[x]=1;
    vis_negative[x]=1;

    for(int i=1;i<n;i++)//各点到x
    {
        int k;
        int minn=INF;
        for(int j=1;j<=n;j++)
            if( !vis_positive[j] && dis_positive[j]<minn )
            {
                minn=dis_positive[j];
                k=j;
            }

        if(minn==INF)
            break;

        vis_positive[k]=1;
        for(int j=1;j<=n;j++)
            if( !vis_positive[j] && dis_positive[j]>dis_positive[k]+g[j][k] )
                dis_positive[j]=dis_positive[k]+g[j][k];
    }

    for(int i=1;i<n;i++)//x到各点
    {
        int k;
        int minn=INF;
        for(int j=1;j<=n;j++)
            if( !vis_negative[j] && dis_negative[j]<minn )
            {
                minn=dis_negative[j];
                k=j;
            }

        if(minn==INF)
            break;

        vis_negative[k]=1;
        for(int j=1; j<=n; j++)
            if( !vis_negative[j] && dis_negative[j]>dis_negative[k]+g[k][j] )
                dis_negative[j]=dis_negative[k]+g[k][j];
    }

    int maxx=-INF;
    for(int i=1;i<=n;i++)
        maxx=max(maxx,dis_negative[i]+dis_positive[i]);

    printf("%d\n",maxx);
    return 0;
}
```





