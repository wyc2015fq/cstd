# codeforces 940-E. Cashback（dp） - HJ - CSDN博客
2018年04月19日 00:42:18[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：117
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/940/E](http://codeforces.com/problemset/problem/940/E)
题目大意：给出大小为n的一个数组，可以分成任意段，每一段的长度为k（k可以为不同值），每次在区分的段内减去最小的(k+1)/c个数，问最后划分的数组值的和最小为多少？
解题思路：用贪心的思想，要求所有和的最小值，每次划分的段长一定不能大于c，那么就一定是若干个长度为1的段和若干个长度为c的段，设d[i]表示前i个数的划分之后的和的最小值，那么状态转移方程为
```
d[i] = min(d[i-1]+a[i],d[i-c]+sum[i]-sum[i-c]-min(a[i-c],...,a[i]))
```
而每次要求a[i]到a[i-c]之间的最小值，可以用多重集合，也可以用单调队列，代码如下：
多重集合 multiset
```cpp
#include<iostream>
#include<set>
using namespace std;
typedef long long ll;
const int N=1e5+10;
int a[N];
ll sum[N],dp[N]; 
multiset<int> s;
int main()
{
    int n,c;
    while(cin>>n>>c)
    {
        for(int i=1;i<=n;i++)
        {
            cin>>a[i];
            dp[i]=sum[i]=sum[i-1]+a[i];
        }
        for(int i=1;i<c;i++)
          s.insert(a[i]);
        for(int i=c;i<=n;i++)
        {
            s.insert(a[i]);
            dp[i]=min(dp[i-1]+a[i],dp[i-c]+sum[i]-sum[i-c]-*s.begin());
            s.erase(s.find(a[i-c+1]));
        }
        s.clear();
        cout<<dp[n]<<endl;
    }
    return 0;
}
```
单调队列 
```
#include<iostream>
#include<set>
using namespace std;
typedef long long ll;
const int N=1e5+10;
ll a[N],sum[N],dp[N],q[N]; 
int main()
{
    int n,c;
    while(cin>>n>>c)
    {
        for(int i=1;i<=n;i++)
        {
            cin>>a[i];
            dp[i]=sum[i]=sum[i-1]+a[i];
        }
        int head=1,tail=0;
        for(int i=1;i<=n;i++)
        {
            while(head<=tail&&q[head]<i-c+1) 
              head++;
            while(head<=tail&&a[q[tail]]>a[i])
              tail--;
            q[++tail]=i;
            if(i>=c)
              dp[i]=min(dp[i-1]+a[i],dp[i-c]+sum[i]-sum[i-c]-a[q[head]]);
        }
        cout<<dp[n]<<endl;
    }
    return 0;
}
```
