# UVA 820 Internet Bandwidth 因特网宽带（无向图，最大流，常规） - xcw0754 - 博客园
# [UVA 820 Internet Bandwidth 因特网宽带（无向图，最大流，常规）](https://www.cnblogs.com/xcw0754/p/4661167.html)
题意：给一个无向图，每条边上都有容量的限制，要求求出给定起点和终点的最大流。
思路：每条无向边就得拆成2条，每条还得有反向边，所以共4条。源点汇点已经给出，所以不用建了。直接在图上跑最大流就可以了。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 #define pii pair<int,int>
 4 #define INF 0x7f7f7f7f
 5 using namespace std;
 6 const int N=200;
 7 const int mod=1e9+7;
 8 int s, t;
 9 
10 int path[N], flow[N];
11 vector<int> vect[N];
12 
13 struct node
14 {
15     int from, to, cap, flow;
16     node(){};
17     node(int from,int to,int cap,int flow):from(from),to(to),cap(cap),flow(flow){};
18 }edge[100000];
19 int edge_cnt;
20 
21 void add_node(int from,int to,int cap,int flow)
22 {
23     edge[edge_cnt]=node(from,to,cap,flow);
24     vect[from].push_back(edge_cnt++);
25 }
26 
27 int BFS(int s,int e)
28 {
29     deque<int> que(1,s);
30     flow[s]=INF;
31     while(!que.empty())
32     {
33         int x=que.front();
34         que.pop_front();
35         for(int i=0; i<vect[x].size(); i++)
36         {
37             node e=edge[vect[x][i]];
38             if(!flow[e.to] && e.cap>e.flow)
39             {
40                 flow[e.to]=min(flow[e.from],e.cap-e.flow);
41                 path[e.to]=vect[x][i];
42                 que.push_back(e.to);
43             }
44         }
45         if(flow[e]) return flow[e];
46     }
47     return flow[e];
48 }
49 
50 int max_flow(int s,int e)
51 {
52     int ans_flow=0;
53     while(true)
54     {
55         memset(path,0,sizeof(path));
56         memset(flow,0,sizeof(flow));
57 
58         int tmp=BFS(s,e);
59         if(!tmp)    return ans_flow;
60         ans_flow+=tmp;
61 
62         int ed=e;
63         while(ed!=s)
64         {
65             int t=path[ed];
66             edge[t].flow+=tmp;
67             edge[t^1].flow-=tmp;
68             ed=edge[t].from;
69         }
70     }
71 }
72 int main()
73 {
74     freopen("input.txt", "r", stdin);
75     int n, a, b, v, c, j=0;
76     while(scanf("%d",&n),n)
77     {
78         edge_cnt=0;
79         memset(edge,0,sizeof(edge));
80         for(int i=0; i<=n+1; i++)   vect[i].clear();
81 
82         scanf("%d%d%d", &s, &t, &c);
83         for(int i=0; i<c; i++)
84         {
85             scanf("%d%d%d",&a,&b,&v);
86             add_node(a, b, v, 0);
87             add_node(b, a, 0, 0);
88             add_node(b, a, v, 0);
89             add_node(a, b, 0, 0);
90         }
91         printf("Network %d\n",++j);
92         printf("The bandwidth is %d.\n\n", max_flow(s ,t) );
93     }
94     return 0;
95 }
AC代码
```

