# HDU-4000 Fruit Ninja 树状数组 - 安得广厦千万间的博客 - CSDN博客





2018年07月24日 17:12:41[独-](https://me.csdn.net/qq_41713256)阅读数：41








链接：[http://acm.hdu.edu.cn/showproblem.php?pid=4000](http://acm.hdu.edu.cn/showproblem.php?pid=4000).

题意：给你一个长度为n的全排列，求有多少组（x，y，z）满足，x<y<z，且a【x】<a【z】<a【y】。

先求出满足，a【x】<a【z】，a【x】<a【y】有多少组，

对于当前点，后面有k个数比他大，那么有 k*（k-1）/2组。

我们在减去  a【x】<a【y】<a【z】的情况。

对于当前点，前面有k个数比他小，后面有u个数比他大，那么就有 k*u组。

利用树状数组来维护。

```
#include<bits/stdc++.h>
using namespace std;
typedef long long LL;
const LL mod=1e8+7;
const int maxn=1e5+5;
int c[maxn];
int lowbit(int x){ return x&(-x); }
void add(int x)
{
    while ( x<maxn )
        c[x]++, x+=lowbit(x);
}
int sum(int x)
{
    int res=0;
    while ( x)
      res+=c[x], x-=lowbit(x);
    return res;
}
int main()
{
    int T; scanf( "%d", &T );
    int cc=1;
    while ( T-- )
    {
        int n; scanf( "%d", &n );
        fill(c,c+maxn,0);
        LL ans=0;
        for(int i=1;i<=n;i++)
        {
            int x; scanf( "%d", &x );
            add(x);
            LL tmp=sum(x-1);
            LL tmp1=(n-x)-(i-1-tmp);
            ans=(ans-tmp*tmp1+mod)%mod;
            if( tmp1>=2 )
                ans=(ans+tmp1*(tmp1-1)/2)%mod;
        }
        printf( "Case #%d: %lld\n",cc++, ans );
    }
    return 0;
}
```





