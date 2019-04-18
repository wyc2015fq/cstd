# UVA 674   Coin Change 硬币转换（完全背包，常规） - xcw0754 - 博客园
# [UVA 674   Coin Change 硬币转换（完全背包，常规）](https://www.cnblogs.com/xcw0754/p/4489495.html)
题意:有5种硬币，个数无限的，组成n元的不同方案有多少种?
思路:常规完全背包。重点在dp[0]=1，dp[j]中记录的是组成 j 元的方案数。状态转移方程dp[j+coin[i]]+=dp[j]。
```
1 #include <bits/stdc++.h>
 2 using namespace std;
 3 int coin[]={1, 5, 10, 25, 50};
 4 int dp[10000], n;
 5 
 6 int cal()
 7 {
 8     if(!n)  return 1;
 9     memset(dp,0,sizeof(dp));
10     dp[0]=1;
11     for(int i=0; i<5; i++)
12         for(int j=0; j+coin[i]<=n; j++)
13             dp[j+coin[i]]+=dp[j];
14     return dp[n];
15 }
16 int main() {
17     //freopen("input.txt", "r", stdin);
18     while(~scanf("%d",&n))
19         printf("%d\n",cal());
20 
21     return 0;
22 }
AC代码
```

