# HDU 1533 Going Home （最小费用流） - xcw0754 - 博客园
# [HDU 1533 Going Home （最小费用流）](https://www.cnblogs.com/xcw0754/p/4644005.html)
题意：
　　一个矩阵n*m，其中有k个房子和k个人，k个人分别必须走到任意一个房子中（匹配），但是权值就是长度（非欧拉距离），求匹配完的权之和。
思路：
　　建图方法是，首先将k个人和k个房子分别抽出来到集合X和Y中，计算两两之间的距离，X到Y有一条边，费用为正，容量为1；Y到X也有一条边，费用为负，容量为0（其实两条边就是相反的）。添加一个源点0号到X集，添加一个汇点2*k+1号到Y集，这些费用都是0，容量都是1。
　　建完图就用正常的方法（EK+SPFA）来解决就行了。由于我想要让每次增广路只找到流为1的路径，所以我在2*k+1号点后面还加个点2*k+2，仅1条边，费用为0，容量为1。这样每次spfa至多也只有1流量通过到汇点了。（可不用此法，将每次spfa所得cost[end]*flow[end]就行了，因为可能流过的并不止1流量。）
```
1 #include <bits/stdc++.h>
  2 #define LL long long
  3 #define pii pair<int,int>
  4 #define INF 0x7f7f7f7f
  5 using namespace std;
  6 const int N=10000+20;
  7 vector<int> vect[N];
  8 int flow[N], cost[N], path[N], inq[N], edge_cnt;
  9 struct node
 10 {
 11     int from;
 12     int to;
 13     int val;
 14     int cap;
 15     int flo;
 16 }edge[N*4];//边数要自己估计，不然就会RE
 17 
 18 
 19 void add_node(int from,int to, int val, int cap, int flo)
 20 {
 21     edge[edge_cnt].from=from;
 22     edge[edge_cnt].to=to;
 23     edge[edge_cnt].val=val;
 24     edge[edge_cnt].cap=cap;
 25     edge[edge_cnt].flo=flo;
 26     vect[from].push_back(edge_cnt++);
 27 }
 28 
 29 int spfa(int s,int e)
 30 {
 31     deque<int> que;
 32     que.push_back(s);
 33     inq[s]=1;
 34     flow[s]=INF;
 35     cost[s]=0;
 36 
 37     while(!que.empty())
 38     {
 39         int x=que.front();
 40         que.pop_front();
 41         inq[x]=0;
 42         for(int i=0; i<vect[x].size(); i++)
 43         {
 44             node e=edge[vect[x][i]];
 45             if(e.cap>e.flo && cost[e.to]>cost[e.from] + e.val)
 46             {
 47                 cost[e.to]=cost[x]+ e.val;//每次的流最多只是1。
 48                 path[e.to]=vect[x][i];
 49                 flow[e.to]=min(flow[e.from], e.cap-e.flo);
 50                 if(!inq[e.to])
 51                 {
 52                     inq[e.to]=1;
 53                     que.push_back(e.to);
 54                 }
 55             }
 56         }
 57     }
 58     return cost[e];
 59 }
 60 
 61 int cal(int s,int e)
 62 {
 63     int ans=0;
 64     while(true)
 65     {
 66         memset(flow, 0, sizeof(flow));
 67         memset(cost, 0x7f, sizeof(cost));
 68         memset(path, 0, sizeof(path));
 69         memset(inq, 0, sizeof(inq));
 70 
 71         if(spfa(s,e)==INF) return ans;
 72         ans+=cost[e]; 
 73 
 74         int ed=e-1;//连e点那条边不清空，可以保证每次至多1流。
 75         while(ed!=s)
 76         {
 77             int t=path[ed];
 78             edge[t].flo++;
 79             edge[t^1].flo--;
 80             ed=edge[t].from;
 81         }
 82     }
 83 }
 84 
 85 int main()
 86 {
 87     freopen("input.txt", "r", stdin);
 88     int n, m;char ch;
 89     while(~scanf("%d%d",&n,&m),n+m)
 90     {
 91         for(int i=N-1; i>=0; i--)    vect[i].clear();
 92         memset(edge, 0, sizeof(edge));
 93         edge_cnt=0;
 94         int cnt=0;
 95         vector<pii> vect_h,vect_m;
 96 
 97         for(int i=1; i<=n; i++)
 98             for(int j=1; j<=m; j++)
 99             {
100                 cin>>ch;
101                 if(ch=='H')    vect_h.push_back(make_pair(i,j)),cnt++;
102                 if(ch=='m')    vect_m.push_back(make_pair(i,j));
103             }
104 
105 
106         for(int i=0; i<vect_m.size(); i++)
107         {
108             int a=vect_m[i].first, b=vect_m[i].second;
109             for(int j=0; j<vect_h.size(); j++)
110             {
111                 int c=vect_h[j].first, d=vect_h[j].second;
112                 int dis=abs(c-a)+abs(d-b);
113 
114                 add_node(i+1,cnt+j+1,dis,1,0);
115                 add_node(cnt+j+1,i+1,-dis,0,0);
116             }
117         }
118 
119         for(int i=0; i<vect_m.size(); i++)            //添加源点
120         {
121             add_node(0,i+1,0,1,0);
122             add_node(i+1,0,0,0,0);
123 
124         }
125         for(int j=0; j<vect_h.size(); j++)          //添加汇点
126         {
127             add_node(cnt+j+1,cnt*2+1,0,1,0);
128             add_node(cnt*2+1,cnt+j+1,0,0,0);
129         }
130         add_node(cnt*2+1,cnt*2+2, 0, 1, 0);     //这是为了每次只流过1。再汇点后面再加个汇点，容量是1，但是每次spfa后都不增加flow。
131         add_node(cnt*2+2,cnt*2+1, 0, 0, 0);
132         cout<<cal(0,cnt*2+2)<<endl;
133     }
134     return 0;
135 }
AC代码
```

