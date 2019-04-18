# ZOJ  3471  Most Powerful （状压DP，经典） - xcw0754 - 博客园
# [ZOJ  3471  Most Powerful （状压DP，经典）](https://www.cnblogs.com/xcw0754/p/4860160.html)
题意：
　　有n个原子，每当两个原子碰撞时就会产生能量，并且消耗其中一个原子。已知每两个原子碰撞时消耗其中指定一个原子所产生的能量，问最多能产生多少能量？
思路：
　　一开始以为是找一个有序序列，使得能量最大，但是一个原子可能一直和别的原子碰撞且留到最后，所以序列是不行的。
　　我们只需要知道消耗的是谁就行了，而不需要管到底是谁碰撞到这个被消耗的原子，那么状态转移就更方便了，每次找到两个还没有碰撞的，碰掉其中1个，然后转移。
```
1 //#include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 #include <cstring>
 5 #include <cmath>
 6 #include <deque>
 7 #include <map>
 8 #include <algorithm>
 9 #include <vector>
10 #include <iostream>
11 #define pii pair<int,int>
12 #define INF 0x3f3f3f3f
13 #define LL long long
14 #define ULL unsigned long long
15 using namespace std;
16 const double PI  = acos(-1.0);
17 const int N=11;
18 int g[N][N], dp[1<<10];
19 
20 int cal(int n)
21 {
22     memset(dp,0,sizeof(dp));
23     dp[0]=0;
24     for(int s=0; s<(1<<n); s++)
25     {
26         for(int i=1; i<=n; i++)
27         {
28             if( s&(1<<i-1) )   continue;
29             for(int j=1; j<=n; j++) //炸掉j
30             {
31                 if(i==j)    continue;
32                 if( s&(1<<j-1) )   continue;
33                 dp[s|(1<<j-1)]=max(dp[s|(1<<j-1)],dp[s]+g[i][j] );
34             }
35         }
36     }
37     int ans=-10086;
38     for(int i=1; i<(1<<n); i++)
39         ans=max(ans, dp[i]);
40     return ans;
41 }
42 
43 int main()
44 {
45     //freopen("input.txt","r",stdin);
46     int n;
47     while(scanf("%d",&n),n)
48     {
49         memset(g,0,sizeof(g));
50         for(int i=1; i<=n; i++)
51             for(int j=1; j<=n; j++)
52                 scanf("%d",&g[i][j]);
53         printf("%d\n",cal(n));
54     }
55     return 0;
56 }
AC代码
```

