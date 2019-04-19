# C++实现SPFA判断负环算法 - fanyun的博客 - CSDN博客
2018年08月26日 14:29:10[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：266
1、SPFA判断负环算法要求
          给定每条街的拥挤度p(x)，街a到街b的时间就是(p(b)-p(a))**3，求第一个点到第k个点的最短路，若无法到达或结果小于3，输出’?’。
2、算法思路
         显然，题目可能存在负环，则所有负环上的点全应该输出’?’ ，因为它们必定小于3，所以，spfa判断负环，并进行标记，即可解决。
3、代码实现
```cpp
#include<iostream>
#include<cstring>
#include<cmath>
#include<cstdio>
#include<algorithm>
#include<stack>
#include<queue>
using namespace std;
const int N = 209;
const int MAX = 0x3f3f3f3f;
int dis[N];
int p[N];
int h[N];
int cnt[N];
bool vis[N];
bool r[N];
int c[N];
struct Node
{
    int u, v, w, next;
}e[N*N];
void dfs(int x)
{
    r[x] = 1;
    for(int i=h[x]; i!=-1; i=e[i].next)
        if(!r[e[i].v])
            dfs(e[i].v);
}
void spfa(int n, int Q)
{
    memset(cnt, 0, sizeof(cnt));
    memset(vis, 0, sizeof(vis));
    memset(r, 0, sizeof(r));
    memset(dis, 0x3f3f3f3f, sizeof(dis));
    queue<int> q;
    q.push(1);
    dis[1] = 0;
    while(!q.empty())
    {
        int x = q.front();
        vis[x] = 0;
        q.pop();
        for(int i=h[x]; i!=-1; i=e[i].next)
        {
            int v = e[i].v;
            if(r[v])
                continue;
            if(dis[v] > dis[x] + e[i].w)
            {
                dis[v] = dis[x] + e[i].w;
                if(!vis[v])
                {
                    vis[v] = 1;
                    q.push(v);
                    if(++cnt[v] >= n)
                        dfs(v);
                }
            }
        }
    }
    for(int i=0; i<Q; i++)
    {
        int to = c[i];
        if(dis[to]==MAX || r[to] || dis[to] < 3)
            printf("?\n");
        else
            printf("%d\n", dis[to]);
    }
}
int main()
{
    int T;
    scanf("%d", &T);
    for(int tt=1; tt<=T; tt++)
    {
        int n, m;
        scanf("%d", &n);
        for(int i=1; i<=n; i++)
            scanf("%d", &p[i]);
        scanf("%d", &m);
        memset(h, -1, sizeof(h));
        for(int i=0; i<m; i++)
        {
            int a, b;
            scanf("%d%d", &a, &b);
            e[i].u = a;
            e[i].v = b;
            e[i].w = (p[b]-p[a])*(p[b]-p[a])*(p[b]-p[a]);
            e[i].next = h[a];
            h[a] = i;
        }
        int Q;
        scanf("%d", &Q);
        for(int i=0; i<Q; i++)
            scanf("%d", &c[i]);
        printf("Case %d:\n", tt);
        spfa(n, Q);
    }
    return 0;
}
```
