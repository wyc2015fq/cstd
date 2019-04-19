# hdu 2066  一个人的旅行（Dijkstra） - HJ - CSDN博客
2017年10月09日 21:16:40[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：202
[http://acm.split.hdu.edu.cn/showproblem.php?pid=2066](http://acm.split.hdu.edu.cn/showproblem.php?pid=2066)
注意建图的方法：把0看做一个大的源点，直接相连的权值为0
最后计算从0到各个点的最短路的最小的那个值即可。
```cpp
#include<iostream>
#include<cstring>
#define INF 0x3f3f3f3f
using namespace std;
const int maxn=1005;
int n,e[maxn],d[maxn],g[maxn][maxn],vis[maxn];
void Dijkstra()
{
    for(int i=0;i<=n;i++)
      d[i]=g[0][i];
    memset(vis,0,sizeof(vis));
    vis[0]=1;
    for(int i=1;i<=n;i++)
    {
        int mind=INF,mark;
        for(int j=1;j<=n;j++)
          if(!vis[j]&&d[j]<mind)
            mind=d[mark=j];
        vis[mark]=1;
        for(int j=1;j<=n;j++)
          if(!vis[j])
            d[j]=min(d[j],d[mark]+g[mark][j]);
    }
}
int main()
{
    int m,a,b,u,v,w,x;
    while(cin>>m>>a>>b)
    {
        memset(g,INF,sizeof(g));
        for(int i=0;i<1001;i++)
          g[i][i]=0;
        while(m--)
        {
            cin>>u>>v>>w;
            n=max(max(n,u),v);        //找出最大的那个点  
            if(g[u][v]>w)
              g[u][v]=g[v][u]=w;
        }
        int minn=INF;
        while(a--)
        {
            cin>>x;
            g[0][x]=g[x][0]=0;          //0为超级源点 到与它相邻的点的距离都为0 
        }
        for(int i=0;i<b;i++)
          cin>>e[i];
        Dijkstra();            //计算出从0（源点）到各个点的最短路 
        for(int i=0;i<b;i++)
          minn=min(minn,d[e[i]]);         //遍历找出最小的最短距离 
        cout<<minn<<endl;
    }
    return 0;
}
```
