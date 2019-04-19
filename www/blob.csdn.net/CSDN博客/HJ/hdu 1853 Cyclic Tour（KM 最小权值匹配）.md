# hdu 1853 Cyclic Tour（KM 最小权值匹配） - HJ - CSDN博客
2017年10月16日 21:19:23[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：179
[http://acm.split.hdu.edu.cn/showproblem.php?pid=1853](http://acm.split.hdu.edu.cn/showproblem.php?pid=1853)
题目大意： 
n个城市里有m条单向路径，每条路径上有一个权值，每个城市都属于且仅属于某一个环，Tom计划环游这n个城市，并且每个城市都只能经过一次。问最后环游了n个城市后最小的权值和是多少。
解题思路：
可以发现，每个点的入度和出度都是1。 如果每个点都拆成入点和出点，对于点u，可以拆成u和u’， u是入点，u’是出点。若有边(u, v)，则u’ -> v连接， 这样整个图转化为一个二分图。由于每个入点需要找一个出点连接，每个出点也要找一个入点连接，那么就是经典的二分图匹配问题。加上一个权值，就是二分图最优匹配问题。用KM或者最小费用流都可以解决。
KM算法：
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#define INF 0x3f3f3f3f
using namespace std;
const int N=205;
int n,m,w[N][N],link[N],lx[N],ly[N],slack[N];
bool visx[N],visy[N];
bool dfs(int u)
{
    visx[u]=true;
    for(int i=0;i<n;i++)
    {
        if(visy[i])
          continue;
        int tmp=lx[u]+ly[i]-w[u][i];
        if(tmp)
          slack[i]=min(slack[i],tmp);
        else
        {
            visy[i]=true;
            if(link[i]==-1||dfs(link[i]))
            {
                link[i]=u;
                return true;
            }
        }
    } 
    return false;
}
int KM()
{
    memset(link,-1,sizeof(link));
    memset(ly,0,sizeof(ly));
    for(int i=0;i<n;i++)
    {
        lx[i]=-INF;
        for(int j=0;j<n;j++)
          lx[i]=max(lx[i],w[i][j]);
    }
    for(int i=0;i<n;i++)
    {
        memset(slack,INF,sizeof(slack));
        while(1)
        {
            memset(visx,false,sizeof(visx));
            memset(visy,false,sizeof(visy));
            if(dfs(i))
              break;
            int d=INF;
            for(int j=0;j<n;j++) 
              if(!visy[j])
                d=min(d,slack[j]);
            for(int j=0;j<n;j++)
            {
                if(visx[j])
                  lx[j]-=d;
                if(visy[j])
                  ly[j]+=d;
                else
                  slack[j]-=d;
            }
        }
    }
    int ans=0,cnt=0;
    for(int i=0;i<n;i++)
    {
        if(link[i]==-1||w[link[i]][i]==-INF)
          continue;
        ans+=w[link[i]][i];
        cnt++;
    } 
    if(cnt!=n)
      return -1;
    return -ans;
}
int main()
{
    while(~scanf("%d%d",&n,&m))
    {
        int u,v,c;
        for(int i=0;i<n;i++)
          for(int j=0;j<n;j++)
            w[i][j]=-INF;
        while(m--)
        {
            scanf("%d%d%d",&u,&v,&c);
            u--;v--;
            w[u][v]=max(w[u][v],-c);
        } 
        printf("%d\n",KM());
    }
    return 0;
}
```
