# hdu 2923 Einbahnstrasse（Floyd+map） - HJ - CSDN博客
2017年10月10日 21:37:11[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：126
个人分类：[====ACM====																[---------OJ---------																[HDU																[----------图论---------																[最短路](https://blog.csdn.net/feizaoSYUACM/article/category/6307997)](https://blog.csdn.net/feizaoSYUACM/article/category/6308029)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.split.hdu.edu.cn/showproblem.php?pid=2923](http://acm.split.hdu.edu.cn/showproblem.php?pid=2923)
题目大意： 
给出城市数，破车数和道路数，然后跟着m条路线，箭头为指向，数字为花费。求出从公司派出车，到每个城市回收破车的最小花费，要注意每辆车只能回收一辆破车。
用map记录，注意有重边，然后Floyd求出 d[u][v] 的花费。
```cpp
#include<iostream>
#include<cstdio>
#include<map>
#define INF 0x3f3f3f3f
using namespace std;
const int maxn=105;
int tt=0,sum,n,c,m,cnt,d[maxn][maxn];
char str[1005][maxn];
map<string,int>mp;
void Floyd()
{
    for(int k=1;k<=n;k++)
      for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)
          d[i][j]=min(d[i][j],d[i][k]+d[k][j]);
}
int main()
{
    while(~scanf("%d%d%d",&n,&c,&m)&&(n||c||m))
    {
        sum=0;
        mp.clear();
        for(int i=0;i<=n;i++)
          for(int j=0;j<=n;j++)
            d[i][j]=INF;
        for(int i=0;i<=c;i++)
          scanf("%s",str[i]);
        cnt=0;
        for(int i=0;i<m;i++)
        {
            int w;
            char from,to,s1[15],s2[15];
            scanf("%s %c-%d-%c %s",s1,&from,&w,&to,s2);
            if(!mp[s1])
              mp[s1]=++cnt;
            if(!mp[s2])
              mp[s2]=++cnt;
            int u=mp[s1],v=mp[s2];
            if(from=='<'&&d[v][u]>w)
              d[v][u]=w;
            if(to=='>'&&d[u][v]>w)
              d[u][v]=w;
        }
        Floyd();
        int src=mp[str[0]];
        for(int i=1;i<=c;i++)
          sum+=d[src][mp[str[i]]]+d[mp[str[i]]][src];
        printf("%d. %d\n",++tt,sum);
    }
}
```
