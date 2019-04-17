# 1260 Tickets（一维简单dp） - PeterBishop - CSDN博客





2018年12月04日 15:01:38[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：25








### 简单易懂，水题 

```cpp
#include<iostream>

#include<cstdio>

#include<cstring>

#include<cstdlib>

#include<cmath>

#include<algorithm>

using namespace std;


int main()
{
    int a[2001];
    int b[2001];
    int N;
    while (~scanf("%d", &N)) {
        while (N--) {
               int K;
               scanf("%d", &K);
               for(int i=1;i<=K;i++){
                   scanf("%d",&a[i]);
               }
               for(int i=2;i<=K;i++){
                   scanf("%d",&b[i] );
               }
               int dp[2001];
               dp[0]=0;
               dp[1]=a[1];
               for(int i=2;i<=K;i++){
                   dp[i]=min(dp[i-1]+a[i],dp[i-2]+b[i]);
               }
               int h = dp[K] / 3600 + 8;
               int m = dp[K] / 60 % 60;
               int s = dp[K] % 60;
               printf("%02d:%02d:%02d am\n", h, m, s);

        }
    }
}
```





