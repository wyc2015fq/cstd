# codeforces 855-B. Marvolo Gaunt's Ring（背包问题） - HJ - CSDN博客
2018年08月14日 08:14:09[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：123
个人分类：[====ACM====																[--------动态规划---------																[背包问题																[---------OJ---------																[codeforce](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/7238225)](https://blog.csdn.net/feizaoSYUACM/article/category/7238093)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/855/B](http://codeforces.com/problemset/problem/855/B)
解题思路：
可以把p,q,r看成三个物品，当做背包问题处理。
```
#include<iostream>
using namespace std;
#define INF 0x3f3f3f3f3f3f3f3f
typedef long long ll;
const int maxn=1e5+10;
ll a[maxn],b[4],dp[maxn][4];
int main()
{
    int n;
    while(cin>>n)
    {
        for(int i=1;i<=3;i++)
          cin>>b[i];
        for(int i=1;i<=n;i++)
          cin>>a[i];
        for(int i=0;i<=n;i++)
        {
            dp[i][0]=0;
            for(int j=1;j<=3;j++)
              dp[i][j]=-INF;
        }
        for(int i=1;i<=n;i++)
          for(int j=1;j<=3;j++)
            dp[i][j]=max(dp[i][j],max(dp[i-1][j],dp[i][j-1]+(ll)(a[i]*b[j])));
        cout<<dp[n][3]<<endl;
    }
    return 0;
}
```
