# codeforces1056B_Divide Candies - westbrook1998的博客 - CSDN博客





2018年12月11日 18:40:04[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：51








题意是给一个n和m,求从(1,1)到(n,n)有多少对数满足(i*i+j*j)%m==0

暴力是不可能暴力的,这辈子都不可能暴力,n很大而m比较小,所以可以考虑预处理出1到n这些数模m对应值有多少个,然后答案就直接枚举这些模数,把个数乘起来就好,题解大概就是这样说的

代码:

```
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1050;
int n,m;
int cnt[N];
ll ans;
int main(void){
    scanf("%d%d",&n,&m);
    //处理出1到n中 %m余数为i的数的个数
    //处理前面km个数
    for(int i=0;i<m;i++){
        cnt[i]=n/m;
    }
    //处理后面n-km个数
    for(int i=(n/m)*m+1;i<=n;i++){
        cnt[i%m]++;
    }
    //枚举余数即可
    for(int i=0;i<m;i++){
        for(int j=0;j<m;j++){
            if((i*i+j*j)%m==0){
                ans+=(ll)cnt[i]*cnt[j];
            }
        }
    }
    // for(int i=0;i<m;i++){
    //     printf("%d ",cnt[i]);
    // }
    // printf("\n");
    printf("%lld\n",ans);
    return 0;
}
```



