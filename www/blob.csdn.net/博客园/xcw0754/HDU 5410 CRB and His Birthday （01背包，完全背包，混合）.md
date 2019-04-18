# HDU 5410  CRB and His Birthday （01背包，完全背包，混合） - xcw0754 - 博客园
# [HDU 5410  CRB and His Birthday （01背包，完全背包，混合）](https://www.cnblogs.com/xcw0754/p/4745875.html)
题意：有n种商品，每种商品中有a个糖果，如果买这种商品就送多b个糖果，只有第一次买的时候才送。现在有m元，最多能买多少糖果？
思路：第一次买一种商品时有送糖果，对这一次进行一次01背包，也就是只能买一次。然后对这种商品来一次完全背包，此时不送糖果，也可以多买。
```
1 #include <bits/stdc++.h>
 2 #define pii pair<int,int>
 3 #define INF 0x7f7f7f7f
 4 #define LL long long
 5 using namespace std;
 6 const int N=2050;
 7 LL dp[N];
 8 int n, m, w[N], a[N], b[N];
 9 
10 LL cal()
11 {
12     memset(dp, 0, sizeof(dp));
13     for(int i=1; i<=n; i++)  //礼物
14     {
15         for(int j=m; j-w[i]>=0; j--) //先来一次01背包
16         {
17             if( dp[j-w[i]]+ a[i] + b[i] > dp[j] )
18                 dp[j]=dp[j-w[i]]+ a[i] + b[i]  ;
19         }
20         for(int j=0; j+w[i]<=m; j++) //再来一次完全背包
21         {
22             if( dp[j+w[i]] < dp[j]+a[i] )
23                 dp[ j+w[i] ] = dp[j]+a[i] ;
24         }
25     }
26     LL sum=-1;
27     for(int i=m; i>0; i--)    sum=max(sum, dp[i]);
28     return sum;
29 }
30 
31 int main()
32 {
33     freopen("input.txt", "r", stdin);
34     int t;
35     cin>>t;
36     while(t--)
37     {
38         scanf("%d %d", &m, &n);
39         for(int i=1; i<=n; i++)    scanf("%d%d%d",&w[i], &a[i], &b[i]);
40         printf("%lld\n", cal());
41     }
42 
43     return 0;
44 }
AC代码
```

