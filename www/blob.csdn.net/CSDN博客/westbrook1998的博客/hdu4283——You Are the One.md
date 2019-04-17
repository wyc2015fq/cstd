# hdu4283——You Are the One - westbrook1998的博客 - CSDN博客





2019年01月20日 23:50:17[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：42








```
#include <bits/stdc++.h>
using namespace std;
const int N=105;
const int INF=0x3f3f3f3f;
int t,n;
int v[N];
int dp[N][N],pre[N];
int main(void){
    scanf("%d",&t);
    int c=1;
    while(t--){
        scanf("%d",&n);
        for(int i=1;i<=n;i++){
            scanf("%d",&v[i]);
            pre[i]=pre[i-1]+v[i];
        }
        memset(dp,0,sizeof(dp));
        for(int i=1;i<=n;i++){
            for(int j=i+1;j<=n;j++){
                dp[i][j]=INF;
            }
        }
        //枚举区间长度从大到小！
        for(int len=2;len<=n;len++){
            //枚举起点
            for(int i=1;i+len-1<=n;i++){
                //终点
                int j=i+len-1;
                //枚举a[i]在区间[i,j]里第k个出场
                //所以出场顺序是先a[i+1...i+k-1]，然后是a[i]，然后是a[i+k...j]
                //前面的顺序直接加，a[i]等待了k-1个人，后面整个部分是一个已经算出来的子问题最优解，每个都多等待了k个人
                for(int k=1;k<=j-i+1;k++){
                    dp[i][j]=min(dp[i][j],dp[i+1][i+k-1]+dp[i+k][j]+v[i]*(k-1)+(pre[j]-pre[i+k-1])*k);
                }
            }
        }
        // for(int i=1;i<=n;i++){
        //     for(int j=1;j<=n;j++){
        //         printf("%d ",dp[i][j]);
        //     }
        //     printf("\n");
        // }
        printf("Case #%d: %d\n",c++,dp[1][n]);
    }
    return 0;
}
```



