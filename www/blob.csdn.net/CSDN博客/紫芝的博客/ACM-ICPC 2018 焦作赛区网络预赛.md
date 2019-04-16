# ACM-ICPC 2018 焦作赛区网络预赛 - 紫芝的博客 - CSDN博客





2018年09月15日 23:34:14[紫芝](https://me.csdn.net/qq_40507857)阅读数：55








[Give Candies](https://nanti.jisuanke.com/t/31716)

题意：有n颗糖，有n个人，按顺序出列，每次随机给那个人一些糖（至少一颗），分完为止，求有多少方案

思路：规律是2^(n−1) 根据[**费马小定理**](https://blog.csdn.net/qq_40507857/article/details/82718179) a^(p−1)=1(mod p), 那么先n%p，然后使用**[快速幂](https://blog.csdn.net/qq_40507857/article/details/80246190)**；

```cpp
#include<bits/stdc++.h>
using namespace std;
typedef long long LL;
const int maxn=1e5+7,mod=1e9+7;
char str[maxn];
/*
LL quick_mul(LL a,LL b)
{
    LL ans=0;
    while(b){
        if(b&1) ans=(ans+a)%mod;
        a=(a+a)%mod;
        b>>=1;
    }
    return ans;
}
*/
LL quick_pow(LL a,LL b)
{
    LL ret=1;
    while(b){
        if(b&1)
        //ret=quick_mul(ret,a);
        ret=(ret*a)%mod;

        //a=quick_mul(a,a);
        a=(a*a)%mod;
        b>>=1;
    }
    return ret;
}
int main()
{
    int T;
    scanf("%d",&T);
    while(T--){
        scanf("%s",str);
        int len=strlen(str);
        LL num=0;
        for(int i=0;i<len;i++)
        {
            num=(num*10+str[i]-'0')%(mod-1);
        }

        printf("%lld\n",quick_pow(2,num-1));
    }
}
```





