# codeforces 933-A.A Twisty Movement （dp） - HJ - CSDN博客
2018年04月20日 18:40:40[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：167
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/933/A](http://codeforces.com/problemset/problem/933/A)
题目大意：给出一个长度为n的数组，求经过一次任意区间的翻转之后，最大的非递减子序列。
解题思路：只要对这个数组正序求非递减子序列、逆序求非递增子序列，然后遍历一次即可。
```cpp
#include<iostream>
#include<vector>
using namespace std;
const int N=2005;
int a[N],dp[N],pre[N],nxt[N];   //pre[i]表示前i个数的最长非递减子序列，nxt[j]表示[j,n]区间内的最长非递增子序列 
int main()
{
    int n;
    while(cin>>n)
    {
        for(int i=1;i<=n;i++)
          cin>>a[i];
        for(int i=1;i<=n;i++)
        {
            dp[i]=1;
            for(int j=1;j<i;j++)
              if(a[j]<=a[i]&&dp[j]+1>dp[i])
                dp[i]=dp[j]+1;
            pre[i]=max(dp[i],pre[i-1]);
        }
        for(int i=n;i>=1;i--)
        {
            dp[i]=1;
            for(int j=n;j>i;j--)
              if(a[j]>=a[i]&&dp[j]+1>dp[i])
                dp[i]=dp[j]+1;
            nxt[i]=max(dp[i],nxt[i+1]);
        }
        int ans=0;
        for(int i=1;i<=n;i++)
          ans=max(ans,pre[i]+nxt[i+1]);
        cout<<ans<<endl;
    } 
    return 0;
}
```
