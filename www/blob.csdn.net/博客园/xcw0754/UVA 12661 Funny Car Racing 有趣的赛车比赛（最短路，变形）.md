# UVA 12661 Funny Car Racing 有趣的赛车比赛（最短路，变形） - xcw0754 - 博客园
# [UVA 12661 Funny Car Racing 有趣的赛车比赛（最短路，变形）](https://www.cnblogs.com/xcw0754/p/4651874.html)
题意：赛道有n个交叉点，和m条单向路径（有重边），每条路都是周期性关闭的，且通过仍需一段时间。在比赛开始时，所有道路刚好打开，选择进入该道路必须满足“在打开的时间段进入，在关闭之前出来”，即不可在路上逗留，但是可以在交叉点逗留。问到达终点的时间要多少？
思路：最短路，而且正权，用Dijkstra+优先队列够了。主要的难点在计算是否可以进入该路段，画图清晰点。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 #define pii pair<int,int>
 4 #define INF 0x7f7f7f7f
 5 using namespace std;
 6 const int N=50000+100;
 7 vector<int> vect[320];
 8 
 9 struct node
10 {
11     int from;
12     int to;
13     int a;
14     int b;
15     int len;
16 
17 }edge[N];
18 int edge_cnt;
19 
20 void add_node(int u,int v,int a,int b,int t)
21 {
22     edge[edge_cnt].from=u;
23     edge[edge_cnt].to=v;
24     edge[edge_cnt].a=a;
25     edge[edge_cnt].b=b;
26     edge[edge_cnt].len=t;
27     vect[u].push_back(edge_cnt++);
28 }
29 
30 int dis[320];
31 bool vis[320];
32 
33 int Dijkstra(int s,int e)
34 {
35     memset(vis,0,sizeof(vis));
36     memset(dis,0x7f,sizeof(dis));
37     priority_queue<pii, vector<pii>, greater<pii> > que;
38     que.push(make_pair(0,s));
39     dis[s]=0;
40 
41     while(!que.empty())     //每次用一个点来更新别人
42     {
43         int x=que.top().second; que.pop();
44         if(vis[x])  continue;   //遍历过
45         vis[x]=1;
46         for(int i=0; i<vect[x].size(); i++)
47         {
48             node e=edge[vect[x][i]];
49             if( dis[x]%(e.a+e.b)+e.len<=e.a
50                 && dis[e.to]>dis[x]+e.len )  //在可通过时间段
51             {
52                 dis[e.to]=dis[x]+e.len;
53                 que.push(make_pair(dis[e.to],e.to));
54             }
55             else if( dis[e.to]>dis[x]+e.len+ (e.a+e.b-dis[x]%(e.a+e.b))  )    //要等待
56             {
57                 dis[e.to]=dis[x]+e.len+ (e.a+e.b-dis[x]%(e.a+e.b)) ;
58                 que.push(make_pair(dis[e.to],e.to));
59             }
60         }
61     }
62     return dis[e];
63 }
64 
65 
66 
67 int main()
68 {
69 
70     freopen("input.txt", "r", stdin);
71 
72     int n, m, s, t, u, v, a, b, tt, j=0;
73     while(~scanf("%d%d%d%d",&n,&m,&s,&t))
74     {
75         for(int i=0; i<=n; i++) vect[i].clear();
76         memset(edge,0,sizeof(edge));
77         edge_cnt=0;
78 
79         for(int i=0; i<m; i++)
80         {
81             scanf("%d %d %d %d %d", &u, &v, &a, &b, &tt );
82             if(a>=tt)   add_node(u, v, a, b, tt);//去掉废路
83         }
84         printf("Case %d: %d\n", ++j, Dijkstra(s,t));
85     }
86     return 0;
87 }
AC代码
```

