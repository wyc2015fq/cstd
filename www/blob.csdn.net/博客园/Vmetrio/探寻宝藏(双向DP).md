# 探寻宝藏(双向DP) - Vmetrio - 博客园







# [探寻宝藏(双向DP)](https://www.cnblogs.com/wangmengmeng/p/5524227.html)





### 题目描述


传说HMH大沙漠中有一个M*N迷宫，里面藏有许多宝物。某天，Dr.Kong找到了迷宫的地图，他发现迷宫内处处有宝物，最珍贵的宝物就藏在右下角，迷宫的进出口在左上角。当然，迷宫中的通路不是平坦的，到处都是陷阱。Dr.Kong决定让他的机器人卡多去探险。

但机器人卡多从左上角走到右下角时，只会向下走或者向右走。从右下角往回走到左上角时，只会向上走或者向左走，而且卡多不走回头路。（即：一个点最多经过一次）。当然卡多顺手也拿走沿路的每个宝物。

Dr.Kong希望他的机器人卡多尽量多地带出宝物。请你编写程序，帮助Dr.Kong计算一下，卡多最多能带出多少宝物。




### 输入

第一行： K 表示有多少组测试数据。 
接下来对每组测试数据：
第1行: M N
第2~M+1行： Ai1 Ai2 ……AiN (i=1,…..,m)


【约束条件】
2≤k≤5 1≤M, N≤50 0≤Aij≤100 (i=1,….,M; j=1,…,N)
所有数据都是整数。 数据之间有一个空格。




### 输出

对于每组测试数据，输出一行：机器人卡多携带出最多价值的宝物数




### 样例输入
2
2 3
0 10 10
10 10 80
3 3
0 3 9
2 8 5
5 7 100


### 样例输出
120
134


### 来源

[NYOJ](http://115.159.40.116/problem.php#searchstr=NYOJ)



题解(1):  http://www.l-ch.net/26112.html

这道题和以往我们做的dp不同之处就在于 是一去一回 

加入只有去 我们可以 用动态规划方程  dp[i][j]=max(dp[i-1][j],dp[i][j-1])+map[i][j].

而这道题去了又回来 我们可以理解为两个人同时从左上角去 不过不走相同的路  

如果两个人不走相同的路 那么这两个人必须不在相同的列或者行 又因为 两个人走的步数完全相同 

所以我们可以通过一个人走的步数得到另外一个人走的步数

我们可以通过一个四维的数组来保存

于是这个时候的动态规划方程  

`dp[i][j][k][l]=max(max(dp[i-1][j][k-1][l],dp[i-1][j][k][l-1]),`

```max(dp[i][j-1][k-1][l],dp[i][j-1][k][l-1]))+map[i][j]+map[k][l];`





题解(2):玉民的思路...三维数组...







代码:


```
1 #include <cstdio>
 2 #include <cmath>
 3 #include <cstring>
 4 #include <string>
 5 #include <algorithm>
 6 #include <queue>
 7 #include <stack>
 8 #include <map>
 9 #include <set>
10 #include <vector>
11 #include <iostream>
12 using namespace std;
13 #define for0(i, n) for(int i=0; i<(n); ++i)
14 #define for1(i,a,n) for(int i=(a);i<=(n);++i)
15 #define for2(i,a,n) for(int i=(a);i<(n);++i)
16 #define for3(i,a,n) for(int i=(a);i>=(n);--i)
17 #define for4(i,a,n) for(int i=(a);i>(n);--i)
18 #define CC(i,a) memset(i,a,sizeof(i))
19 #define ll long long
20 #define MOD 1000000007
21 #define inf 0x3f3f3f3f
22 
23 ll dp[155][55][55];
24 ll mp[55][55];
25 
26 int k,n,m,p,q;
27 
28 int main()
29 {
30     int l;
31     scanf("%d",&k);
32     while(k--){
33         scanf("%d%d",&n,&m);
34         for(int i=1; i<=n; i++)
35             for(int j=1; j<=m; j++)
36                 scanf("%lld",&mp[i][j]);
37         memset(dp,0,sizeof(dp));
38         dp[2][1][1]=mp[1][1];
39         for(int l=3; l<n+m; l++)
40             for(int i=1; i<=n ;i++)
41                 for(int j=1; j<=n; j++){
42                     p=l-i;
43                     q=l-j;
44                     if(p<1 || q<1)  break;
45                     if(p>m || q>m)  continue;
46                     if(p==q) continue;
47                     dp[l][i][j]=max(max(dp[l-1][i-1][j],dp[l-1][i-1][j-1]),max(dp[l-1][i][j-1],dp[l-1][i][j]));
48                     //dp[k][i][j]=max(max(dp[k-1][i-1][j],dp[k-1][i-1][j-1]),max(dp[k-1][i][j-1],dp[k-1][i][j]));
49                     dp[l][i][j]+=mp[i][p]+mp[j][q];
50                     //dp[k][i][j]+=map[i][p]+map[j][q];
51                 }
52         dp[n+m][n][n]=max(max(dp[m+n-1][n-1][n],dp[m+n-1][n-1][n-1]),max(dp[n+m-1][n][n-1],dp[n+m-1][n][n]));
53         printf("%lld\n",dp[n+m][n][n]+mp[n][m]);
54     }
55    
56 }
```

















