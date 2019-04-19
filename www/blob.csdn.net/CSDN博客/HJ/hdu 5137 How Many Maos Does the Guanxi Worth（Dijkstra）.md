# hdu 5137 How Many Maos Does the Guanxi Worth（Dijkstra） - HJ - CSDN博客
2017年10月27日 19:51:52[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：177
个人分类：[====ACM====																[----------图论---------																[最短路																[---------OJ---------																[HDU](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6307997)](https://blog.csdn.net/feizaoSYUACM/article/category/6308029)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.hdu.edu.cn/showproblem.php?pid=5137](http://acm.hdu.edu.cn/showproblem.php?pid=5137)
题目大意： 
给出一个无向无环图，现在你有一个机会炸掉n个点其中的任意一个，只要炸掉这个点，那么这个点与其他但凡有连接的路径都会删除，问炸除一个点之后从 1–>n如果还有路径，那个最短路径是多少，如果没有路径，输出inf。
解题思路： 
枚举每一个点，进行删点操作，之后进行求最短路，找出那个最短路最大值的那个点，删除之后即可。最后输出更新值。
```cpp
#include<iostream>
#include<cstring>
#define INF 0x3f3f3f3f 
using namespace std;
const int maxn=35;
int n,g[maxn][maxn],s[maxn],d[maxn],vis[maxn];
void Dijkstra()
{
    for(int i=1;i<=n;i++)
      d[i]=g[1][i];
    memset(vis,0,sizeof(vis));
    vis[1]=1;
    for(int i=1;i<=n;i++)
    {
        int mind=INF,mark;
        for(int j=1;j<=n;j++)
          if(!vis[j]&&mind>d[j])
            mind=d[mark=j];
        vis[mark]=1;
        for(int j=1;j<=n;j++)
          if(!vis[j])
            d[j]=min(d[j],d[mark]+g[mark][j]);
    }
}
int main()
{
    int m;
    while(cin>>n>>m&&(n||m))
    {
        for(int i=1;i<=n;i++)
          for(int j=1;j<=n;j++)
            g[i][j]=(i==j)?0:INF;
        int ans=0,u,v,w,k;
        while(m--)
        {
            cin>>u>>v>>w;
            if(g[u][v]>w)
              g[u][v]=w;
        }
        for(int i=2;i<n;i++)
        {
            for(int j=1;j<=n;j++)     
            {
                s[j]=g[i][j];     //先用一个数组存储好i->j的路径 
                g[i][j]=INF;      //然后拆掉从i->j的路径 
            }
            Dijkstra();        //计算当前到n的最短路 
            ans=max(ans,d[n]);         //不断更新那个拆了之后到n最短路值最大的那个 
            for(int j=1;j<=n;j++)    //之后还原路径 
              g[i][j]=s[j];
        }
        if(ans==INF)
          cout<<"Inf"<<endl;
        else
          cout<<ans<<endl;
    }
    return 0;
}
```
