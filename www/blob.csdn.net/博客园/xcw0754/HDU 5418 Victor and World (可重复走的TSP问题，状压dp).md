# HDU 5418 Victor and World  (可重复走的TSP问题，状压dp) - xcw0754 - 博客园
# [HDU 5418 Victor and World  (可重复走的TSP问题，状压dp)](https://www.cnblogs.com/xcw0754/p/4752513.html)
题意：
　　每个点都可以走多次的TSP问题：有n个点（n<=16），从点1出发，经过其他所有点至少1次，并回到原点1，使得路程最短。
思路：
给了很多重边，选最小的留下即可。任意点可能无法直接到达，所以先执行一次floyd，算出任意点对之间可达的最短距离。
　　（1）先考虑穷举的方法，将2~n个这n-1个数字的所有组合情况都算一遍，复杂度是 15！=1 3076 7436 8000，那是真的TSP了，不可能实现。
　　（2）上面的方法中有没有多余的计算量？有的！里面还是有贪心可以运用的地方。对于当前遍历过了哪些点，我们只需要知道最后一个点是什么，中间的点的顺序是所所谓的，那么最后一个遍历的可以是2~n，而中间那些可以是其他的2~n中的数。起点1的距离更新为0，接下来递推就行了。递推方法是，穷举所有的中间状态s，然后以这些状态去穷举下一个到达的点（此点不在s中）。
　　状态方程是：  dp[s|(1<<(i-1))][i]=min(dp[s][j]+g[j][i] ); s表示已经遍历过的点，j是最后那个点，i是未遍历过的点，从j走到i。
　　290ms算可以了。
```
1 #include <bits/stdc++.h>
 2 #define pii pair<int,int>
 3 #define INF 0x3f3f3f3f
 4 #define LL long long
 5 using namespace std;
 6 const int N=17;
 7 int g[N][N], dp[1<<N][N];
 8 
 9 void floyd(int n)
10 {
11     for(int k=1; k<=n; k++)
12         for(int i=1; i<=n; i++)
13             for(int j=1; j<=n; j++)
14                 g[i][j]=min( g[i][j], g[i][k]+g[k][j]);
15 
16 }
17 
18 int cal( int n )
19 {
20     floyd(n);
21     memset(dp, 0x7f, sizeof(dp));
22     dp[1][1]=0;
23     for(int s=1; s<(1<<n); s+=2) //穷举状态
24     {
25         for(int i=2; i<=n; i++) //设最后访问的点是i。i!=0
26         {
27             if( ( 1<<(i-1) ) & s ) continue;  //s中的第i位必须为0，即未访问过。
28             for(int j=1; j<=n; j++)
29             {
30                 if( s&(1<<(j-1)) )           //穷举s中出现过的1的位置。
31                     dp[s|(1<<(i-1))][i]=min( dp[s|(1<<(i-1))][i], dp[s][j]+g[j][i] );
32             }
33         }
34     }
35     int ans=INF;
36     for(int i=2; i<=n; i++)  //最后访问的点不会是起点1。
37         ans=min(ans, dp[(1<<n)-1][i]+g[i][1]);
38     return ans==INF? 0: ans;        //只有1个点的情况
39 }
40 int main()
41 {
42     //freopen("input.txt", "r", stdin);
43     int t, n, m, a, b, c;
44     cin>>t;
45     while(t--)
46     {
47         scanf("%d%d", &n, &m);
48         memset(g, 0x3f, sizeof(g));
49         for(int i=0; i<=n; i++)    g[i][i]=0;     //初始化
50         for(int i=0; i<m; i++)
51         {
52             scanf("%d%d%d",&a,&b,&c);
53             g[b][a]=g[a][b]=min(g[a][b], c);
54         }
55         printf("%d\n", cal(n));
56     }
57     return 0;
58 }
AC代码
```

