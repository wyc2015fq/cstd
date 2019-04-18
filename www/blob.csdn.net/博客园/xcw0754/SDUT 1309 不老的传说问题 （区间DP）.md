# SDUT 1309 不老的传说问题 （区间DP） - xcw0754 - 博客园
# [SDUT 1309 不老的传说问题 （区间DP）](https://www.cnblogs.com/xcw0754/p/4851226.html)
题意：
　　有一个环形序列，n个数字表示一种颜色，要求将白板环刷成一模一样的环，限制是每次最多只能刷连续的K个位置，问最少需要刷几次？
思路：
　　跟2008长春那道painter string 差不多。只是这次是个环，难度也是没有提升的，只需要变成一个2*n-1个数字的序列就可以了。
　　考虑区间[L,R]，如果[L]和[L+1,R]中的某一个颜色相同，才有可能减少刷的次数。那么从左到右枚举这个和[L]相同颜色的位置，[L,R]的次数就可以变成[L+1,k]+[k+1,R]了。可以想象成[L]是依靠另一个同颜色的位置来获得免刷的可能，则这个位置必定是距离它K个位置之内的。如果长度为K的某一段区间[L,L+K-1]中有多段分散的同颜色的，有没有可能是刷一次那个颜色，然后其他不同颜色的再截成一段一段的，将次数给组合起来呢？其实这种情况在枚举依靠位置k的时候已经考虑了，假设你选择依靠[L+K-1]，那么[L+1,L+K-2]中还有和[L]是同颜色的，而区间[L+1,L+K-1]已经是最优，其他的同色位置能不能也依靠[L+K-1]已经不是本次要考虑的问题了，本次只考虑能否让[L]依靠其他的位置从而获得免刷。
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
15 const int N=410;
16 int n, c, K;
17 int dp[N][N], a[N];
18 int cal()
19 {
20     memset(dp,0,sizeof(dp));
21     for(int i=1; i<=2*n; i++)
22         for(int j=i; j<=2*n; j++)
23             dp[i][j]=INF;
24     for(int j=1; j<2*n; j++)
25     {
26         for(int i=j; i>0; i--)
27         {
28             dp[i][j]=dp[i+1][j]+1;
29             for(int k=i+1; k<i+K&&k<=j; k++ )
30             {
31                 if(a[i]!=a[k])  continue;
32                 dp[i][j]=min(dp[i][j], dp[i+1][k]+dp[k+1][j]);
33             }
34         }
35     }
36     int ans=INF;
37     for(int i=1; i<=n; i++)
38     {
39         ans=min(ans, dp[i][i+n-1]);
40     }
41     return ans;
42 }
43 
44 int main()
45 {
46     //freopen("input.txt","r",stdin);
47     while(~scanf("%d%d%d",&n,&c,&K))
48     {
49         for(int i=1; i<=n; i++)
50         {
51             scanf("%d",&a[i]);
52             a[i+n]=a[i];
53         }
54         cout<<cal()<<endl;
55     }
56     return 0;
57 }
AC代码
```

