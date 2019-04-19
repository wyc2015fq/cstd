# hdu  3478 Catch（判断奇数环） - HJ - CSDN博客
2017年10月16日 21:25:33[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：289
[http://acm.split.hdu.edu.cn/showproblem.php?pid=3478](http://acm.split.hdu.edu.cn/showproblem.php?pid=3478)
题目大意： 
判断任意时刻，任意一个点是否都可以到达。
解题思路：要让从起点到任何点，那么就要判断是否存在奇数环，如果不存在奇数环，那么偶数步数的才能到达的点就不符合题目要求，即无解。因此就可以转变成判断是否存在奇数环，而如果所有点组成一个奇数环，那么所有点组成的图就是一个二分图，也就是判断这个图是否为二分图，用图的二染色解即可。
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<queue>
using namespace std;
const int maxn=1000005;
vector<int>g[maxn];
int n,m,s,vis[maxn];
bool bfs()
{
    int blag=0;
    queue<int>q;
    memset(vis,0,sizeof(vis));
    q.push(s);
    vis[s]=1;
    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        for(int i=0;i<g[u].size();i++)
        {
            int v=g[u][i];
            if(vis[v]==0)
            {
                vis[v]=-vis[u];
                q.push(v);
            }
            else if(vis[v]==vis[u])
              blag=1; 
        }
    }   
    for(int i=0;i<n;i++)
      if(vis[i]==0)
        return false;
    return blag;
}
int main()
{
    int t,tt=1,u,v;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d%d%d",&n,&m,&s);
        for(int i=0;i<n;i++)
          g[i].clear();
        while(m--)
        {
            scanf("%d%d",&u,&v);
            g[u].push_back(v);
            g[v].push_back(u);
        }
        printf("Case %d: %s\n",tt++,bfs()?"YES":"NO");
    } 
}
```
