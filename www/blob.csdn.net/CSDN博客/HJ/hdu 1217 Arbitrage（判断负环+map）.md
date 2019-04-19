# hdu  1217 Arbitrage（判断负环+map） - HJ - CSDN博客
2017年10月10日 14:41:06[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：164
个人分类：[====ACM====																[---------OJ---------																[HDU																[----------图论---------																[最短路](https://blog.csdn.net/feizaoSYUACM/article/category/6307997)](https://blog.csdn.net/feizaoSYUACM/article/category/6308029)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.split.hdu.edu.cn/showproblem.php?pid=1217](http://acm.split.hdu.edu.cn/showproblem.php?pid=1217)
判断是否出现负环？用map存储，floyd或者SPFA都可。
SPFA版
```cpp
#include<iostream>
#include<queue>
#include<cstring> 
#include<cstdio>
#include<map>
using namespace std;
const int maxn=40;
map<string,int>m;
double money,d[maxn],g[maxn][maxn];
int t,tt=0,n,blag,vis[maxn];
string s,a,b;
int SPFA(int src)
{
    queue<int>q;
    for(int i=0;i<n;i++)
      vis[i]=d[i]=0;
    d[src]=1.0;
    vis[src]=1;
    q.push(src);
    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        vis[u]=0;
        for(int i=0;i<n;i++)
        {
            if(d[i]<d[u]*g[u][i])
            {
                d[i]=d[u]*g[u][i];
                if(d[src]>1.0)
                  return 1;
                if(!vis[i])
                {
                    vis[i]=1;
                    q.push(i);
                }
            } 
        }
    }
    return 0;
}
int main()
{
    while(cin>>n&&n)
    {
        m.clear();
        for(int i=0;i<n;i++)
          for(int j=0;j<n;j++)
            g[i][j]=(i==j)?1.0:0; 
        for(int i=0;i<n;i++)
        {
            cin>>s;
            m[s]=i;
        }
        cin>>t;
        memset(d,0,sizeof(d));
        while(t--)
        {
            cin>>a>>money>>b;
            g[m[a]][m[b]]=money;
        }
        blag=0;
        for(int i=0;i<n;i++)
          if(SPFA(i))
          {
              blag=1;
              break;
          }
        printf("Case %d: %s\n",++tt,blag?"Yes":"No");
    }
}
```
Floyd版
```cpp
#include<iostream>
#include<cstring> 
#include<cstdio>
#include<map>
using namespace std;
const int maxn=40;
map<string,int>m;
double money,d[maxn][maxn];
int t,tt=0,n,blag;
string s,a,b;
void floyd()
{
    for(int k=0;k<n;k++)
      for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
          d[i][j]=max(d[i][j],d[i][k]*d[k][j]);
    for(int i=0;i<n;i++)
      if(d[i][i]>1.0)
      {
          blag=1;
          return ;
      }
}
int main()
{
    while(cin>>n&&n)
    {
        for(int i=0;i<n;i++)
        {
            cin>>s;
            m[s]=i;
        }
        cin>>t;
        memset(d,0,sizeof(d));
        while(t--)
        {
            cin>>a>>money>>b;
            d[m[a]][m[b]]=money;
        }
        blag=0;
        floyd();
        printf("Case %d: %s\n",++tt,blag?"Yes":"No");
    }
}
```
