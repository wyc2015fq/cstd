# HDU  4283  You Are the One （区间DP，经典） - xcw0754 - 博客园
# [HDU  4283  You Are the One （区间DP，经典）](https://www.cnblogs.com/xcw0754/p/4850538.html)
题意：
　　某校举行一场非诚勿扰，给定一个出场序列，表示n个人的屌丝值，如果他是第k个出场的，他的不满意度为(k-1)*diao[i]。为了让所有人的屌丝值之和更小，导演设置一个栈，可以将部分人装进栈中，来改变序列。也就是说，要么按照给定序列上场，要么将某个人前面部分人装进栈，让该人先出场，再让栈中的人出场，都行。问屌丝值之和最少可以是多少？
思路：
　　想挫了，哎～
　　如果考虑区间p[1->n]的出场次序时，只考虑p[1]是第几个出场的就行了，假设是第k个，那么区间p[2->k]就必须比p[1]先出场（想想为什么），而p[k+1->n]就在p[1]之后出场了。那么不就分成了两个子问题了吗？只要在处理[L,R]时保证其任一子区间都已经考虑过了，就可以完成转移了。比p[j]后出场的，整个区间的dp值要上升了，因为他们的出场顺序变成k+1~n了。
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
11 #define INF 0x7f3f3f3f
12 #define LL long long
13 using namespace std;
14 const double PI  = acos(-1.0);
15 const int N=110;
16 
17 int d[N], dp[N][N], sum[N], n;
18 
19 int cal()
20 {
21     memset(dp, 0, sizeof(dp));    //初始化
22     for(int i=1; i<=n; i++)
23         for(int j=i+1; j<=n; j++)
24             dp[i][j]=INF;
25 
26     for(int j=2; j<=n; j++)
27     {
28         for(int i=j-1; i>0; i--)
29         {
30             for(int k=0; k<=j-i; k++) //考虑让d[i]成为第k+1个出场
31             {
32                 int t= d[i]*k + dp[i+1][i+k] + dp[i+k+1][j]+(k+1)*(sum[j]-sum[i+k]);
33                 dp[i][j]=min(dp[i][j], t );
34             }
35         }
36     }
37     return dp[1][n];
38 }
39 
40 int main()
41 {
42     freopen("input.txt", "r", stdin);
43     int t, Case=0;cin>>t;
44     while( t-- )
45     {
46         sum[0]=0;
47         scanf("%d",&n);
48         for(int i=1; i<=n; i++)
49         {
50             scanf("%d",&d[i]);
51             sum[i]=sum[i-1]+d[i];
52         }
53         printf("Case #%d: %d\n",++Case, cal());
54     }
55     return 0;
56 }
AC代码
```

