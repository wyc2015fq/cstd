# POJ 3041 Asteroids（二分匹配） - HJ - CSDN博客
2017年10月14日 14:42:34[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：159
[http://poj.org/problem?id=3041](http://poj.org/problem?id=3041)
题目大意： 
给一个N*N的矩阵，有些格子有障碍，要求我们消除这些障碍，问每次消除一行或一列的障碍，最少要几次。
思路： 
将每行x看成一个X结点，每列Y看成一个Y结点，障碍的坐标x,y看成X到Y的一条边。构建出图后，就变成了找最少的点，使得这些点与所有的边相邻，即最小点覆盖问题。
最小点覆盖=最大匹配
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
const int N=505;
int n,link[N],map[N][N];
bool vis[N];
bool dfs(int u)
{
    for(int i=1;i<=n;i++)
    {
        if(!vis[i]&&map[u][i])
        {
            vis[i]=true;
            if(link[i]==-1||dfs(link[i]))
            {
                link[i]=u;
                return true;
            }
        } 
    }
    return false;
}
int hungary()
{
    int ans=0;
    memset(link,-1,sizeof(link));
    for(int i=1;i<=n;i++)
    {
        memset(vis,false,sizeof(vis));
        if(dfs(i))
          ans++;
    }
    return ans;
}
int main()
{
    int m,x,y;
    while(~scanf("%d%d",&n,&m))
    {
        memset(map,0,sizeof(map));
        while(m--)
        {
            scanf("%d%d",&x,&y);
            map[x][y]=1;
        }
        printf("%d\n",hungary());
    }
    return 0;
}
```
