# hdu6395——Sequence - westbrook1998的博客 - CSDN博客





2018年10月31日 09:16:40[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：30








递推式 要用矩阵快速幂加速，但是构造出来的矩阵里面有一个不确定的遍历 p/i

所以没办法从第一项直接用矩阵快速幂推到最后一项

不过可以看出p/i是可以分块的，就是从1递推到n中间的这些矩阵里，可以分成几个块这样子来递推，每一个块里面的构造矩阵就是一样了，直接用矩阵快速幂

注意p/(p/i)这个就可以快速找到这个块的最右边界

还要注意开long long
代码：

```
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll MOD=1e9+7;
struct Mat{
    ll m[3][3];
};
Mat operator *(Mat a,Mat b){
    Mat t;
    memset(t.m,0,sizeof(t.m));
    for(ll k=0;k<3;k++){
        for(ll i=0;i<3;i++){
            if(a.m[i][k]==0){
                continue;
            }
            for(ll j=0;j<3;j++){
                if(b.m[k][j]==0){
                    continue;
                }
                ll tt=((ll)a.m[i][k]*(ll)b.m[k][j])%MOD;
                t.m[i][j]+=tt%MOD;
            }
        }
    }
    return t;
}
Mat operator ^ (Mat a,ll n){
    Mat t;
    for(ll i=0;i<3;i++){
        for(ll j=0;j<3;j++){
            t.m[i][j]=i==j?1:0;
        }
    }
    while(n){
        if(n%2){
            t=t*a;
        }
        n/=2;
        a=a*a;
    }
    return t;
}
ll t,a,b,c,d,p,n;
int main(void){
    scanf("%lld",&t);
    while(t--){
        scanf("%lld%lld%lld%lld%lld%lld",&a,&b,&c,&d,&p,&n);
        if(n==1){
            printf("%lld\n",a);
            continue;
        }
        else if(n==2){
            printf("%lld\n",b);
            continue;
        }
        Mat f;
        memset(f.m,0,sizeof(f));
        f.m[0][0]=d;
        f.m[0][1]=c;
        f.m[1][0]=1;
        f.m[2][2]=1;
        bool flag=false;
        for(ll i=3;i<=n;){
            //p/i向下取整为0的的情况，后面的p/i也是为0，直接一次快速幂
            if(p/i==0){
                Mat w=f;
                w=w^(n-i+1);
                ll ans=w.m[0][0]*(ll)b%MOD+w.m[0][1]*(ll)a%MOD+w.m[0][2]%MOD;
                ans%=MOD;
                printf("%lld\n",ans);
                flag=true;
                break;
            }
            //i到j这一段的p/i都是一样的，分块
            ll j=min(n,p/(p/i));
            Mat w=f;
            w.m[0][2]=p/i;
            w=w^(j-i+1);
            ll tmp1=(w.m[1][0]*(ll)b+w.m[1][1]*(ll)a+w.m[1][2])%MOD;
            ll tmp2=(w.m[0][0]*(ll)b+w.m[0][1]*(ll)a+w.m[0][2])%MOD;
            //更新fn-1和fn-2
            a=tmp1;
            b=tmp2;
            //分块重点
            i=j+1;
        }
        if(!flag){
            printf("%lld\n",b);
        }
    }
    return 0;
}
```




