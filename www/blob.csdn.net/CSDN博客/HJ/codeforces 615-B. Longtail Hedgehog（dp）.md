# codeforces 615-B. Longtail Hedgehog（dp） - HJ - CSDN博客
2018年08月08日 10:53:50[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：56
个人分类：[====ACM====																[--------动态规划---------																[基础DP																[---------OJ---------																[codeforce](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/7238027)](https://blog.csdn.net/feizaoSYUACM/article/category/7238093)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/615/B](http://codeforces.com/problemset/problem/615/B)
题目大意：
给出一个n个点m条边的无向无环图（不一定全联通），求一条递增链长度与尾节点个数乘积的最大值。
解题思路：
设dp[i]表示以节点i结尾的最长的递增链长度，那么只要比较每个点的dp[i]值与对应点的节点个数乘积即可。
```cpp
#include<iostream>
#include<cstring>
#include<algorithm>
#include<vector>
using namespace std;
typedef long long ll;
const int maxn=2e5+10;
struct node{
    int x,y;
}s[maxn];
int n,m;
ll num[maxn],dp[maxn];
bool cmp(node a,node b)
{
    if(a.x==b.x)
      return a.y<b.y;
    return a.x<b.x;
}
int main()
{
    while(cin>>n>>m)
    {
        memset(dp,0,sizeof(dp));
        memset(num,0,sizeof(num));
        for(int i=0;i<m;i++)
        {
            cin>>s[i].x>>s[i].y;
            if(s[i].x>s[i].y)
              swap(s[i].x,s[i].y);
            num[s[i].x]++,num[s[i].y]++;
        }
        sort(s,s+m,cmp);
        for(int i=0;i<m;i++)
          dp[s[i].y]=max(dp[s[i].y],dp[s[i].x]+1);
        ll ans=0;
        for(int i=1;i<=n;i++)
          ans=max(ans,(dp[i]+1)*num[i]);
        cout<<ans<<endl;
    }
    return 0;
}
```
