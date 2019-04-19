# codeforces 279-B. Books（尺取） - HJ - CSDN博客
2017年08月31日 13:28:26[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：252
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/279/B](http://codeforces.com/problemset/problem/279/B)
简单尺取，当k>=0的时候更新ans值。
```cpp
#include<iostream>
#include<algorithm>
using namespace std;
#define maxn 100005
#define INF 0x3f3f3f3f
long long s[maxn];
int main()
{
    long long n,t;
    while(cin>>n>>t)
    {
        int cnt=0;
        for(int i=0;i<n;i++)
          cin>>s[i];
        s[n]=INF;
        int l=0,r=0,ans=0;
        long long k=t;
        while(l<n&&r<n)
        {
            while(r<n&&k-s[r]>=0)
            {
                k-=s[r];
                r++;
            }
            if(k>=0)
              ans=max(ans,r-l);
            k+=s[l];
            l++;
        }
        cout<<ans<<endl;
    }
    return 0;
}
```
