# UVA 10801 Lift Hopping 电梯换乘（最短路，变形） - xcw0754 - 博客园
# [UVA 10801 Lift Hopping 电梯换乘（最短路，变形）](https://www.cnblogs.com/xcw0754/p/4668301.html)
题意：
　　有n<6部电梯，给出每部电梯可以停的一些特定的楼层，要求从0层到达第k层出来，每次换乘需要60秒，每部电梯经过每层所耗时不同，具体按 层数*电梯速度 来算。问经过多少秒到达k层（k可以为0）？
思路：
　　dijkstra再加一些特殊的处理就行了。首先要考虑，如何建图：
（1）每层作为一个点。但是特定路径可以有多种权，比如从2->5可以坐1号电梯10s，但是坐2号只需要5s，所以有重边。
（2）k=0时，不耗时间。
（3）有多种路径可达同一楼层且权值相同，那么从本楼层到另一楼层有多种选择，有时可以不换电梯，有时需要换。比如到达5楼的有2条路径，权都是5，但是是两部不同的电梯1和2，此时有其他电梯可以从5到7楼，其中有一部仍是电梯1，如果坐电梯1则不需要换乘时间，坐其他电梯就要了。所以要记录到某个点权值相等的电梯号。
```
1 #include <bits/stdc++.h>
  2 #define LL long long
  3 #define pii pair<int,int>
  4 #define INF 0x7f7f7f7f
  5 using namespace std;
  6 const int N=1010;
  7 int take[6];
  8 char s[1000];
  9 vector<int> lift[6], vect[500];
 10 int cost[N], vis[N];
 11 struct node
 12 {
 13     int from, to, lift, cost;
 14     node(){};
 15     node(int from,int to,int lift,int cost):from(from),to(to),lift(lift),cost(cost){};
 16 }edge[100000];
 17 int edge_cnt;
 18 
 19 void add_node(int from,int to,int lift,int cost)
 20 {
 21     edge[edge_cnt]=node(from,to,lift,cost);
 22     vect[from].push_back(edge_cnt++);
 23 }
 24 
 25 void build_graph(int n) //重新建图
 26 {
 27     for(int i=0; i<n; i++)
 28     {
 29         for(int j=0; j<lift[i].size(); j++)
 30         {
 31             for(int t=j+1; t<lift[i].size(); t++)
 32             {
 33                 int a=lift[i][j];
 34                 int b=lift[i][t];
 35                 add_node(a,b,i,(b-a)*take[i]);
 36                 add_node(b,a,i,(b-a)*take[i]);
 37             }
 38         }
 39     }
 40 }
 41 
 42 int dijkstra(int s,int e)
 43 {
 44     vector<int> flo[101];
 45     memset(cost,0x7f,sizeof(cost));
 46     memset(vis,0,sizeof(vis) );
 47 
 48     priority_queue<pii,vector<pii>,greater<pii> >    que;
 49     que.push( make_pair(0,s));
 50     cost[s]=0;
 51     while(!que.empty())
 52     {
 53         int x=que.top().second;que.pop();
 54         if(vis[x])  continue;
 55 
 56         vis[x]=1;
 57         for(int i=0; i<vect[x].size(); i++)
 58         {
 59             node e=edge[vect[x][i]];
 60             int ext=60;
 61 
 62             for(int j=0; j<flo[e.from].size(); j++)   //如果有一个匹配，就不用额外时间
 63                 if( flo[e.from][j]==e.lift )    ext=0;
 64 
 65             if(cost[e.to]>=cost[e.from]+ext+e.cost )
 66             {
 67                 if( cost[e.to]>cost[e.from]+ext+e.cost ) flo[e.to].clear();
 68                 flo[e.to].push_back(e.lift);
 69 
 70                 cost[e.to]= cost[e.from] +ext +e.cost;
 71                 que.push( make_pair(cost[e.to], e.to) );
 72             }
 73         }
 74     }
 75     return cost[e];
 76 }
 77 
 78 int main()
 79 {
 80     freopen("input.txt", "r", stdin);
 81     int n, k;
 82     while(~scanf("%d%d",&n,&k))
 83     {
 84         for(int i=0; i<n; i++)  lift[i].clear();
 85         for(int i=0; i<101; i++)  vect[i].clear();
 86         memset(s,0,sizeof(s));
 87 
 88         for(int i=0; i<n; i++)    scanf("%d",&take[i]);
 89         getchar();
 90         for(int i=0; i<n; i++)
 91         {
 92             lift[i].clear();
 93             gets(s);
 94             int p=0;
 95             while(s[p]!='\0')
 96             {
 97                 if(s[p]==' ')   p++;
 98                 int tmp=0;
 99                 while(s[p]!=' ' &&s[p]!='\0' )    tmp=tmp*10+(s[p++]-'0');
100                 lift[i].push_back(tmp);
101             }
102         }
103         build_graph(n);
104         int ans=dijkstra(0,k);
105         if(!k)  puts("0");
106         else if(ans==INF)    puts("IMPOSSIBLE");
107         else    printf("%d\n", ans-60 ) ;
108     }
109     return 0;
110 }
AC代码
```

