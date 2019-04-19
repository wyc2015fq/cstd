# hdu 2680 Choose the best route（SPFA或者Dijkstra） - HJ - CSDN博客
2017年10月10日 21:16:23[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：131
个人分类：[====ACM====																[---------OJ---------																[HDU																[----------图论---------																[最短路](https://blog.csdn.net/feizaoSYUACM/article/category/6307997)](https://blog.csdn.net/feizaoSYUACM/article/category/6308029)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.split.hdu.edu.cn/showproblem.php?pid=2680](http://acm.split.hdu.edu.cn/showproblem.php?pid=2680)
把0看成一个大源点。走一遍最短路就行了。
Dijkstra优先队列优化
```cpp
#include<iostream>
#include<cstring>
#include<cstdio> 
#include<queue>
#include<vector>
#define INF 0x3f3f3f3f
using namespace std;
const int maxn=1005;
struct node{
    int v,len;
    bool operator < (const node&a)const{
        return len>a.len;
    }
};
vector<node>G[maxn];
int n,m,s,d[maxn],vis[maxn];
void Dijkstra()
{
    memset(vis,0,sizeof(vis));
    for(int i=0;i<=n;i++)
      d[i]=INF;
    d[0]=0;
    priority_queue<node>Q;
    Q.push(node{0,0});
    while(!Q.empty())
    {
        node now=Q.top();
        Q.pop();
        int u=now.v;
        if(vis[u])
          continue;
        vis[u]=1;
        for(int i=0;i<G[u].size();i++)
        {
            int v=G[u][i].v,w=G[u][i].len;
            if(!vis[v]&&d[v]>d[u]+w)
            {
                d[v]=d[u]+w;
                Q.push(node{v,d[v]});
            }
        } 
    } 
}
int main()
{
    while(~scanf("%d%d%d",&n,&m,&s))
    {
        int u,v,w,x;
        for(int i=0;i<=n;i++)
          G[i].clear();
        while(m--)
        {
            scanf("%d%d%d",&u,&v,&w);
            G[u].push_back(node{v,w});
        }
        scanf("%d",&m);
        while(m--)
        {
            scanf("%d",&x);
            G[0].push_back(node{x,0});
        }
        Dijkstra();
        if(d[s]==INF)
          printf("-1\n");
        else
          printf("%d\n",d[s]);
    }
    return 0;
}
```
SPFA
```cpp
#include<iostream>
#include<cstring>
#include<cstdio> 
#include<queue>
#include<vector>
#define INF 0x3f3f3f3f
using namespace std;
const int maxn=1005;
struct node{
    int to,w;
};
vector<node>G[maxn];
int n,m,s,d[maxn],vis[maxn];
void SPFA(int src)
{
    for(int i=0;i<=n;i++)
    {
        d[i]=INF;
        vis[i]=false;
    }
    d[src]=0;
    vis[src]=true;
    queue<int>q;
    q.push(src);
    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        vis[u]=false;
        for(int i=0;i<G[u].size();i++)
        {
            int v=G[u][i].to,w=G[u][i].w;
            if(d[v]>d[u]+w)
            {
                d[v]=d[u]+w;
                if(!vis[v])
                {
                    vis[v]=true;
                    q.push(v);
                }
            }
        }
    }
}
int main()
{
    while(~scanf("%d%d%d",&n,&m,&s))
    {
        int u,v,w,x;
        for(int i=0;i<=n;i++)
          G[i].clear();
        while(m--)
        {
            scanf("%d%d%d",&u,&v,&w);
            G[u].push_back({v,w});
        }
        scanf("%d",&m);
        while(m--)
        {
            scanf("%d",&x);
            G[0].push_back({x,0});
        }
        SPFA(0);
        if(d[s]==INF)
          printf("-1\n");
        else
          printf("%d\n",d[s]);
    }
    return 0;
}
```
