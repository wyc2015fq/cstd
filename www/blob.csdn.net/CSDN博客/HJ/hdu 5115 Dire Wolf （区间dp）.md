# hdu  5115 Dire Wolf （区间dp） - HJ - CSDN博客
2018年08月12日 19:54:06[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：57
个人分类：[====ACM====																[--------动态规划---------																[区间DP																[---------OJ---------																[HDU](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/7238029)](https://blog.csdn.net/feizaoSYUACM/article/category/7238093)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.hdu.edu.cn/showproblem.php?pid=5115](http://acm.hdu.edu.cn/showproblem.php?pid=5115)
题目大意： 
有n匹狼，每次击杀其中的一匹需要耗费对方的a[i]值以及相邻两匹狼的b[i]值，问要如何击杀，才能让最后耗费最少？
解题思路：
区间dp。刚开始想的是贪心，但是总是过不了。
```
dp[i][j]表示击杀区间[i,j]中的狼最少耗费
```
```cpp
#include<iostream>
#include<algorithm>
#include<cstring>
#include<vector>
#include<map>
#include<set>
#include<cstdio>
#include<cmath>
using namespace std;
typedef long long ll;
#define mod 1000000007
#define INF 0x3f3f3f3f
const int maxn=200+10;
int a[maxn],b[maxn],dp[maxn][maxn];
int main()
{
    int t,n,tt=0;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d",&n);
        ll sum=0;
        for(int i=1;i<=n;i++)
          scanf("%lld",&a[i]),sum+=a[i];
        for(int i=1;i<=n;i++)
          scanf("%lld",&b[i]);
        b[0]=b[n+1]=0; 
        for(int len=1;len<=n;len++)      
          for(int i=1;i+len-1<=n;i++)            
          {
              int j=i+len-1;
              dp[i][j]=INF;        
              for(int k=i;k<=j;k++)            
                dp[i][j]=min(dp[i][j],dp[i][k-1]+dp[k+1][j]+a[k]+b[i-1]+b[j+1]);
          }
        printf("Case #%d: %lld\n",++tt,dp[1][n]);
    }
    return 0;
}
```
