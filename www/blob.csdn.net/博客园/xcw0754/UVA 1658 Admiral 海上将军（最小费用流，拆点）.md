# UVA 1658 Admiral 海上将军（最小费用流，拆点） - xcw0754 - 博客园
# [UVA 1658 Admiral 海上将军（最小费用流，拆点）](https://www.cnblogs.com/xcw0754/p/4646748.html)
题意：
　　一个有v个点的有向图，要从点1到点v需要找两条路径，两路径不可经过同一个点（除了1和v点）。求这两条路径的最小费用（保证有解）。
分析：
　　难在建图，其他套模板。
　　此图给的是超级复杂图，两个点之间有多条有向边，方向还可能是相反的。用网络流来做不能仅靠2点流来保证，因为当只有3个点，4条边都是1->2->3这样的，是不是刚好2条路径？也满足了2点流的限制。其实不能这样，要保证经过每个点1点流量，还得拆点，将2~v-1这些点都拆两个点，两点之间有条费用为0容量为1的边，这样就能保证了。然后源点1出发，汇点v结束，都是流量为2就行了。当然源点，汇点需要自己另外建。
```
1 #include <bits/stdc++.h>
  2 #define LL long long
  3 #define pii pair<int,int>
  4 #define INF 0x7f7f7f7f
  5 using namespace std;
  6 const int N=2000+50;
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
 36     cost[s]=0;
 37     flow[s]=INF;
 38 
 39     while(!que.empty())
 40     {
 41         int x=que.front();
 42         que.pop_front();
 43         inq[x]=0;
 44         for(int i=0; i<vect[x].size(); i++)
 45         {
 46             node e=edge[vect[x][i]];
 47             if(e.cap>e.flow && cost[e.to]>cost[e.from]+e.val)
 48             {
 49                 flow[e.to]=min(flow[e.from],e.cap-e.flow);
 50                 cost[e.to]=cost[e.from]+e.val;
 51                 path[e.to]=vect[x][i];
 52                 if(!inq[e.to])
 53                 {
 54                     inq[e.to]=1;
 55                     que.push_back(e.to);
 56                 }
 57             }
 58         }
 59     }
 60     return flow[e];
 61 }
 62 
 63 
 64 int cal(int s,int e)
 65 {
 66     int ans_flow=0;
 67     while(true)
 68     {
 69         memset(flow, 0, sizeof(flow));
 70         memset(path, 0, sizeof(path));
 71         memset(cost, 0x7f, sizeof(cost));
 72         memset(inq, 0, sizeof(inq));
 73 
 74         int tmp=spfa(s, e);
 75         if(!tmp)    return ans_cost;
 76         ans_cost+=cost[e];
 77         ans_flow+=tmp;
 78 
 79         int ed=e;
 80         while(ed!=s)
 81         {
 82             int t=path[ed];
 83             edge[t].flow+=tmp;
 84             edge[t^1].flow-=tmp;
 85             ed=edge[t].from;
 86         }
 87     }
 88 }
 89 
 90 
 91 int main()
 92 {
 93     freopen("input.txt", "r", stdin);
 94     int n, m, a, b, c;
 95     while(~scanf("%d%d",&n,&m))
 96     {
 97         ans_cost=0;
 98         edge_cnt=0;
 99         for(int i=n+1; i>=0; i-- )    vect[i].clear();
100         memset(edge,0,sizeof(edge));
101         for(int i=2; i<n; i++)
102         {
103             add_node(i*2,i*2+1,0,1,0);
104             add_node(i*2+1,i*2,0,0,0);
105         }
106         for(int i=0; i<m; i++)
107         {
108             scanf("%d%d%d", &a, &b, &c);
109             add_node(a*2+1, b*2, c, 1, 0);
110             add_node(b*2, a*2+1, -c, 0, 0);
111         }
112 
113         add_node(2, 2*1+1, 0, 2, 0);  //源点。2点流量
114         add_node(2*1+1, 2, 0, 0, 0);
115 
116         add_node(n*2, n*2+1, 0, 2, 0);//汇点
117         add_node(n*2+1, n*2, 0, 0, 0);
118 
119         printf("%d\n",cal(2, n*2+1));
120     }
121     return 0;
122 }
AC代码
```

