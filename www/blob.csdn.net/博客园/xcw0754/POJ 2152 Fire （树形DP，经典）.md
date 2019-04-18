# POJ 2152 Fire （树形DP，经典） - xcw0754 - 博客园
# [POJ 2152 Fire （树形DP，经典）](https://www.cnblogs.com/xcw0754/p/4813160.html)
题意：给定一棵n个节点的树，要在某些点上建设消防站，使得所有点都能够通过某个消防站解决消防问题，但是每个点的建站费用不同，能够保证该点安全的消防站的距离上限也不同。给定每个点的建站费用以及最远的消防站距离上限，求保证该树安全的最小花费。
思路：
　　要选择部分点来建站解决消防问题，而总花费是有最优解的。
　　如何进行树形DP？
　　假设某点t的所有子树的消防问题都解决，而且已经获得最优解了，那么现在考虑的是点t的最优解问题，点t可以依靠任何点只要不超过距离限制即可，那枚举一下所有点试试，一旦t依靠某个点j解决消防问题，那么t的孩子/孙子也可能可以依靠j来解决消防问题。这只需要判断一下每个孩子是否能够依靠j就行了，而孙子如果依靠j的话，如何知道？此时，之前已经假设t的孩子的消防问题已经解决，那么t的孩子在枚举依靠站时也肯定枚举过j了，或者能依靠，或者不能依靠。而t的孙子能不能依靠j，是交给t的孩子去解决的，就像t的孩子是交给t来负责一样。这是符合递归性质的，那么当前我们就可以只考虑点t和他的孩子们能不能愉快玩耍就行了。
　　但是还有个最优性质，那么在枚举j的基础上，dp[t][j]=w[j]是至少的，然后再加上所有子树的最优花费，就是dp[t][j]了。上面是领会思路的。下面看别人的状态方程。
**复杂度为O(n^2)的树形DP.因为要依赖其他站点，所以不仅仅只从子树中获取信息，也可能从父亲结点，兄弟结点获取信息，所以在计算每个点时首先想到要枚举，因为n特别小，允许我们枚举。设dp[i][j]表示i点及其子树都符合情况下i点依赖j点的最小花费，有了这个似乎还不够，再开个一维数组best，best[i]表示以i为根的子树符合题目要求的最小花费。这样状态转移方程就是dp[i][j] = cost[j] + sum(min(dp[k][j]-cost[j],best[k])) (k为i的子节点，j为我们枚举的n个点)，因为i的每个子节点可以和i一样依赖j结点，那么花费是dp[k][j]-cost[j]，或者依赖以k为根的树中的某点，花费是best[k]，最后再加上cost[j]，因为要在j结点建站所以要增加花费。**
　　为什么是O(n2)？递归计算每个节点时的复杂度为O(孩子数*n)，而每个节点作为孩子节点来计算的话仅有1次。即总复杂度最高为O(n2)。
```
1 //#include <bits/stdc++.h>
 2 #include <vector>
 3 #include <iostream>
 4 #include <cstdio>
 5 #include <cstring>
 6 #define pii pair<int,int>
 7 #define INF 0x3f3f3f3f
 8 #define LL long long
 9 using namespace std;
10 const int N=1010;
11 
12 struct node
13 {
14     int from,to,len;
15     node(){};
16     node(int from,int to,int len):from(from),to(to),len(len){};
17 }edge[N*2];
18 int edge_cnt, w[N], d[N], n;
19 vector<int> tree[N];
20 void add_node(int from,int to,int len)
21 {
22     edge[edge_cnt]=node(from,to,len);
23     tree[from].push_back(edge_cnt++);
24 }
25 
26 int dist[N][N], root;    //到根的距离
27 void get_dis(int t,int far,int len)
28 {
29     dist[root][t]=dist[t][root]=len;
30     if(len>d[root]) return ;
31     for(int i=0; i<tree[t].size(); i++)
32     {
33         node &e=edge[tree[t][i]];
34         if(e.to!=far)    get_dis(e.to,t,len+e.len);
35     }
36 }
37 
38 
39 int dp[N][N], best[N];
40 void DFS(int t,int far)
41 {
42     for(int i=0; i<tree[t].size(); i++) //递归先解决子问题
43     {
44         node &e=edge[tree[t][i]];
45         if(e.to!=far)    DFS(e.to,t);
46     }
47 
48     best[t]=INF;
49     for(int j=1,sum=0; j<=n; j++,sum=0)     //尝试将i依靠j解决消防问题
50     {
51         if( dist[t][j]>d[t] )   continue;   //不能依靠到j，太远了
52         for(int i=0; i<tree[t].size(); i++) //每个子节点取最优解
53         {
54             node &e=edge[tree[t][i]];
55             if(e.to!=far)    sum+=min(best[e.to], dp[e.to][j]-w[j]);
56         }
57         dp[t][j]=w[j]+sum;
58         best[t]=min(best[t], dp[t][j]);
59     }
60 }
61 
62 void init()
63 {
64     edge_cnt=0;
65     for(int i=1; i<=n; i++) tree[i].clear();
66     memset(dp, 0x3f, sizeof(dp));
67     memset(dist, 0x3f, sizeof(dist));
68 }
69 
70 int main()
71 {
72     //freopen("input.txt", "r", stdin);
73     int t,a,b,c;
74     cin>>t;
75     while(t--)
76     {
77         scanf("%d",&n);
78         init();
79         for(int i=1; i<=n; i++) scanf("%d",&w[i]);
80         for(int i=1; i<=n; i++) scanf("%d",&d[i]);  //距离i城市最远的消防站距离上限
81         for(int i=1; i<n; i++)
82         {
83             scanf("%d%d%d",&a,&b,&c);
84             add_node(a,b,c);
85             add_node(b,a,c);
86         }
87         for(int i=1; i<=n; i++)    get_dis(root=i, -1, 0);  //计算任意点对之间的距离
88         DFS(1, -1);
89         printf("%d\n",best[1]);
90 
91     }
92     return 0;
93 }
AC代码
```

