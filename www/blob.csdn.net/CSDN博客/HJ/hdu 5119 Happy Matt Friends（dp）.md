# hdu  5119 Happy Matt Friends（dp） - HJ - CSDN博客
2018年08月12日 19:56:25[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：60
个人分类：[====ACM====																[--------动态规划---------																[基础DP																[---------OJ---------																[HDU](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/7238027)](https://blog.csdn.net/feizaoSYUACM/article/category/7238093)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.hdu.edu.cn/showproblem.php?pid=5119](http://acm.hdu.edu.cn/showproblem.php?pid=5119)
解题思路：
dp递推求解，设dp[i][j]表示前i个数异或值为j的种类数，则状态转移方程为：
```
dp[i][j] = dp[i-1][j] + dp[i-1][j^a[i]]
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
const int maxn=(1<<20);
int a[50];
ll dp[50][maxn];
int main()
{
    int t,n,m,tt=0;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d%d",&n,&m);
        for(int i=1;i<=n;i++)
          scanf("%d",&a[i]);
        memset(dp,0,sizeof(dp));
        dp[0][0]=1;
        for(int i=1;i<=n;i++)
          for(int j=0;j<maxn;j++)
            dp[i][j]=dp[i-1][j]+dp[i-1][j^a[i]];
        ll ans=0;
        for(int i=m;i<maxn;i++)
          ans+=dp[n][i];
        printf("Case #%d: %lld\n",++tt,ans);
    }
    return 0;
}
```
