# hdu 2807 The Shortest Path（矩阵计算+Floyd） - HJ - CSDN博客
2017年10月11日 19:25:35[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：176
个人分类：[====ACM====																[---------OJ---------																[HDU																[----------图论---------																[最短路](https://blog.csdn.net/feizaoSYUACM/article/category/6307997)](https://blog.csdn.net/feizaoSYUACM/article/category/6308029)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.split.hdu.edu.cn/showproblem.php?pid=2224](http://acm.split.hdu.edu.cn/showproblem.php?pid=2224)
题目大意： 
城市一共有n个地点，每个地点是一个m*m的矩阵，当a*b=c时，说明存在一条从a指向c的边
解题思路： 
矩阵计算，并且判断a与c是否连接，然后求出最短路即可。
```cpp
#include<iostream>
#include<cstring>
#include<cstdio>
#define INF 0x3f3f3f3f 
using namespace std;
const int maxn=85;
int n,m,a[maxn][maxn][maxn],dp[maxn][maxn],ans[maxn][maxn];
void Floyd()
{
    for(int k=1;k<=n;k++)
      for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)
          dp[i][j]=min(dp[i][j],dp[i][k]+dp[k][j]);
}
void check(int x,int i,int j)
{
    for(int y=1;y<=m;y++)
      for(int z=1;z<=m;z++)
        if(ans[y][z]!=a[x][y][z])
          return ;
    dp[i][x]=1;
}
void get_dp()
{
    for(int i=0;i<=n;i++)          //初始化 
      for(int j=0;j<=n;j++)
        dp[i][j]=(i==j)?0:INF;
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=n;j++)
        {
            if(i==j)
              continue;
            memset(ans,0,sizeof(ans));
            for(int x=1;x<=m;x++)              //矩阵相乘 
              for(int y=1;y<=m;y++)
              {
                  ans[x][y]=0;
                  for(int z=1;z<=m;z++)
                    ans[x][y]+=a[i][x][z]*a[j][z][y];                 
              }       
            for(int x=1;x<=n;x++)   //检查是否有 a*b=c 即a是否可以到达c
            {
                if(x==i||x==j)
                  continue;
                check(x,i,j);
            }
        }
    }
} 
int main()
{
    while(cin>>n>>m&&(n||m))
    {
        for(int i=1;i<=n;i++)
          for(int j=1;j<=m;j++)
            for(int k=1;k<=m;k++)
              cin>>a[i][j][k];
        get_dp();
        Floyd();
        int t,x,y;
        cin>>t;
        while(t--)
        {
            cin>>x>>y;
            if(dp[x][y]==INF)
              cout<<"Sorry"<<endl;
            else
              cout<<dp[x][y]<<endl;
        }
    }
    return 0;
}
```
