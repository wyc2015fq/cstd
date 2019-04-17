# codeforces1025B_Weakened Common Divisor - westbrook1998的博客 - CSDN博客





2018年12月13日 09:48:22[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：35








数论的题啊…

给n对数,找出一个数是满足是n对数里至少一个数的因子,称为wcd

一般这种题就是gcd lcm 素因子什么的

所以我们用第一对数的两个数不断去和接下来的n-1对数的乘积取gcd,因为两个数的乘积就包含了这两个数的因子了,这样求出来最后再取个最大的,判断是否大于1,如果是,找出一个因子即可
代码:

```
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=2e5+50;
int n;
ll aa,bb;
ll a[N],b[N];
int main(void){
    scanf("%d",&n);
    scanf("%lld%lld",&aa,&bb);
    for(int i=1;i<n;i++){
        scanf("%lld%lld",&a[i],&b[i]);
        aa=__gcd(aa,a[i]*b[i]);
        bb=__gcd(bb,a[i]*b[i]);
    }
    ll d=max(aa,bb);
    if(d>1){
        //输出因子
        for(ll i=2;i*i<=d;i++){
            if(d%i==0){
                printf("%lld\n",i);
                return 0;
            }
        }
        printf("%lld\n",d);
    }else{
        printf("-1\n");
    }
    return 0;
}
```




