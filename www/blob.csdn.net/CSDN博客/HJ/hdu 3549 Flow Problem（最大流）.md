# hdu 3549 Flow Problem（最大流） - HJ - CSDN博客
2017年10月09日 21:26:25[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：178
[http://acm.split.hdu.edu.cn/showproblem.php?pid=3549](http://acm.split.hdu.edu.cn/showproblem.php?pid=3549)
给你一个N个顶点M条边的有向图，要你求1号点到N号点的最大流。最大流的EK模板。
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<queue>
#define INF 0x3f3f3f3f
using namespace std;
const int maxn=20;
struct Network_Flow{
    int n;
    int flow[maxn][maxn];
    int cap[maxn][maxn];
    void init(int n)
    {
        this->n=n;
        memset(cap,0,sizeof(cap));
    }
    int BFS(int s,int t)
    {
        queue<int>q;
        memset(flow,0,sizeof(flow));
        int ans=0;         //最大流 
        int a[maxn];         //a[i]表示从s到i点的最小残量 
        int p[maxn];       //增广路上一节点 
        while(true)
        {
            memset(a,0,sizeof(a));
            a[s]=INF;
            q.push(s);
            while(!q.empty())          //BFS查找增广路   a[t]保存增广路上的最小残量 
            {
                int u=q.front();
                q.pop();
                for(int v=1;v<=n;v++)
                  if(!a[v]&&cap[u][v]>flow[u][v])
                  {
                      p[v]=u;
                      q.push(v);
                      a[v]=min(a[u],cap[u][v]-flow[u][v]);
                  }
            }
            if(a[t]==0)        //如果最小残量为0 则代表不存在增广路 即可退出 
              break;
            for(int u=t;u!=s;u=p[u])          //更新残留网络的值（含反向弧） 
            { 
                flow[p[u]][u]+=a[t];
                flow[u][p[u]]-=a[t];
            }
            ans+=a[t];
        } 
        return ans;
    }
}EK;
int main()
{
    int t,tt=0,n,m,u,v,w;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d%d",&n,&m);
        EK.init(n);
        while(m--)
        {
            scanf("%d%d%d",&u,&v,&w);
            EK.cap[u][v]+=w;
        }
        printf("Case %d: %d\n",++tt,EK.BFS(1,n));
    }
    return 0;
}
```
