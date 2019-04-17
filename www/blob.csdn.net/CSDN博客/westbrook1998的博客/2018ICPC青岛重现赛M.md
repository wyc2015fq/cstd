# 2018ICPC青岛重现赛M - westbrook1998的博客 - CSDN博客





2018年11月11日 13:16:58[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：66








签到题

定义f(x)就是x这个数（0-9）含有的圈的个数，然后再定义函数g(x)，0次的时候就是x，然后每加一次幂就是f(x)一次

1e9想想应该可以直接暴力循环，第一次tle，然后仔细一想到最后其实很多都是0-1-0-1这样变化的，真正有效的变化只有前面几个，所以循环到g(x)值为0就可以判断然后break了
代码：

```
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
int t;
ll x,k;
int f[10]={1,0,0,0,1,0,1,0,2,1};
int main(void){
    scanf("%d",&t);
    while(t--){
        scanf("%lld%lld",&x,&k);
        ll ans=x;
        for(int i=1;i<=k;i++){
            ll t=0;
            if(ans==0){
                t+=f[0];
            }else{
                while(ans){
                    t+=f[ans%10];
                    ans/=10;
                }
            }
            if(t==0){
                if((k-i)%2){
                    ans=1;
                }else{
                    ans=0;
                }
                break;
            }
            ans=t;
        }
        printf("%lld\n",ans);
    }
    return 0;
}
```




