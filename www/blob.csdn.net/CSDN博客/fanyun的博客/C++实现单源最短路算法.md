# C++实现单源最短路算法 - fanyun的博客 - CSDN博客
2018年08月26日 14:29:22[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：152
1、单源最短路算法
        n个处理器，第一个处理器要广播消息到其他所有的处理器，求需要时间最短是多少（从第一个点出发，求到其他点最短路的最大值）
2、思路
       这个基本上没啥可说。看代码理解就是。
3、代码实现
```cpp
#include<iostream>
#include<cstring>
#include<cmath>
#include<cstdio>
#include<algorithm>
#include<queue>
using namespace std;
const int N = 509;
const int MAX = 0x3f3f3f3f;
int d[N];
bool vis[N];
int v[N][N];
int dijkstra(int n)
{
    memset(vis, 0, sizeof(vis));
    for(int i=1; i<=n; i++)
        d[i] = v[1][i];
    d[1] = 0;
    vis[1] = 1;
    for(int i=1; i<n; i++)
    {
        int x = -1, mmin = MAX;
        for(int j=1; j<=n; j++)
            if(!vis[j] && d[j] < mmin)
                mmin = d[x = j];
        if(x == -1)
            break;
        vis[x] = 1;
        for(int j=1; j<=n; j++)
        {
            if(!vis[j])
                d[j] = min(d[j], d[x] + v[x][j]);
        }
    }
    int mmax = 0;
    for(int j=1; j<=n; j++)
        mmax = max(mmax, d[j]);
    return mmax;
}
int main()
{
    int n;
    scanf("%d", &n);
    memset(v, 0x3f3f3f3f, sizeof(v));
    for(int i=2; i<=n; i++)
    {
        for(int j=1; j<i; j++)
        {
            char t[20];
            scanf("%s", t);
            if(t[0] != 'x')
            {
                int k = 0;
                int x = 0;
                while(t[k] != '\0')
                    x = x*10 + t[k++]-'0';
                v[i][j] = v[j][i] = x;
            }
        }
    }
    printf("%d\n", dijkstra(n));
    return 0;
}
```
