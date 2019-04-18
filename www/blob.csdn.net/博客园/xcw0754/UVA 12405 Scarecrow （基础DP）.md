# UVA 12405 Scarecrow （基础DP） - xcw0754 - 博客园
# [UVA 12405 Scarecrow （基础DP）](https://www.cnblogs.com/xcw0754/p/4847137.html)
题意：
　　给出一个1*N的矩阵（就是一行的格子），其中部分格子可以有草，部分无草，现在要求放置一些稻草人在某些格子上，每个稻草人可以覆盖3个连续格子，为使得有草的格子都能被覆盖，问最少放置几个稻草人。
思路：
　　常规的DP。从左到右，考虑每个格子，无论是否有草，都可以放稻草人，也可以不放稻草人。然后进行各种转移就行了。
```
1 #include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 #include <cstring>
 5 #include <cmath>
 6 #include <map>
 7 #include <algorithm>
 8 #include <vector>
 9 #include <iostream>
10 #define pii pair<int,int>
11 #define INF 0x7f7f7f7f
12 #define LL unsigned long long
13 using namespace std;
14 const double PI  = acos(-1.0);
15 const int N=110;
16 char g[N];
17 int dp[N];
18 
19 
20 
21 
22 int cal(int n)
23 {
24     if(n==1)
25     {
26         if(g[1]=='.')   return 1;
27         else            return 0;
28     }
29     dp[0]=0;
30     for(int i=1; i<=n; i++)
31     {
32         if( g[i]=='.' ) //有种草
33         {
34             dp[i]=min(dp[i], dp[max(0,i-2)]+1);       //放,覆盖[i-1,i]
35             dp[i+1]=min(dp[i+1],dp[max(0,i-2)]+1);    //放,覆盖[i-1,i,i+1]
36         }
37         else            //无草
38         {
39             dp[i]=min(dp[i],dp[i-1]);                 //不放，因为无草
40             dp[i+1]=min(dp[i+1], dp[max(0,i-2)]+1);   //放，可以覆盖[i-1,i,i+1]三块
41             dp[i]=min(dp[i], dp[max(0,i-2)]+1);       //放，覆盖[i-1,i]。
42         }
43     }
44     return dp[n];
45 }
46 
47 
48 int main()
49 {
50     freopen("input.txt", "r", stdin);
51     int t, n, Case=0;
52     cin>>t;
53     while(t--)
54     {
55         memset(dp, 0x3f, sizeof(dp));
56         memset(g, 0, sizeof(g));
57         scanf("%d%s",&n, g+1 );
58         printf("Case %d: %d\n", ++Case, cal(n));
59     }
60 }
AC代码
```

