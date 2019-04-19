# codeforces 1005-C. Summarize to the Power of Two（打表+暴力） - HJ - CSDN博客
2018年07月10日 20:45:07[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：150
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/1005/C](http://codeforces.com/problemset/problem/1005/C)
题目大意： 
      如果一个数组中，存在着这个数不管加上数组中其他任何数，其和都不是2的次幂，那么这个数就需要删除，问这个数组中需要删除多少个这样的数。
解题思路： 
    数据的范围是1e9，那么2的次幂打表到2e9即可，用set和map分别处理判断这个数与2的次幂之差是否能在数组中找到，如果都找不到，那么这个数就需要删除。
```cpp
#include<iostream>
#include<algorithm>
#include<cstring>
#include<map>
#include<set>
using namespace std;
typedef long long ll;
#define max 1000000000
map<ll,int> mp;
set<int> s;
ll a[120005],d[35];
int main()
{
    int n;
    d[0]=2;
    for(int i=1;i<=35;i++)
      d[i]=d[i-1]*2;
    while(cin>>n)
    {
        int cnt=0;
        for(int i=0;i<n;i++)
        {
            cin>>a[i];
            if(s.find(a[i])!=s.end())
              mp[a[i]]=2;
            else
            {
                s.insert(a[i]);
                mp[a[i]]=1;
            }
        }
        for(int i=0;i<n;i++)
        {
            int blag=0;
            for(int j=0;;j++)
            {
                if(d[j]<=a[i])
                  continue;
                ll p=d[j]-a[i];
                if(p==a[i])
                {
                    if(mp[p]==2)
                    {
                        blag=1;
                        break;
                    }
                }
                else
                {
                    if(mp[p]==1||mp[p]==2)
                    {
                        blag=1;
                        break;
                    }
                }
                if(p>max)
                  break;
            }
            if(blag==0)
              cnt++;
        }
        cout<<cnt<<endl;
        mp.clear();
        s.clear();
    }
    return 0;
}
```
