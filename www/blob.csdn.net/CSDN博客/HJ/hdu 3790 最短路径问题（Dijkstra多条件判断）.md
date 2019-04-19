# hdu 3790 最短路径问题（Dijkstra多条件判断） - HJ - CSDN博客
2017年10月09日 20:59:50[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：302
[http://acm.split.hdu.edu.cn/showproblem.php?pid=3790](http://acm.split.hdu.edu.cn/showproblem.php?pid=3790)
开结构体保存距离（len）和费用（cost）两个变量，在用Dijkstra求最短路更新
**d[y] = min{ d[y] , d[x] + w(x,y) };**
的时候，分两种情况：
**①d[y].len > d[x].len + len(x,y)；**
**②d[y].len == d[x].len + len(x,y)；**
其他不变就行了。注意初始值的赋值！
```
#include<iostream>
#include<cstdio>
#define INF 0x3f3f3f3f
using namespace std;
const int maxn=1005;
struct edge{
    int len,cost;
};
int n,m,s,t;
edge g[maxn][maxn],d[maxn];
bool vis[maxn];
void Dijkstra()
{
    for(int i=0;i<n;i++)
    {
        vis[i]=false;           //访问标记初始化 
        d[i]=g[s][i];         //结构体变量赋值 
    }
    d[s].len=d[s].cost=0;
    vis[s]=true;
    for(int i=1;i<n;i++)
    {
        int mind=INF,mark;
        for(int j=0;j<n;j++)
          if(!vis[j]&&d[j].len<mind)
            mind=d[mark=j].len;
        vis[mark]=true;
        for(int j=0;j<n;j++)
        {
            if(!vis[j])
            {               
                if(d[j].len>d[mark].len+g[mark][j].len) 
                {
                    d[j].len=d[mark].len+g[mark][j].len;
                    d[j].cost=d[mark].cost+g[mark][j].cost;
                }
                else if(d[j].len==d[mark].len+g[mark][j].len)    //相同距离 比较路费 
                   d[j].cost=min(d[j].cost,d[mark].cost+g[mark][j].cost);
            }
        }
    }
}
int main()
{
    while(scanf("%d%d",&n,&m)&&(n||m))
    {
        int u,v,len,cost;
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
              g[i][j].len=g[i][j].cost=INF;             //初始值为无穷大 
            g[i][i].len=g[i][i].cost=0;          //自己到自己的值为0 
        }
        while(m--)
        {
            scanf("%d%d%d%d",&u,&v,&len,&cost);
            u--,v--;
            if(g[u][v].len>len)        //根据距离赋值（费用只是在相同距离的时候才起作用） 
            {
                g[u][v].len=g[v][u].len=len;
                g[u][v].cost=g[v][u].cost=cost;
            }
        }
        scanf("%d%d",&s,&t);
        s--,t--;
        Dijkstra();
        printf("%d %d\n",d[t].len,d[t].cost);
    }
    return 0;
}
```
