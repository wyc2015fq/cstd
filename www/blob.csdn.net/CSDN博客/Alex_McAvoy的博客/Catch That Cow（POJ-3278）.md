# Catch That Cow（POJ-3278） - Alex_McAvoy的博客 - CSDN博客





2018年06月02日 01:23:12[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：61
个人分类：[POJ																[搜索——广度优先搜索 BFS](https://blog.csdn.net/u011815404/article/category/8115771)](https://blog.csdn.net/u011815404/article/category/7504500)








# Problem Description

Farmer John has been informed of the location of a fugitive cow and wants to catch her immediately.

He starts at a point N (0 ≤ N ≤ 100,000) on a number line and the cow is at a point K (0 ≤ K ≤ 100,000) on the same number line.

Farmer John has two modes of transportation: walking and teleporting.

* Walking: FJ can move from any point X to the points X - 1 or X + 1 in a single minute

* Teleporting: FJ can move from any point X to the point 2 × X in a single minute.

If the cow, unaware of its pursuit, does not move at all, how long does it take for Farmer John to retrieve it?

# **Input**

Line 1: Two space-separated integers: N and K

# Output

Line 1: The least amount of time, in minutes, it takes for Farmer John to catch the fugitive cow.

# Sample Input

**5 17**

# Sample Output

**4**

———————————————————————————————————————————————————————



题意：在一数轴上，农夫位置是N，牛的位置是K，牛不会移动，农夫移动想要抓住牛。农夫有两种移动方式：从X到X-1或X+1，花费一分钟；从X到2*X，花费一分钟，求抓住牛的最小分钟数。

    具体参见：[点击这里](https://blog.csdn.net/u011815404/article/details/80488217)

思路：bfs搜索入门题

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
#define N 100001
#define MOD 2520
#define E 1e-12
using namespace std;
bool vis[N];
int dir[2]={-1,1};
struct node
{
    int x;
    int step;
}q[N*10];
void bfs(int n,int k)
{
    int head=1,tail=1;
    memset(vis,0,sizeof(vis));

    vis[n]=1;
    q[tail].x=n;
    q[tail].step=0;
    tail++;

    while(head<tail)
    {
        int x=q[head].x;
        int step=q[head].step;
        int nx;

        if(x==k)
        {
            printf("%d\n",step);
            break;
        }

        /*第一种走法*/
        for(int i=0;i<2;i++)
        {
            nx=x+dir[i];
            if(0<=nx&&nx<N&&vis[nx]==0)
            {
                vis[nx]=1;
                q[tail].x=nx;
                q[tail].step=step+1;
                tail++;
            }
        }

        /*第二种走法*/
        nx=x*2;
        if(nx>=0&&nx<N&&vis[nx]==0)
        {
            vis[nx]=1;
            q[tail].x=nx;
            q[tail].step=step+1;
            tail++;
        }
        head++;
    }
}
int main()
{
    int n,k;
    scanf("%d%d",&n,&k);
    if(k<n)
    {
        printf("%d",n-k);
        exit(0);
    }
    bfs(n,k);

    return 0;
}
```






