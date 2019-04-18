# UVA 1349 Optimal Bus Route Design 最优公交路线（最小费用流，拆点） - xcw0754 - 博客园
# [UVA 1349 Optimal Bus Route Design 最优公交路线（最小费用流，拆点）](https://www.cnblogs.com/xcw0754/p/4646524.html)
题意：
　　给若干景点，每个景点有若干单向边到达其他景点，要求规划一下公交路线，使得每个景点有车可达，并且每个景点只能有1车经过1次，公车必须走环形回到出发点（出发点走2次）。问是否存在这样的线路？若存在就给出所有公交车需要走过的路的长度，要求长度尽量小。
分析：
　　这超级难发现是网络流来做的。要将每个点归结到某个环上，那么环上的点都是只有1个前驱，1个后继。如果1个前驱配1个后继，就是匹配问题了。但是这样的匹配有点混杂，所以要拆点，将1个点拆成2个，分别处于X和Y集中，然后根据有向边建图。成了带权二分图的匹配了，只是要求权最小。
　　建图步骤，对于每条有向边a->b，由于b放在Y集中，编号就改变为2*b+1，而a在左边，改变为a*2，容量是1，因为只能匹配一次，费用为长度。当然肯定需要反边了！接着添加汇点，由Y集到汇点都有边，再添加源点，源点到X集都有边，他们容量都是1费用0。
```
1 #include <bits/stdc++.h>
  2 #define LL long long
  3 #define pii pair<int,int>
  4 #define INF 0x7f7f7f7f
  5 using namespace std;
  6 const int N=200+50;
  7 
  8 struct node
  9 {
 10     int from;
 11     int to;
 12     int val;
 13     int cap;
 14     int flow;
 15 }edge[N*N];
 16 int edge_cnt, ans_cost;
 17 int flow[N], cost[N], path[N], inq[N];
 18 
 19 vector<int> vect[N];
 20 
 21 void add_node(int from,int to,int val,int cap,int flow)
 22 {
 23     edge[edge_cnt].from=from;
 24     edge[edge_cnt].to=to;
 25     edge[edge_cnt].val=val;
 26     edge[edge_cnt].cap=cap;
 27     edge[edge_cnt].flow=flow;
 28     vect[from].push_back(edge_cnt++);
 29 }
 30 
 31 
 32 int spfa(int s,int e)
 33 {
 34     deque<int> que(1,s);
 35     inq[s]=1;
 36     flow[s]=INF;
 37     cost[s]=0;
 38     while(!que.empty())
 39     {
 40         int x=que.front();que.pop_front();
 41         inq[x]=0;
 42         for(int i=0; i<vect[x].size(); i++)
 43         {
 44             node e=edge[vect[x][i]];
 45             if( e.cap>e.flow && cost[e.to]>cost[e.from]+e.val )
 46             {
 47                 flow[e.to]=min(flow[e.from], e.cap-e.flow);
 48                 cost[e.to]=cost[e.from]+e.val;
 49                 path[e.to]=vect[x][i];
 50                 if(!inq[e.to])
 51                 {
 52                     inq[e.to]=1;
 53                     que.push_back(e.to);
 54                 }
 55             }
 56         }
 57     }
 58     return flow[e];
 59 }
 60 
 61 
 62 int cal(int s,int e)
 63 {
 64     int ans_flow=0;
 65     while(true)
 66     {
 67         memset(flow,0,sizeof(flow));
 68         memset(path,0,sizeof(path));
 69         memset(cost,0x7f,sizeof(cost));
 70         memset(inq,0,sizeof(inq));
 71 
 72         int tmp=spfa(s,e);
 73         if(!tmp)    return ans_flow;
 74         ans_flow+=tmp;
 75         ans_cost+=cost[e];//长度
 76 
 77         int ed=e;
 78         while(ed!=s)
 79         {
 80             int t=path[ed];
 81             edge[t].flow+=flow[e];
 82             edge[t^1].flow-=flow[e];
 83             ed=edge[t].from;
 84         }
 85     }
 86 }
 87 
 88 
 89 int main()
 90 {
 91     freopen("input.txt", "r", stdin);
 92     int n, b, v;
 93     while(scanf("%d",&n), n)
 94     {
 95         ans_cost=edge_cnt=0;
 96         for(int i=N-1; i>=0; i--)   vect[i].clear();
 97         memset(edge,0,sizeof(edge));
 98 
 99         for(int i=1; i<=n; i++)
100         {
101             while(scanf("%d",&b),b)
102             {
103                 scanf("%d",&v);
104                 add_node( i*2, b*2+1, v, 1, 0 );    //编号从2~n*2+1。
105                 add_node( b*2+1, i*2, -v, 0, 0 );
106             }
107         }
108         //添加汇点n*2+2，源点0
109         for(int i=1; i<=n; i++)
110         {
111             add_node(0, i*2, 0, 1, 0);
112             add_node(i*2, 0, 0, 0, 0);
113         }
114 
115         for(int i=1; i<=n; i++)
116         {
117             add_node(i*2+1, n*2+2, 0, 1, 0);
118             add_node(n*2+2, i*2+1, 0, 0, 0);
119         }
120 
121         if(cal(0, n*2+2)!=n)    puts("N");
122         else    printf("%d\n",ans_cost);
123 
124     }
125     return 0;
126 }
AC代码
```

