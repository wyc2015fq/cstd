# ZOJ 3469 Food Delivery （区间DP，经典） - xcw0754 - 博客园
# [ZOJ 3469 Food Delivery （区间DP，经典）](https://www.cnblogs.com/xcw0754/p/4849298.html)
题意：
　　在x轴上有一家外卖餐馆，有n个顾客站在x轴上不同坐标上且叫了外卖，每个人的脾气不同，每1分钟没有收到外卖就会增加Fi点愤怒值，而外卖小哥的车是有速度的v-1/分钟，问怎样的送餐次序会让所有顾客的愤怒值之和最小？输出愤怒值之和！
思路：
　　此题是很经典了，比较现实的模型。
　　随便画画就知道小哥可以一下子往左一下子往右走，往返多次也是有可能的，取决于顾客的愤怒系数Fi。那么在考虑一个区间[L,R]时，其任一子区间都必须是已经被考虑过了。现在考虑区间[L,R]可以转移到哪里，明显可以分别转移到[L-1,R]和[L,R+1]，也就是往区间外送去1个人的外卖。由于送完区间[L,R]所有外卖后可能停在左/右边，得到的DP值不同，所以可以增加1维来区分送完后停的位置，设为dp[L][R][0/1]来记录愤怒之和。
　　这样还没有完，如果仅考虑当前区间[L,R]的顾客的愤怒值之和的话，无论怎样记录还是难以实现转移（这也是比较巧的地方）。但是如果你将其他未送达的顾客的愤怒值也先算进dp值的话就好转移了，比如区间[L,R]转移到[L,R+1]，那么[1,L-1]和[R+2,n]这些顾客就还在等外卖，每过1分钟他们的愤怒值也在增加，可以加到[L,R+1]的dp值进行考虑。
　　有没有可能dp[L][R][0]会转移到dp[L][R+1][0]？也就是从L走到R+1后还回到L处。经过推算，并不需要这样，不是很难想的。
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
13 using namespace std;
14 const double PI  = acos(-1.0);
15 const int N=1010;
16 struct node
17 {
18     int x, f;
19 }p[N];
20 int dp[N][N][2], sum[N];
21 int n, V, x, pos;
22 inline int cmp(node a,node b){return a.x<b.x;}
23 
24 void init(int pos)
25 {
26     sum[0]=0;
27     for(int i=1; i<=n; i++)    sum[i]=sum[i-1]+p[i].f;
28 
29     for(int i=1; i<=n; i++)     //初始化
30         for(int j=i; j<=n; j++)
31             dp[i][j][0]=dp[i][j][1]=INF;
32     dp[pos][pos][0]=dp[pos][pos][1]=0;
33 }
34 
35 int cal(int pos)
36 {
37     for(int j=pos; j<=n; j++)
38     {
39         for(int i=pos; i>0; i--)
40         {
41             int f=sum[i-1]+sum[n]-sum[j];   //f值之和*
42             int L=dp[i][j][0], R=dp[i][j][1];
43 
44             dp[i-1][j][0]=min(dp[i-1][j][0], L+f*(p[i].x-p[i-1].x));    //往左
45             dp[i-1][j][0]=min(dp[i-1][j][0], R+f*(p[j].x-p[i-1].x));
46 
47             dp[i][j+1][1]=min(dp[i][j+1][1], L+f*(p[j+1].x-p[i].x));    //往右
48             dp[i][j+1][1]=min(dp[i][j+1][1], R+f*(p[j+1].x-p[j].x));
49         }
50     }
51     return min(dp[1][n][0], dp[1][n][1])*V;
52 }
53 
54 int main()
55 {
56     //freopen("input.txt", "r", stdin);
57     while(~scanf("%d%d%d",&n,&V,&x))
58     {
59         for(int i=1; i<=n; i++)
60             scanf("%d%d", &p[i].x, &p[i].f);
61 
62         p[++n].x=x, p[n].f=0;   //添加餐馆
63         sort(p+1, p+n+1, cmp);
64 
65         for(int i=1; i<=n; i++)
66         {
67             if(p[i].x==x)   //找到餐馆
68             {
69                 init(i);
70                 cout<<cal(i)<<endl;
71                 break;
72             }
73         }
74     }
75     return 0;
76 }
AC代码
```

