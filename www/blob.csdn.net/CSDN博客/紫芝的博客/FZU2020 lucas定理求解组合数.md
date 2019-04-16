# FZU2020 lucas定理求解组合数 - 紫芝的博客 - CSDN博客





2018年09月13日 16:10:41[紫芝](https://me.csdn.net/qq_40507857)阅读数：1424








# [组合](https://cn.vjudge.net/problem/FZU-2020)

给出组合数C(n,m), 表示从n个元素中选出m个元素的方案数。例如C(5,2) = 10, C(4,2) = 6.可是当n,m比较大的时候，C(n,m)很大！于是xiaobo希望你输出 C(n,m) mod p的值！

Input

输入数据第一行是一个正整数T,表示数据组数 (T <= 100) 接下来是T组数据，每组数据有3个正整数 n, m, p (1 <= m <= n <= 10^9, m <= 10^4, m < p < 10^9, p是素数)

Output

对于每组数据，输出一个正整数，表示C(n,m) mod p的结果。

Sample Input

```
2
5 2 3
5 2 61
```

Sample Output

```
1
10
```

```cpp
//卢卡斯定理求组合数
#include<cstdio>
#include<cstring>
using namespace std;
typedef long long ll;
//快速幂
ll fastPow(ll a,ll b,ll p)
{
    ll ret=1;
    while(b){
        if(b&1)
        ret=(ret*a)%p;
        a=(a*a)%p;
    b>>=1;
    }
    return ret;
}
//费马小定理求逆元fastPow(a,b-2,b)
ll c(ll n,ll m,ll p)
{
    if(m==0)    return 1;
    //if(m>n-m) m=n-m;
    ll up=1,down=1;
    for(int i=1;i<=m;i++)
    {
        up=(up*(n-i+1))%p;
        down=(down*i)%p;
    }
    return up*fastPow(down,p-2,p)%p;
}
//求C(n,m)%p
ll lucas(ll n,ll m,ll p)
{
    if(m==0)    return 1;
    return c(n%p,m%p,p)*lucas(n/p,m/p,p)%p;
}
int main()
{
    int T;
    ll n,m,p;
    scanf("%d",&T);
    while(T--){
        scanf("%I64d%I64d%I64d",&n,&m,&p);
        //scanf("%lld%lld%lld",&n,&m,&p);//错误
        //getFact(p);
        printf("%I64d\n",lucas(n,m,p));
    }
    return 0;
}
```





