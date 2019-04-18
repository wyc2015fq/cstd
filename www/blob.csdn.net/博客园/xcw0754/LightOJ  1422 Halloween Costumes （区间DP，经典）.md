# LightOJ   1422  Halloween Costumes  （区间DP，经典） - xcw0754 - 博客园
# [LightOJ   1422  Halloween Costumes  （区间DP，经典）](https://www.cnblogs.com/xcw0754/p/4841207.html)
题意：
　　有个人要去参加万圣节趴，但是每到一个趴都要换上特定的服装，给定一个序列表示此人要穿的衣服编号（有先后顺序的），他可以套很多件衣服在身上，但此人不喜欢再穿那些脱下的衣服（即脱下后就必须换新的），问最少需要穿多少件衣服？
思路：
　　如果多件一样的相连的话就可以只穿1件。将问题化为小问题，再来连接起来，假设区间[i->j]，如果color[i]和后面其中某一个颜色(假设下标为k)一样，那么color[i]=color[k]。那么第i件就可能可以不穿，当且仅当 dp[i+1][k-1]+dp[k][j] 小于 dp[i+1][j]，那么我们需要事先知道这三个量应该是多少，因此可以从小区间的先开始枚举，再枚举k。复杂度O(n3)。
```
1 #include <bits/stdc++.h>
 2 #define pii pair<int,int>
 3 #define max(x,y) ((x)>(y)?(x):(y))
 4 #define min(x,y) ((x)<(y)?(x):(y))
 5 #define abs(x) ((x)<0?-(x):(x))
 6 #define INF 0x3f3f3f3f
 7 #define LL  long long
 8 using namespace std;
 9 const double PI  = acos(-1.0);
10 const int N=110;
11 int c[N];   //颜色
12 int dp[N][N];
13 int main()
14 {
15     //freopen("input.txt", "r", stdin);
16     int t, n, Case=0;
17     cin>>t;
18     while(t--)
19     {
20         scanf("%d",&n);
21         for(int i=1; i<=n; i++) scanf("%d",&c[i]);
22 
23         memset(dp,0,sizeof(dp));
24         for(int i=1; i<=n; i++) dp[i][i]=1;
25         for(int j=2; j<=n; j++)
26         {
27             for(int i=j-1; i>0; i--)        //当前考虑衣服i
28             {
29                 dp[i][j]=dp[i+1][j]+1;
30                 for(int k=i+1; k<=j; k++)   //枚举k
31                 {
32                     if(c[i]==c[k])          //相同了才可以减少穿的次数
33                         dp[i][j]=min(dp[i][j], dp[i+1][k-1]+dp[k][j]);
34                 }
35             }
36         }
37         printf("Case %d: %d\n", ++Case, dp[1][n]);
38     }
39     return 0;
40 }
AC代码
```

