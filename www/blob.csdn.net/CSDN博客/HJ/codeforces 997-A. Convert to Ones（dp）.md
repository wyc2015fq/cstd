# codeforces 997-A. Convert to Ones（dp） - HJ - CSDN博客
2018年08月08日 20:41:06[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：72
个人分类：[====ACM====																[--------动态规划---------																[基础DP																[---------OJ---------																[codeforce](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/7238027)](https://blog.csdn.net/feizaoSYUACM/article/category/7238093)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/contest/998/problem/C](http://codeforces.com/contest/998/problem/C)
解题思路：
查找连续‘0’出现的区间的个数（假设为cnt），枚举cnt，将初始化为正无穷的ans与前i个连续出现‘0’的区间采用反转，后cnt-i+1采用翻转方法所耗费的值进行比较，取较小值。
```cpp
#include<iostream>
#include<algorithm>
#include<queue>
#include<vector>
#include<cstdio>
#include<cstring>
#include<cmath>
using namespace std;
#define mod 1000000009
typedef long long ll;
string s;
ll x,y;
int main()
{
    int n;
    while(cin>>n>>x>>y)
    {
        cin>>s;
        int num=0;
        for(int i=0;i<s.length();i++)
          if(s[i]=='0')
            num++;
        if(s[s.length()-1]=='0')
          s+='1';
        if(num==n)
          cout<<y<<endl;
        else if(num==0)
          cout<<0<<endl;
        else
        {
             int i=0;
             ll cnt=0;
             for(;i<s.length();i++)
               if(s[i]=='0')
                 break;
             for(;i<s.length();i++)
               if(s[i]=='1'&&s[i-1]=='0')
                 cnt++;
            ll ans=1e18;
            for(int i=0;i<cnt;i++)
            {
                ll p=i*x,q=(cnt-i)*y;
                ans=min(ans,(ll)(p+q));
            }
            cout<<ans<<endl;
        }
    }
    return 0;
}
```
