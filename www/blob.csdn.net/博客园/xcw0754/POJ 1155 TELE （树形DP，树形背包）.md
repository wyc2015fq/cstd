# POJ 1155 TELE （树形DP，树形背包） - xcw0754 - 博客园
# [POJ 1155 TELE （树形DP，树形背包）](https://www.cnblogs.com/xcw0754/p/4817381.html)
题意：给定一棵树，n个节点，其中有m个叶子表示的是用户，其他点表示中转器， 每条边都有权值，每个用户i愿意给的钱w[i]，问如果在不亏钱的情况下能为多少用户转播足球比赛？
思路：
　　其实就是要选出部分叶子节点，其花费=所选叶子权值 - 经过的所有边权（每条边只算1次花费）。
　　那么对于每个节点，可以考虑在其子树下选择1~k个叶子节点，记录下dp值（是个最优值）。那么就需要枚举所有可能了。复杂度貌似在极端情况下还是挺高的，比如单链1000个节点+2000个叶子节点的情况，不会算复杂度，每个非叶子节点中有两个for循环，全按上限来算接近O(n3)。
```
1 //#include <bits/stdc++.h>
 2 #include <cstdio>
 3 #include <cstring>
 4 #include <iostream>
 5 #define pii pair<int,int>
 6 #define INF 0x3f3f3f3f
 7 #define LL long long
 8 using namespace std;
 9 const int N=3010;
10 
11 struct node
12 {
13     int from,to,val,next;
14     node(){};
15     node(int from,int to,int val,int next):from(from),to(to),val(val),next(next){};
16 }edge[N*2];
17 int head[N], n, m, edge_cnt;
18 void add_node(int from,int to,int val)
19 {
20     edge[edge_cnt]=node(from,to,val,head[from]);
21     head[from]=edge_cnt++;
22 }
23 
24 int dp[N][N], mon[N];
25 int DFS(int t,int cost)
26 {
27     node e;
28     int sum=dp[t][0]=0;
29     for(int i=head[t]; i!=-1; i=e.next)
30     {
31         e=edge[i];
32         int tmp=DFS(e.to, cost+e.val);
33         sum+=tmp;   //统计叶子数量
34 
35         for(int j=sum; j>0; j--)    //必须降序，防止重复。
36             for(int k=1; k<=tmp&&k<=j; k++)   //在此子树中挑k个叶子节点。升序/降序皆可，但需稍修改。
37                 dp[t][j]=max(dp[t][j], dp[t][j-k]+dp[e.to][k]-e.val ); //dp值表示花费
38     }
39     if(sum==0)
40     {
41         dp[t][1]=mon[t];
42         sum=1;
43     }
44     return sum;
45 }
46 
47 int main()
48 {
49     //freopen("input.txt", "r", stdin);
50     int a,b;
51     while(~scanf("%d%d",&n,&m))
52     {
53         memset(head, -1, sizeof(head));
54         for(int i=0; i<=n; i++)
55             for(int j=0; j<=n; j++)
56                 dp[i][j]=-INF;
57 
58         for(int i=1,y; i<=n-m; i++) //i点的后继和边权
59         {
60             scanf("%d",&y);
61             while(y--)
62             {
63                 scanf("%d%d",&a,&b);
64                 add_node(i,a,b);
65             }
66         }
67         for(int i=n-m+1; i<=n; i++) //用户肯交的钱
68             scanf("%d",&mon[i]);
69 
70         DFS(1,0);
71         for(int i=m; i>=0; i--)
72             if(dp[1][i]>=0) {printf("%d\n",i); break;}
73     }
74     return 0;
75 }
76 
77 AC代码
AC代码
```

