# hdu 3339 In Action （Dijkstra+01背包） - HJ - CSDN博客
2017年10月11日 19:12:46[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：143
[http://acm.split.hdu.edu.cn/showproblem.php?pid=3339](http://acm.split.hdu.edu.cn/showproblem.php?pid=3339)
题目大意： 
给出n个发电站，m条路，每条路都要消耗一定的油，每个发电站都有一定的能量，要注意，每个发电站必须停一辆坦克才能控制这个发电站，只有控制的总能量超过一半能使其瘫痪。问使其瘫痪的最少要消耗多少油？
解题思路： 
分两步，第一步计算出从0出发到n个点每个点的最短路，第二部从n个中以能量为体积（总体积即总能量为sum），最短路大小为价值，从[sum/2+1,sum]中找出最小值即可。
Dijkstra优先队列优化+01背包
```cpp
#include<iostream>
#include<queue> 
#include<cstdio>
#include<cstring>
#define INF 0x3f3f3f3f
using namespace std;
const int maxn=105;
int n,m,sum[2000005],vis[maxn],d[maxn],g[maxn][maxn],cost[maxn];
struct node{
    int v,len;
    bool operator < (const node&a)const{
        return len>a.len;
    }
};
void Dijkstra()     //Dijkstra优先队列优化 
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
        for(int i=0;i<=n;i++)
        {
            if(!vis[i]&&d[i]>d[u]+g[u][i])
            {
                d[i]=d[u]+g[u][i];
                Q.push(node{i,d[i]});
            }
        }
    }   
} 
void DP()          //01背包 
{
    int ans=0,res=INF;
    for(int i=1;i<=n;i++)
    { 
        cin>>cost[i];
        ans+=cost[i];
    }   
    for(int i=1;i<=ans;i++)     //赋初值 
      sum[i]=INF;
    sum[0]=0;
    for(int i=1;i<=n;i++)              //01背包 选择若干个能量站 保证控制的能量超过一半 
      for(int j=ans;j>=cost[i];j--)
        sum[j]=min(sum[j],sum[j-cost[i]]+d[i]);
    for(int i=ans/2+1;i<=ans;i++)           //找出消耗油量最少的方案 
      res=min(res,sum[i]); 
    if(res==INF)      //如果找不到 res的值仍然为无穷大 
      printf("impossible\n");
    else
      printf("%d\n",res);
}
int main()
{
    int t,u,v,w;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d%d",&n,&m);
        for(int i=0;i<=n;i++)
          for(int j=0;j<=n;j++)
            g[i][j]=(i==j)?0:INF; 
        while(m--)
        {
            scanf("%d%d%d",&u,&v,&w);
            if(g[u][v]>w)
              g[u][v]=g[v][u]=w;
        }       
        Dijkstra();
        DP();   
    }
    return 0;
}
```
