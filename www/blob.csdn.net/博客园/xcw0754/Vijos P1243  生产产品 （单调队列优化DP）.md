# Vijos  P1243   生产产品  （单调队列优化DP） - xcw0754 - 博客园
# [Vijos  P1243   生产产品  （单调队列优化DP）](https://www.cnblogs.com/xcw0754/p/4863124.html)
题意：
　　必须严格按顺序执行M个步骤来生产一个产品，每一个步骤都可以在N台机器中的任何一台完成。机器i完成第j个步骤的时间为T[i][j]。把半成品从一台机器上搬到另一台机器上也需要一定的时间K。每台机器最多只能连续完成产品的L个步骤。也就是说，如果有一台机器连续完成了产品的L个步骤，下一个步骤就必须换一台机器来完成。问一个产品最短需要多长时间呢？（对于100%的数据，N<=5， L<=50000，M<=100000）
题意：
　　被高中生虐了~
　　题意要求尽量缩短时间来完成一件产品，但是由于需要按照步骤，所以多线程的方式不必考虑。dp[i][j]=min（dp[k][p]）+（sum[i][j] - sum[k][j]），dp[i][j]表示完成前i个步骤，且最后一步是在第j台机器上完成。sum[k][j]表示第j台机器完成第1~k个步骤所需要的时间和，但是k与i的距离不宜超过规定的L。式子也可以这样写：dp[i][j]=min（dp[k][p] -  sum[k][j]）+ sum[i][j] ，那么就单单看这项min（dp[k][p] -  sum[k][j]）就行了，这不就是类似于在序列区间a[i-L]~a[i-1]之间找一个最小值的问题？那就可以用单调队列解决了。复杂度为O(n2*m)，是与L无关的。
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
17 const int N=100100;
18 const int M=10;
19 int m, n, K, L;
20 int t[N][M], dp[N][M];
21 int que[N][M], idx[N][M], top[M], rear[M];
22 
23 void calmin(int i)
24 {
25     for(int j=1; j<=n; j++)
26     {
27         int tar=INF;
28         for(int k=1; k<=n; k++)
29         {
30             if(j==k)    continue;
31             tar=min(tar, dp[i][k]);
32         }
33         tar-=t[i][j];
34         while( top[j]<rear[j] && que[rear[j]-1][j]>=tar )    rear[j]--;
35         que[rear[j]][j]=tar;
36         idx[rear[j]][j]=i;
37         rear[j]++;
38     }
39 }
40 
41 void init()
42 {
43     memset(dp,0x3f,sizeof(dp));
44     memset(top,0,sizeof(top));
45     memset(rear,0,sizeof(rear));
46     memset(dp[0],0,sizeof(dp[0]));
47     memset(que,0,sizeof(que));
48     memset(idx,0,sizeof(idx));
49     for(int i=1; i<=n; i++) //在单个机器上运行所有任务，求区间和
50         for(int j=1; j<=m; j++)
51             t[j][i]+=t[j-1][i];
52     for(int i=1; i<=n; i++)
53         rear[i]++;
54 }
55 int cal()
56 {
57     init();
58     for(int i=1; i<=m; i++) //枚举步骤
59     {
60         for(int j=1; j<=n; j++) //用第j台机器来完成i项
61         {
62             while( top[j]<rear[j] && idx[top[j]][j]<i-L)
63                 top[j]++;       //过期
64             int &d=dp[i][j];
65             if( top[j]==rear[j] )
66             {
67                 for(int k=1; k<=n; k++)
68                 {
69                     if(k==j)continue;
70                     d=min(d, dp[i-1][k]);
71                 }
72                 d+=t[i][j]-t[i-1][j];
73             }
74             else    d=min(d, que[top[j]][j]+t[i][j]+K );
75         }
76         calmin(i);
77     }
78 
79     int ans=INF;
80     for(int i=1; i<=n; i++)    ans=min(ans, dp[m][i]);
81     return ans-K;   //第一步不需要换机器费用
82 }
83 
84 
85 int main()
86 {
87     //freopen("input.txt","r",stdin);
88     while(~scanf("%d%d%d%d",&m, &n, &K, &L))
89     {
90         for(int i=1; i<=n; i++) //机器
91             for(int j=1; j<=m; j++) //步骤
92                 scanf("%d",&t[j][i]);
93         cout<<cal()<<endl;
94     }
95     return 0;
96 }
AC代码
```

