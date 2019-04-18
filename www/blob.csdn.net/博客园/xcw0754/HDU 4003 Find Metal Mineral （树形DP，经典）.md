# HDU 4003 Find Metal Mineral （树形DP，经典） - xcw0754 - 博客园
# [HDU 4003 Find Metal Mineral （树形DP，经典）](https://www.cnblogs.com/xcw0754/p/4820463.html)
题意：给定一棵树图，n个节点，有边权，要派k<11个机器人从节点s出发，遍历所有的点，每当1只机器人经过1条边时就会花费该边的边权，边是可重复走的。问遍历完所有点的最小花费？
思路：
　　非常经典，首先需要知道一点，才能往下推理。就是“如果在t点派c个机器人往孩子u，那么最多只有1个机器人能走会回来到t，否则花费总是不划算的”。
　　稍微证明一下：
　　（1）假设派1个机器人往u，逛一圈回到u的父亲t，花费v= 子树u的边权和*2 + e(t,u)*2。若机器人不要了，那花费肯定比v还要少。
　　（2）假设派2个机器人往u，若要2个机器人都回来，显然花费要比（1）要大。若仅要1个机器人回来，花费仍比“仅派1只机器人往u”要高。（可以试试画一棵有3层的树，头两层只有1个节点，第三层节点数随意即可，来模拟一下就知道结果了）
　　得到的结论是，要么只派c个机器人去孩子u，且1个都不要走回来(注：0<c<=k)；要么派1个机器人出去，且遍历完子树u走回t。
　　这样已经很好解决了。枚举下点t的孩子数（组），再枚举点t可能获得的机器人数（容量），再枚举给这个孩子派的机器人数（物品）。
　　还有个初始化DP值的问题，因为每个孩子都是必须遍历的，而常规的分组背包是可选（至多选1件）或者不选物品的，那么可以先将“派1个机器人且回收1个机器人的DP值”丢进背包（保证了此容量下肯定有遍历过子树u），然后若有更好的状态再更新掉此容量。
 　　用DP[t][0]表示“有1个机器人到达t遍历完子树再走回t”的花费。DP[t][i]表示“有i个机器人到达t且遍历完整棵子树t，且不要求回到t”的最小花费。
```
1 #include <bits/stdc++.h>
 2 #include <cstdio>
 3 #include <cstring>
 4 #include <map>
 5 #include <vector>
 6 #include <iostream>
 7 #define pii pair<int,int>
 8 #define INF 0x3f3f3f3f
 9 #define LL  long long
10 using namespace std;
11 const int N=1010;
12 struct tower
13 {
14     int pri,pow;
15 }tow[N][60];
16 
17 struct node
18 {
19     int from,to,next;
20     node(){};
21     node(int from,int to,int next):from(from),to(to),next(next){};
22 }edge[N*2];
23 
24 int edge_cnt, head[N], cnt[N], dp[N][210], n, m ;
25 void add_node(int from,int to)
26 {
27     edge[edge_cnt]=node(from, to, head[from]);
28     head[from]=edge_cnt++;
29 }
30 inline int cmp(tower a,tower b){return a.pri<b.pri;}
31 
32 void DFS(int t,int far)
33 {
34     node e;
35     int flag=0;
36     for(int i=head[t]; i!=-1; i=e.next)
37     {
38         e=edge[i];
39         if(e.to==far)  continue;
40         DFS(e.to, t);
41 
42         for(int j=m; j>=0; j-- )  //容量
43         {
44             dp[t][j]=min(dp[t][j], dp[e.to][0]);  //先装代价为0的，如果有更好的再代替掉。
45             for(int k=1; k<=j; k++ ) //给此孩子k元,得到的最大攻击力。
46                 dp[t][j]=max(dp[t][j], min(dp[t][j-k], dp[e.to][k]));
47         }
48         flag=1; //标记是否叶子。
49     }
50     if(flag==0)    memset(dp[t], 0, sizeof(dp[t]));     //叶子节点
51 
52     //本节点的决策是“仅有1组的分组背包”模型： 必须且最多买1个。(不买就只能靠孩子来防御)
53     for(int j=m; j>=0; j-- )  //容量
54     {
55         int k=1;
56         for( ; k<cnt[t] && !tow[t][k].pri; k++ )  //不用钱的,只留1个power最大的.
57             if(tow[t][k-1].pow > tow[t][k].pow)
58                 swap(tow[t][k],tow[t][k-1]);
59 
60         for( k-- ; k<cnt[t]; k++)    //物品
61         {
62             if(j-tow[t][k].pri>=0)
63                 dp[t][j]=max(dp[t][j], dp[t][j-tow[t][k].pri] + tow[t][k].pow );    //可以直接挡
64         }
65     }
66 }
67 
68 int main()
69 {
70     freopen("input.txt", "r", stdin);
71     int t, a, b; cin>>t;
72     while(t--)
73     {
74         edge_cnt=0;
75         memset(head, -1, sizeof(head));
76         memset(dp, 0x3f, sizeof(dp));   //注意初始化
77 
78         scanf("%d",&n);
79         for(int i=1; i<n; i++)  //无向图
80         {
81             scanf("%d%d",&a,&b);
82             add_node(a,b);
83             add_node(b,a);
84         }
85 
86         scanf("%d",&m);     //钱。
87         for(int i=1; i<=n; i++)
88         {
89             scanf("%d",&cnt[i]);
90             for(int j=0; j<cnt[i]; j++)
91                 scanf("%d%d", &tow[i][j].pri, &tow[i][j].pow);
92             sort(tow[i], tow[i]+cnt[i], cmp);   //免费的排前面
93         }
94         DFS(1, -1);
95         printf("%d\n",dp[1][m]);
96     }
97     return 0;
98 }
AC代码
```

