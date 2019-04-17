# hdu2068——RPG的错排 - westbrook1998的博客 - CSDN博客





2018年11月04日 13:12:05[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：42








问答对一半的方案数，也就是k个错，n-k个对，k从0到n/2枚举，然后k个错的就用错排公式，然后还要乘上从n个人中选出k个错的的组合数 ，两个都可以打表出来

代码:

```
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int N=30;
typedef long long ll;
ll a[N],c[N][N];
void init(){
    a[0]=1;
    a[1]=0;
    for(int i=2;i<N;i++){
        a[i]=(i-1)*(a[i-1]+a[i-2]);
    }
    c[0][0]=1;
    for(int i=1;i<N;i++){
        c[i][0]=c[i][i]=1;
        for(int j=1;j<i;j++){
            c[i][j]=c[i-1][j-1]+c[i-1][j];
        }
    }
}
int main(void){
    init();
    int n;
    while(~scanf("%d",&n) && n){
        ll ans=0;
        for(int i=0;i<=n/2;i++){
            ans+=c[n][i]*a[i];
        }
        printf("%lld\n",ans);
    }
    return 0;
}
```



