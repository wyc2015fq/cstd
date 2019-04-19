# codeforces 987-C. Three displays（思维） - HJ - CSDN博客
2018年08月17日 09:06:14[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：81
个人分类：[====ACM====																[---------OJ---------																[codeforce](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/987/C](http://codeforces.com/problemset/problem/987/C)
解题思路：
枚举中间的节点 j，然后往左边扫一遍i，再往右边扫一遍k即可
```cpp
#include<iostream>
#include<algorithm>
#include<cstring>
#include<vector>
#include<set>
#include<map>
#include<cstdio>
using namespace std;
#define mod 1000000007
#define INF 0x3f3f3f3f3f3f3f3f
typedef long long ll;
const int maxn=1e5+10;
ll x,y,z,a[maxn],s[maxn];
int main()
{
    int n;
    while(cin>>n)
    {
        for(int i=1;i<=n;i++)
          cin>>s[i];
        for(int i=1;i<=n;i++)
          cin>>a[i];
        ll ans=INF;
        for(int i=2;i<n;i++)
        {
            ll res1=INF,res2=INF;
            for(int j=i-1;j>0;j--)
              if(s[j]<s[i])
                res1=min(res1,a[j]);
            for(int j=i+1;j<=n;j++)
              if(s[j]>s[i])
                res2=min(res2,a[j]);
            ans=min(ans,(ll)(res1+res2+a[i]));
        }
        if(ans==INF)
          cout<<-1<<endl;
        else
          cout<<ans<<endl;
    }
    return 0;
}
```
