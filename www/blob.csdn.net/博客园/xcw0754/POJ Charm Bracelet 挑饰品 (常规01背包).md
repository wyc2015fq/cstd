# POJ Charm Bracelet 挑饰品 (常规01背包) - xcw0754 - 博客园
# [POJ Charm Bracelet 挑饰品 (常规01背包)](https://www.cnblogs.com/xcw0754/p/4474285.html)
问题：去珠宝店抢饰品，给出饰品种数n，能带走的重量m，以及每种饰品的重量w与价值v。求能带走的最大量。
思路：常规01背包。
```
1 #include <iostream>
 2 using namespace std;
 3 const int N=100000;
 4 int w[N], v[N],dp[N];
 5 
 6 void cal(int n, int m)
 7 {
 8     for(int i=0; i<n; i++)
 9         for(int j=m; j>=w[i]; j--)
10             dp[j]=max( dp[j-w[i]]+v[i], dp[j] );
11 
12     cout<<dp[m]<<endl;
13 }
14 
15 int main()
16 {
17     //freopen("input.txt","r",stdin);
18     int n, m;
19     while(cin>>n>>m)
20     {
21         for(int i=0; i<n; i++)    cin>>w[i]>>v[i];
22         cal(n, m);
23     }
24     return 0;
25 }
AC代码
```

