# POJ  3311  Hie with the Pie  （状压DP） - xcw0754 - 博客园
# [POJ  3311  Hie with the Pie  （状压DP）](https://www.cnblogs.com/xcw0754/p/4858888.html)
题意：
　　每个点都可以走多次的TSP问题：有n个点（n<=11），从点1出发，经过其他所有点至少1次，并回到原点1，使得路程最短是多少？
思路：
　　同[HDU 5418 VICTOR AND WORLD (可重复走的TSP问题，状压DP)](http://www.cnblogs.com/xcw0754/p/4752513.html)这道题几乎一模一样。
```
1 //#include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 #include <cstring>
 5 #include <cmath>
 6 #include <map>
 7 #include <algorithm>
 8 #include <vector>
 9 #include <iostream>
10 #define pii pair<int,int>
11 #define INF 0x3f3f3f3f
12 #define LL long long
13 #define ULL unsigned long long
14 using namespace std;
15 const double PI  = acos(-1.0);
16 const int N=12;
17 
18 int g[N][N], dp[1<<11][N];
19 void floyd(int n)
20 {
21     for(int k=1; k<=n; k++)
22         for(int i=1; i<=n; i++)
23             for(int j=1; j<=n; j++)
24                 g[i][j]=min( g[i][j], g[i][k]+g[k][j]);
25 
26 }
27 
28 int cal( int n )
29 {
30     floyd(n);
31     memset(dp, 0x7f, sizeof(dp));
32     dp[1][1]=0;
33 
34     for(int s=1; s<(1<<n); s++)   //枚举状态
35     {
36         for(int i=1; i<=n; i++) //枚举中间点:必须已访问
37         {
38             if( s&(1<<i-1) )
39             {
40                 for(int j=1; j<=n; j++)           //枚举终点
41                 {
42                     int &q=dp[s|(1<<(j-1))][j];
43                     q=min(q, dp[s][i]+g[i][j] );
44                 }
45             }
46         }
47     }
48     int ans=dp[(1<<n)-1][1];
49     for(int i=2; i<=n; i++)
50         ans=min(ans, dp[(1<<n)-1][i]  +g[i][1]);
51     return ans;        //只有1个点的情况
52 }
53 
54 int main()
55 {
56     //freopen("input.txt","r",stdin);
57     int n;
58     while(scanf("%d",&n), n)
59     {
60         memset(g,0x3f,sizeof(g));
61         ++n;
62         for(int i=1; i<=n; i++)
63             for(int j=1; j<=n; j++)
64                 scanf("%d",&g[i][j]);
65 
66         printf("%d\n",cal(n));
67     }
68     return 0;
69 }
AC代码
```

