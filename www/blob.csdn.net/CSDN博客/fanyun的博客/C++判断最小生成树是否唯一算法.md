# C++判断最小生成树是否唯一算法 - fanyun的博客 - CSDN博客
2018年08月26日 14:29:32[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：135
1、最小生成树是否唯一算法
        给定一无向图，判断最小生成树是否唯一。
2、思路
       先求出最小生成树，记录结果，依次删除树中各边，再求最小生成树，看与最初结果是否相同，若相同则不唯一。
3、代码实现
```cpp
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <queue>
#include <stack>
#include <vector>
using namespace std;
const int N = 109;
const int MAX = 100000000;
int d[N];
int g[N][N];
int fa[N];
pair<int, int> v[N];
int prim(int n, int m, bool f)
{
    for(int i=0;i<n;i++)
    {
        d[i] = g[0][i];
        fa[i] = 0;
    }
    d[0] = -1;
    int ans = 0;
    for(int i=1;i<n;i++)
    {
        int min = MAX, mini = -1;
        for(int j=0;j<n;j++)
        {
            if(d[j]!=-1 && d[j]<min)
            {
                min = d[j];
                mini = j;
            }
        }
        if(mini == -1)
            return -1;
        d[mini] = -1;
        if(f)
        {
            v[i].first = mini;
            v[i].second = fa[mini];
        }
        ans += min;
        for(int j=0;j<n;j++)
            if(d[j]!=-1 && d[j] > g[mini][j])
            {
                fa[j] = mini;
                d[j] = g[mini][j];
            }
    }
    return ans;
}
int main()
{
    int T;
    scanf("%d",&T);
    while(T--)
    {
        int n, m;
        memset(g, 0x3f, sizeof(g));
        scanf("%d%d",&n, &m);
        int a, b, c;
        for(int i=0;i<m;i++)
        {
            scanf("%d%d%d", &a, &b, &c);
            g[a-1][b-1] = g[b-1][a-1] = c;
        }
        int ans = prim(n, m, 1);
        for(int i=1;i<n;i++)
        {
            int t = g[v[i].first][v[i].second];
            g[v[i].first][v[i].second]
            = g[v[i].second][v[i].first] = MAX;
            if(ans == prim(n, m, 0))
            {
                ans = -1;
                break;
            }
            g[v[i].first][v[i].second]
            = g[v[i].second][v[i].first] = t;
        }
        if(ans == -1)
            printf("Not Unique!\n");
        else
            printf("%d\n",ans);
    }
    return 0;
}
```
