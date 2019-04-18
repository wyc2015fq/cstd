# POJ 2411 Mondriaan's Dream （状压DP，骨牌覆盖，经典） - xcw0754 - 博客园
# [POJ 2411 Mondriaan's Dream （状压DP，骨牌覆盖，经典）](https://www.cnblogs.com/xcw0754/p/4860337.html)
题意：
　　用一个2*1的骨牌来覆盖一个n*m的矩形，问有多少种方案？(1<=n,m<=11)
![](https://images2015.cnblogs.com/blog/641737/201510/641737-20151008104819049-423108625.png)
思路：
　　很经典的题目，如果n和m都是奇数，那么答案为0。同uva11270[这道题](http://acm.hust.edu.cn/vjudge/problem/viewProblem.action?id=33787)。
　　只需要m个bit来记录状态行了，标记是否已经被覆盖到了。考虑当前格子，如果上面格子未覆盖，则必须放竖的，否则，将再也覆盖不到此格子；如果上面格子已经覆盖，而左边未覆盖，那么还可以选择放横的，或者是不放（左边若未覆盖可以由左下格子去考虑）。
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
18 LL dp[2][1<<11], cur;
19 
20 LL cal(int n,int m)
21 {
22     if(n==m&&n%2==1)    return 0;
23     if(n<m) swap(n,m);
24     memset(dp[0],0,sizeof(dp[0]));
25     dp[cur=0][(1<<m)-1]=1;
26     for(int i=1; i<=n; i++)
27     {
28         for(int j=1; j<=m; j++)
29         {
30             cur^=1;
31             memset(dp[cur],0,sizeof(dp[cur]));
32             int r=(1<<m)-1;
33             for(int  s=0; s<(1<<m); s++)
34             {
35                 LL v=dp[cur^1][s];
36                 if( (s&(1<<m-1))==0 )               //必须放竖
37                     dp[cur][(s<<1)+1]+=v;
38                 else
39                 {
40                     dp[cur][(s<<1)&r]+=v; //不放
41                     if( j>1 && (s&1)==0 )          //放横
42                         dp[cur][((s<<1)+3)&r]+=v;
43                 }
44             }
45         }
46     }
47     return dp[cur][(1<<m)-1];
48 }
49 
50 int main()
51 {
52     freopen("input.txt","r",stdin);
53     int n, m;
54     while(scanf("%d%d",&n,&m),n+m)
55         printf("%lld\n",cal(n,m));
56     return 0;
57 }
AC代码
```

