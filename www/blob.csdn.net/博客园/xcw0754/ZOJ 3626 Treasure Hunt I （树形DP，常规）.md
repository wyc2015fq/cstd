# ZOJ  3626  Treasure Hunt I （树形DP，常规） - xcw0754 - 博客园
# [ZOJ  3626  Treasure Hunt I （树形DP，常规）](https://www.cnblogs.com/xcw0754/p/4831712.html)
题意：给一棵树，一个人站在节点s，他有m天时间去获取各个节点上的权值，并且最后需要回到起点s，经过每条边需要消耗v天，问最少能收获多少权值？
思路：　　常规的，注意还得跑回原地s。
```
1 //#include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 #include <cstring>
 5 #include <map>
 6 #include <algorithm>
 7 #include <vector>
 8 #include <iostream>
 9 #define pii pair<int,int>
10 #define INF 0x3f3f3f3f
11 #define LL  long long
12 using namespace std;
13 const int N=210;
14 int w[N], head[N], dp[N][N], edge_cnt, n;
15 struct node
16 {
17     int from,to,cost,next;
18     node(){};
19     node(int from,int to,int cost,int next):from(from),to(to),cost(cost),next(next){};
20 }edge[N*2];
21 
22 void add_node(int from,int to,int cost)
23 {
24     edge[edge_cnt]=node(from,to,cost,head[from]);
25     head[from]=edge_cnt++;
26 }
27 
28 void DFS(int t,int far,int m)
29 {
30     for(int j=m; j>=0; j--) dp[t][j]=w[t];  //本节点的权值
31     node e;
32     for(int i=head[t]; i!=-1; i=e.next)
33     {
34         e=edge[i];
35         if(e.to!=far && m-2*e.cost>=0)
36         {
37             DFS(e.to, t, m-2*e.cost);
38             for(int j=m; j>0; j--)
39                 for(int k=2*e.cost; k<=j; k++)  //给孩子k-2*cost天
40                     dp[t][j]=max(dp[t][j], dp[t][j-k]+dp[e.to][k-2*e.cost]);
41         }
42     }
43 }
44 
45 int main()
46 {
47     //freopen("input.txt", "r", stdin);
48     int a, b, c, s, m;
49     while(~scanf("%d",&n))
50     {
51         memset(head, -1, sizeof(head));
52         memset(dp, 0, sizeof(dp));
53        edge_cnt=0;
54 
55         for(int i=1; i<=n; i++) scanf("%d",&w[i]);
56         for(int i=1; i<n; i++)
57         {
58             scanf("%d%d%d",&a,&b,&c);
59             add_node(a,b,c);
60             add_node(b,a,c);
61         }
62         scanf("%d%d",&s,&m);
63         DFS(s,-1,m);
64         printf("%d\n",dp[s][m]);
65     }
66     return 0;
67 }
AC代码
```

