# POJ 2486 Apple Tree （树形DP，树形背包） - xcw0754 - 博客园
# [POJ 2486 Apple Tree （树形DP，树形背包）](https://www.cnblogs.com/xcw0754/p/4824467.html)
题意：给定一棵树图，一个人从点s出发，只能走K步，每个点都有一定数量的苹果，要求收集尽量多的苹果，输出最多苹果数。
思路：
　　既然是树，而且有限制k步，那么树形DP正好。
　　考虑1个点的情况：（1）可能在本子树结束第k步（2）可能经过了j步之后，又回到本节点（第k步不在本子树）
　　第二种比较简单，背包一下，就是枚举给本节点的孩子t多少步，收集到最多苹果数。第一种的话要求第k步终止于本节点下的某个子树中，那么只能在1个孩子子树中，所以应该是【其他孩子全部得走回来】+【本孩子不要求走回来】   or   【其他某个孩子中不走回来】+【本节点走回来】。
　　用两个DP数组区分下“回”与“不回”就行了，注意，“不回”只能有1个孩子不要求其走回来，“回”是全部回。而“不要求回来”收集到的苹果数必定大于等于“要求回来”。
```
1 //#include <bits/stdc++.h>
 2 #include <cstdio>
 3 #include <cstring>
 4 #include <iostream>
 5 #define pii pair<int,int>
 6 #define INF 0x3f3f3f3f3f3f3f3f
 7 #define LL  long long
 8 using namespace std;
 9 const int N=210;
10 struct node
11 {
12     int from,to,next;
13     node(){};
14     node(int from,int to,int next):from(from),to(to),next(next){};
15 }edge[N*2];
16 int edge_cnt, head[N], w[N];
17 void add_node(int from,int to)
18 {
19     edge[edge_cnt]=node(from, to, head[from]);
20     head[from]=edge_cnt++;
21 }
22 /*
23     dp[][][1]   记录每次都回到本节点的。
24     dp[][][0]   记录仅1次不回到本节点的。
25 */
26 int dp[N][N][2];
27 void DFS(int t,int far,int m)
28 {
29     node e;
30     for(int j=0; j<=m; j++)    dp[t][j][0]=dp[t][j][1]=w[t];    //既然能到这,至少带上本节点
31     if(m==0)     return;
32     for(int i=head[t]; i!=-1; i=e.next)
33     {
34         e=edge[i];
35         if( e.to^far )
36         {
37             DFS(e.to, t, m-1);
38             for(int j=m; j>0; j-- )
39             {
40                 for(int k=0; k+2<=j; k++ )
41                 {
42                     //所有分支都回。
43                     dp[t][j][1]=max( dp[t][j][1], dp[t][j-k-2][1]+dp[e.to][k][1] );
44                     //本分支要回,但在其他分支不回。因为已经有1个不回了,所以更新在‘[0]’中。
45                     dp[t][j][0]=max( dp[t][j][0], dp[t][j-k-2][0]+dp[e.to][k][1] );
46                 }
47                 for(int k=0; k+1<=j; k++ )
48                 {
49                     //不回,但其他分支就必须全回。
50                     dp[t][j][0]=max( dp[t][j][0], dp[t][j-k-1][1]+dp[e.to][k][0] );
51                 }
52             }
53         }
54     }
55 }
56 
57 
58 
59 int main()
60 {
61     //freopen("input.txt", "r", stdin);
62     int n, K, a, b;
63     while(~scanf("%d%d",&n,&K))
64     {
65         edge_cnt=0;
66         memset(head, -1, sizeof(head));
67 
68         for(int i=1; i<=n; i++) scanf("%d",&w[i]);
69         for(int i=1; i<n; i++)
70         {
71             scanf("%d%d",&a,&b);
72             add_node(a,b);
73             add_node(b,a);
74         }
75         DFS(1, -1, K);
76         cout<<dp[1][K][0]<<endl;
77     }
78     return 0;
79 }
AC代码
```

