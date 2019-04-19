# hdu 2112 HDU Today（SPFA+map） - HJ - CSDN博客
2017年10月10日 14:42:59[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：134
个人分类：[====ACM====																[---------OJ---------																[HDU																[----------图论---------																[最短路](https://blog.csdn.net/feizaoSYUACM/article/category/6307997)](https://blog.csdn.net/feizaoSYUACM/article/category/6308029)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.split.hdu.edu.cn/showproblem.php?pid=2112](http://acm.split.hdu.edu.cn/showproblem.php?pid=2112)
求最短路，SPFA+map存储。
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<queue> 
#include<map>
#define INF 0x3f3f3f3f
using namespace std;
const int maxn=150;
int cnt,n,time,g[maxn][maxn],d[maxn];
bool vis[maxn];
void Init()
{
    for(int i=1;i<=cnt;i++)
    {
        d[i]=INF;
        vis[i]=false;
    }
}
int SPFA(int s,int t)
{
    if(s==t)
      return 0;
    Init();
    queue<int>q;
    d[s]=0;
    vis[s]=true;
    q.push(s);
    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        vis[u]=false;
        for(int i=1;i<=cnt;i++)
        {
            if(d[i]>d[u]+g[u][i])
            {
                d[i]=d[u]+g[u][i];
                if(!vis[i])
                {
                    vis[i]=true;
                    q.push(i);
                }
            }
        }
    }
    if(d[t]>=INF)
      return -1;
    return d[t];
}
int main()
{
    while(~scanf("%d",&n)&&n!=-1)
    {
        for(int i=1;i<maxn;i++)
          for(int j=1;j<maxn;j++)
            g[i][j]=INF;
        map<string,int> m;
        m.clear();
        char s[150],t[150];
        scanf("%s%s",&s,&t);
        cnt=0;
        m[s]=++cnt;
        if(m[t]==0)
          m[t]=++cnt;   
        for(int i=0;i<n;i++)
        {
            char s1[150],s2[150];
            scanf("%s%s%d",&s1,&s2,&time);
            if(m[s1]==0)
              m[s1]=++cnt;
            if(m[s2]==0)
              m[s2]=++cnt;
            if(g[m[s1]][m[s2]]>time)
              g[m[s1]][m[s2]]=g[m[s2]][m[s1]]=time;
        }   
        printf("%d\n",SPFA(m[s],m[t]));
    }
    return 0;
}
```
