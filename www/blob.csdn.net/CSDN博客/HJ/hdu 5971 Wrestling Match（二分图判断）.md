# hdu 5971 Wrestling Match（二分图判断） - HJ - CSDN博客
2017年10月27日 14:04:06[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：174
[http://acm.hdu.edu.cn/showproblem.php?pid=5971](http://acm.hdu.edu.cn/showproblem.php?pid=5971)
题目大意： 
有n个人，一些人会进行比赛，每次比赛的两个人都是分别属于X和Y不同阵营的人，另外会告诉你哪些人属于X阵营，哪些人属于Y阵营，问你最后能不能把所有的n个人都归好类别？
解题思路： 
二分图判断，每次比赛，就看做X和Y的一根连线。
```cpp
#include<iostream>
#include<vector>
#include<cstdio>
#include<cstring>
using namespace std;
const int maxn=1005;
vector<int> g[maxn];
int n,m,color[maxn];
bool dfs(int u,int col)
{
    if(g[u].size()==0&&color[u]==-1)
      return false;
    color[u]=col;
    for(int i=0;i<g[u].size();i++)
    {
        int v=g[u][i];
        if(color[v]==col||(color[v]==-1&&!dfs(v,!col)))
          return false;
    } 
    return true;
}
bool Judge()
{
    for(int i=1;i<=n;i++)
    {
        if(color[i]==0&&!dfs(i,0))
            return false;
        if(color[i]==1&&!dfs(i,1))
            return false;
    }
    for(int i=1;i<=n;i++)
      if(color[i]==-1&&!dfs(i,0))
        return false;
    return true;
}
int main()
{
    int x,y,a,u,v;
    while(cin>>n>>m>>x>>y)
    {
        for(int i=1;i<=n;i++)
          g[i].clear();
        memset(color,-1,sizeof(color));
        while(m--)
        {
            cin>>u>>v;
            g[u].push_back(v);
            g[v].push_back(u);
        }
        while(x--)
        {
            cin>>a;
            color[a]=0;
        }
        while(y--)
        {
            cin>>a;
            color[a]=1;
        } 
        printf("%s\n",Judge()?"YES":"NO");
    }
    return 0;
}
```
