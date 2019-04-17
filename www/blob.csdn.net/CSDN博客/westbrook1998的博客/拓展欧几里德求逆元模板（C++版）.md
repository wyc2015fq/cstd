# 拓展欧几里德求逆元模板（C++版） - westbrook1998的博客 - CSDN博客





2018年11月06日 22:58:18[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：307








逆元其实就相当于倒数的一个推广（我的理解），比如A/B 就相当于A乘B的逆元，也就是倒数，在模n情况下就不只是简单的倒数这种情况了

用拓展欧几里德来求逆元，具体证明还不太懂，就。。先背吧

hdu1576

题意是给A%MOD 和MOD 和B 求（A/B）%MOD ，那已知A%MOD 其实就可以相当于已知A，但是肯定不能直接除以B，所以相当于乘以B的逆元，所以问题就转化为求B的逆元

```
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
ll exgcd(ll a,ll b,ll &x,ll &y){
    if(!b){
        x=1;
        y=0;
        return a;
    }
    ll d=exgcd(b,a%b,x,y);
    ll tmp=x;
    x=y;
    y=tmp-a/b*y;
    return d;
}
ll inv(ll a,ll m){
    ll x,y;
    ll d=exgcd(a,m,x,y);
    if(d==1){
        //处理负数
        return (x%m+m)%m;
    }
    return -1;
}
ll n,b;
int t;
const ll MOD=9973;
int main(void){
    scanf("%d",&t);
    while(t--){
        scanf("%lld%lld",&n,&b);
        ll c=inv(b,MOD);
        printf("%lld\n",n*c%MOD);
    }
    return 0;
}
```



