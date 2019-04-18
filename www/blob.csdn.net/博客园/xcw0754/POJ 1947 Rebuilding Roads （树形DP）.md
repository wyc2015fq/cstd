# POJ 1947 Rebuilding Roads （树形DP） - xcw0754 - 博客园
# [POJ 1947 Rebuilding Roads （树形DP）](https://www.cnblogs.com/xcw0754/p/4818470.html)
题意：给一棵树，在树中删除一些边，使得有一个连通块刚好为p个节点，问最少需要删除多少条边？
思路：
　　因为任一条边都可能需要被删除，独立出来的具有p个节点的连通块可能在任意一处地方。先从根开始DFS，然后进行树DP，dp[t][i]表示在以t为根的子树中删除i个点需要删除多少条边。dp[t][n-p]有可能是答案了，但是这种仅考虑到从树上脱落掉部分子树，那么留下的连通块通常是与1号点（树根）相连的，那如果所需要的连通块是在某棵子树中呢？将所有可能的子树取出来，若该子树节点数>=p，那么就可以在该子树中再删除一些边，来取得最优解。
　　注：若p=n，那么ans=0；若有某棵子树的节点数等于p，那么ans=1。
```
1 //#include <bits/stdc++.h>
 2 #include <cstdio>
 3 #include <cstring>
 4 #include <iostream>
 5 #define pii pair<int,int>
 6 #define INF 0x3f3f3f3f
 7 #define LL long long
 8 using namespace std;
 9 const int N=200;
10 
11 struct node
12 {
13     int from,to,next;
14     node(){};
15     node(int from,int to,int next):from(from),to(to),next(next){};
16 }edge[N*2];
17 int head[N], dp[N][N], cnt[N], n, p, edge_cnt;
18 void add_node(int from,int to)
19 {
20     edge[edge_cnt]=node(from, to, head[from]);
21     head[from]=edge_cnt++;
22 }
23 
24 int DFS(int t)
25 {
26     int sum=dp[t][0]=0;
27     node e;
28     for(int i=head[t]; i!=-1; i=e.next)
29     {
30         e=edge[i];
31         cnt[e.to]=DFS(e.to);
32         sum+=cnt[e.to];                                   //统计叶子数量
33 
34         for(int j=sum; j>0; j--)
35             for(int k=1; k<=cnt[e.to] && k<=j; k++)
36                 dp[t][j]=min(dp[t][j], dp[t][j-k]+dp[e.to][k]); //dp值表示至少需要断开多少条边
37     }
38     dp[t][sum+1]=(t==1?0:1); //断开edge(t,父亲)这条边，以t为根的子树就是sum+1个点了。
39     return sum+1;
40 }
41 
42 int main()
43 {
44     freopen("input.txt", "r", stdin);
45     int a,b;
46     while(~scanf("%d%d",&n,&p))
47     {
48         memset(head, -1, sizeof(head));
49         memset(dp, 0x3f, sizeof(dp));
50         memset(cnt, 0, sizeof(cnt));
51         edge_cnt=0;
52 
53         for(int i=1; i<n; i++)
54         {
55             scanf("%d%d",&a,&b);
56             add_node(a, b);
57         }
58         cnt[1]=DFS(1); //根一定是1
59 
60         int ans=INF;
61         for(int i=1; i<=n; i++)
62         {
63             if(cnt[i]-p>=0)  //子树i去掉cnt[i]-p个点后与i相连的连通块。
64                 ans=min(ans, dp[i][cnt[i]-p]+1);
65             ans=min(ans, dp[i][n-p]);   //在此子树中
66         }
67         printf("%d\n", ans);
68     }
69 
70 /*
71     （1）计算从每棵子树断开k个节点的最少花费。
72     （2）断开某一子树与父亲的边，再从该子树中断开cnt-p条边（dp值已求），就能获得p个节点的树。
73 */
74     return 0;
75 }
AC代码
```

