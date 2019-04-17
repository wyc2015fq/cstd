# codeforces1114D——Flood Fill - westbrook1998的博客 - CSDN博客





2019年02月11日 15:35:44[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：109








每个格子有一个颜色c[i]。每次可以选择相同颜色的一段变成另一种颜色，然后求把整段变成同一种颜色的最小次数

区间dp

先初始化dp[i][i]为0，然后大区间由小区间转移而来，是否需要次数加1就取决于a[l-1]/a[r+1]和原本区间[l,r]的两端是否相同

因为比如两个区间的合并，5 5 5 1合并后的颜色只能是a[l]或者a[r]也就是5 5 5 5或1 1 1 1
```cpp
#include <bits/stdc++.h>
using namespace std;
const int N=5050;
const int INF=0x3f3f3f3f;
//定义dp[l][r][0/1]为区间[l,r]调整为同一颜色(0/1)所需次数
int dp[N][N][2],a[N];
int n;
int main(void){
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        scanf("%d",&a[i]);
    }
    //init
    for(int l=0;l<n;l++){
        for(int r=0;r<n;r++){
            //初始化单点区间为0，dp转移从小区间转移到大区间
            dp[l][r][0]=dp[l][r][1]=(l==r?0:INF);
        }
    }
    //注意循环边界
    for(int r=0;r<n;r++){
        for(int l=r;l>=0;l--){
            if(l>0){
                dp[l-1][r][0]=min(dp[l-1][r][0],min(dp[l][r][0]+!(a[l-1]==a[l]),dp[l][r][1]+!(a[l-1]==a[r])));
            }
            if(r<n-1){
                dp[l][r+1][1]=min(dp[l][r+1][1],min(dp[l][r][0]+!(a[r+1]==a[l]),dp[l][r][1]+!(a[r+1]==a[r])));
            }
        }
    }
    printf("%d\n",min(dp[0][n-1][0],dp[0][n-1][1]));
    return 0;
}
```




