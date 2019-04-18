# HDU 4044 GeoDefense （树形DP，混合经典） - xcw0754 - 博客园
# [HDU 4044 GeoDefense （树形DP，混合经典）](https://www.cnblogs.com/xcw0754/p/4826283.html)
题意：
　　给一棵n个节点的树，点1为敌方基地，叶子结点都为我方阵地。我们可以在每个结点安放炸弹，每点至多放一个，每个结点有ki种炸弹可选，且每种炸弹有一个花费和一个攻击力（1点攻击力使敌人掉1点hp）。敌人的目的是我方阵地，任意路线都有可能，但规定只能往下跑。问当有m钱时，最坏情况下最多能打掉敌人多少hp？(n<1001, m<201, ki>=0)
思路：
　　我竟然自己写出来了，咔咔~证明此题不难！
　　注意：某些点可能没有炸弹可选（即不能放炸弹）；有0花费的炸弹；最坏情况下指的是敌人总是跑攻击力最小的那条路径。
　　本题说不出具体什么模型，混着来的。观察一下知道，如果本节点不放炸弹，那么所有子树就都得放炸弹，否则本节点往下走的攻击力必定为0。那么尽量炸弹放在更靠近根的地方应该是比较划算的（按常理）。
　　同样，分析一个节点。本节点为根的子树的攻击力=min（每个子树的攻击力）+（本节点的攻击力），这是短板原理。然后再考虑本节点，如果买了，直接在短板上加攻击力，前提是本节点只允许放1炸弹。
　　如何转移？先从将这子树花费为0的装进背包，如果有更好的再代替掉他。这一步暂时将dp[t][j]表示为以t为根的子树如果得到j元，至少能攻击敌人的hp。状态方程：dp[t][j]=max(dp[t][j],  min(dp[t][j-k], dp[e.to][k]));。为什么有个min?因为要取短板。注意初始化问题。
　　由于可能会有免费炸弹，而每个点只需要考虑1个免费炸弹（只能放1个），其他免费炸弹删除掉（因为不划算啊），而且免费炸弹总是要先被考虑，否则的话就会重复。
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
52     //本节点的决策是01背包模型： 买 or 不买。(不买就只能靠孩子来防御)
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

