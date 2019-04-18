# HDU 5294 Tricks Device （最短路，最大流） - xcw0754 - 博客园
# [HDU 5294 Tricks Device （最短路，最大流）](https://www.cnblogs.com/xcw0754/p/4666357.html)
题意：给一个无向图（连通的），张在第n个点，吴在第1个点，‘吴’只能通过最短路才能到达‘张’，两个问题：（1）张最少毁掉多少条边后，吴不可到达张（2）吴在张毁掉最多多少条边后仍能到达张。
思路：注意是最短路才可达，但是最短路径可能有多条（即权值相等的）！！
　　第二个问题好回答，来次最短路，记录下到达每个点在最低权值的情况下的最少次用边。
　　第一个问题，同样只要砍掉最短路的某些边即可。要根据第2个问题所跑的SSSP，将不是最短路的边的剔除，重新建图，跑最大流，得到结果。
　　当然要考虑重边！
```
1 #include <bits/stdc++.h>
  2 #define LL long long
  3 #define pii pair<int,int>
  4 #define INF 0x7f7f7f7f
  5 using namespace std;
  6 const int N=62000;
  7 vector<int> vect[10000], vect2[10000];
  8 int g[2100][2100];
  9 struct node
 10 {
 11     int from,to,cap,flow;
 12     node(){};
 13     node(int from,int to,int cap,int flow):from(from),to(to),cap(cap),flow(flow){};
 14 }edge[N*4];
 15 struct node2//跑最短路用的
 16 {
 17     int from,to,cost;
 18     node2(){};
 19     node2(int from,int to,int cost):from(from),to(to),cost(cost){};
 20 }oldedge[N*2];
 21 
 22 
 23 int edge_cnt,edge_cnt2, n, m;
 24 void add_node(int from,int to,int cap,int flow)
 25 {
 26     edge[edge_cnt]=node(from, to, cap, flow);
 27     vect[from].push_back(edge_cnt++);
 28 }
 29 void add_node2(int from,int to,int cost)//跑最短路用的
 30 {
 31     oldedge[edge_cnt2]=node2(from, to, cost);
 32     vect2[from].push_back(edge_cnt2++);
 33 }
 34 
 35 
 36 
 37 int flow[10000], path[10000];
 38 int cost[10000], inq[10000], times[10000];
 39 
 40 int BFS(int s,int e)
 41 {
 42     deque<int> que(1,s);
 43     flow[s]=INF;
 44     while(!que.empty())
 45     {
 46         int x=que.front();
 47         que.pop_front();
 48         for(int i=0; i<vect[x].size(); i++)
 49         {
 50             node e=edge[vect[x][i]];
 51             if(!flow[e.to] && e.cap>e.flow)
 52             {
 53                 flow[e.to]=min(flow[e.from], e.cap-e.flow);
 54                 path[e.to]=vect[x][i];
 55                 que.push_back(e.to);
 56             }
 57         }
 58         if(flow[e]) return flow[e];
 59     }
 60     return flow[e];
 61 }
 62 int max_flow(int s,int e)
 63 {
 64     int ans_flow=0;
 65     while(true)
 66     {
 67         memset(path,0,sizeof(path));
 68         memset(flow,0,sizeof(flow));
 69 
 70         int tmp=BFS(s,e);
 71         if(!tmp)    return ans_flow;
 72         ans_flow+=tmp;
 73 
 74         int ed=e;
 75         while(ed!=s)
 76         {
 77             int t=path[ed];
 78             edge[t].flow+=tmp;
 79             edge[t^1].flow-=tmp;
 80             ed=edge[t].from;
 81         }
 82     }
 83 }
 84 
 85 int spfa(int s,int e)
 86 {
 87     memset(cost,0x7f,sizeof(cost));
 88     memset(inq,0,sizeof(inq));
 89     memset(times,0x7f,sizeof(times));//记录到达每个点的最少用边，前提是权最少
 90 
 91     deque<int> que(1,s);
 92     cost[s]=0;
 93     times[s]=0;
 94     while(!que.empty())
 95     {
 96         int x=que.front();que.pop_front();
 97         inq[x]=0;
 98         for(int i=0; i<vect2[x].size(); i++)
 99         {
100             node2 e=oldedge[vect2[x][i]];
101             if(cost[e.to]>=cost[e.from]+e.cost)
102             {
103                 if( cost[e.to]>cost[e.from]+e.cost) times[e.to]=times[e.from]+1;
104                 else   times[e.to]=min(times[e.to],  times[e.from]+1);
105 
106                 cost[e.to]=cost[e.from]+e.cost;
107                 if(!inq[e.to])
108                 {
109                     inq[e.to]=1;
110                     que.push_back(e.to);
111                 }
112             }
113         }
114     }
115     return times[e];
116 }
117 
118 void build_graph()
119 {
120     for(int i=1; i<=n; i++)
121     {
122         for(int j=0; j<vect2[i].size(); j++)
123         {
124             node2 e=oldedge[vect2[i][j]];
125             if(cost[e.to]==cost[e.from]+e.cost)
126             {
127                 add_node(e.from,e.to,1,0);
128                 add_node(e.to,e.from,0,0);
129             }
130         }
131     }
132 }
133 
134 
135 
136 int main()
137 {
138     freopen("input.txt", "r", stdin);
139     int a, b, c;
140     while(~scanf("%d%d", &n, &m))
141     {
142         edge_cnt=0;
143         edge_cnt2=0;
144         memset(edge,0,sizeof(edge));
145         memset(oldedge,0,sizeof(oldedge));
146         for(int i=n; i>=0; i--)   vect[i].clear(),vect2[i].clear();
147 
148         for(int i=0; i<m; i++)
149         {
150             scanf("%d %d %d", &a, &b, &c);
151             add_node2(a,b,c);
152             add_node2(b,a,c);
153         }
154         int ans2=spfa(1,n);
155         build_graph();//重新建图
156         int ans1=max_flow(1,n);
157         printf("%d %d\n", ans1, m-ans2);
158     }
159     return 0;
160 }
AC代码
```

