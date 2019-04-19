# hdu 4800 Josephina and RPG （概率dp） - HJ - CSDN博客
2017年10月30日 21:09:01[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：176
个人分类：[====ACM====																[--------动态规划---------																[概率DP																[---------OJ---------																[HDU](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/7238030)](https://blog.csdn.net/feizaoSYUACM/article/category/7238093)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.hdu.edu.cn/showproblem.php?pid=4800](http://acm.hdu.edu.cn/showproblem.php?pid=4800)
题意： 
给定C(m，3)支队伍之间对战获胜的概率，再给定一个序列存放队伍的编号，每次获胜之后可以选择和当前战胜的对手换队伍或者不换。求挑战胜利的最大概率。
解题思路： 
想了好久，先想着用dfs，但最后不管怎么弄都超时。然后想到记忆化搜索，于是就想到了要往dp方面靠靠。设dp[i][j]表示战胜第i支队伍时，当前的队伍为j。而每次只有两种状态：换队伍和不换队伍。
```cpp
#include<iostream>
#include<cstdio>
#include <cstring>
using namespace std;
const int maxn=1500;
int n,m,s[10005];
double p[maxn][maxn],dp[maxn][maxn],ans;   //dp[i][j] 表示战胜前i支队伍 当前用的第j号队伍
int main()
{
    while(~scanf("%d",&m))
    {
        memset(dp, 0, sizeof dp);
        m=m*(m-1)*(m-2)/6;
        for(int i=0;i<m;i++)
          for(int j=0;j<m;j++)
            scanf("%lf",&p[i][j]);
        scanf("%d",&n);
        for(int i=1;i<=n;i++)
          scanf("%d",&s[i]);
        ans=0;
        for(int i=0;i<m;i++)
          dp[0][i]=1;
        for(int i=1;i<=n;i++)
          for(int j=0;j<m;j++)
          {
              dp[i][j]=max(dp[i][j],dp[i-1][j]*p[j][s[i]]);     //换队伍 
              dp[i][s[i]]=max(dp[i][s[i]],dp[i-1][j]*p[j][s[i]]);       //不换队伍 
          }
        for(int i=0;i<m;i++)
          ans=max(ans,dp[n][i]);
        printf("%.6lf\n", ans);
    }
    return 0;
}
```
