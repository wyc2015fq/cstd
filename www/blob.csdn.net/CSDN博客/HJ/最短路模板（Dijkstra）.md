# 最短路模板（Dijkstra） - HJ - CSDN博客
2018年05月08日 20:44:03[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：77
普通版
```
int d[N],vis[N],g[N][N];
int n,m,a,b,len;
void Dijkstra(int s)
{
    for(int i=1;i<=n;i++)
      d[i]=g[s][i];
    memset(vis,0,sizeof(vis));
    vis[s]=1;
    for(int i=1;i<=n;i++)
    {
        int mind=INF,mark;
        for(int j=1;j<=n;j++)
          if(!vis[j]&&d[j]<mind)
            mind=d[mark=j];
        vis[mark]=1;
        for(int j=1;j<=n;j++)
          if(!vis[j]&&d[mark]+g[mark][j]<d[j])
            d[j]=d[mark]+g[mark][j];
    }
}
```
优先队列优化版
```cpp
const int N=1005;
struct node{
    int v,len;
    bool operator < (const node&a)const{
        return len<a.len;
    } 
};
vector<node>G[N];
int n,m,s,d[N],vis[N];
void Dijkstra()
{
    memset(vis,0,sizeof(vis));
    for(int i=0;i<=n;i++)
      d[i]=INF;
    d[0]=0;
    priority_queue<node> Q;
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
```
