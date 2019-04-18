# HDU 1599 find the mincost route （无向图的最小环） - xcw0754 - 博客园
# [HDU 1599 find the mincost route （无向图的最小环）](https://www.cnblogs.com/xcw0754/p/4695779.html)
题意：
　　给一个带权无向图，求其至少有3个点组成的环的最小权之和。
思路：
　　（1）DFS可以做，实现了确实可以，只是TLE了。量少的时候应该还是可以水一下的。主要思路就是，深搜过程如果当前点搜到一个点访问过了，而其不是当前点的父亲，则肯定有环，可以更新答案。深搜过程要记录路径和，父亲，是否访问过等等信息，因为图可能有多个连通分量。
```
1 #include <bits/stdc++.h>
 2 #define INF 0x7f7f7f7f
 3 #define pii pair<int,int>
 4 #define LL unsigned long long
 5 using namespace std;
 6 const int N=110;
 7 struct node
 8 {
 9     int from, to, cost;
10     node(){};
11     node(int from,int to,int cost):from(from),to(to),cost(cost){};
12 }edge[N*N];
13 int edge_cnt;
14 vector<int> vect[N];
15 
16 void add_node(int from,int to,int cost)
17 {
18     edge[edge_cnt]=node(from, to, cost);
19     vect[from].push_back(edge_cnt++);
20 }
21 
22 int sum[N], vis[N], inq[N], pre[N], ans;
23 
24 void DFS(int x)
25 {
26     vis[x]=1;
27     inq[x]=1;
28     for(int i=0; i<vect[x].size(); i++)
29     {
30         node e=edge[vect[x][i]];
31         if( inq[e.to] && pre[x]!=e.to )
32         {
33             ans=min(ans, sum[x]+e.cost-sum[e.to]);
34         }
35         if( !inq[e.to] )
36         {
37             pre[e.to]=x;
38             sum[e.to]=sum[x]+e.cost;
39             DFS(e.to);
40             sum[e.to]=0;
41             pre[e.to]=0;
42         }
43     }
44     inq[x]=0;
45 }
46 
47 int cal(int n)
48 {
49     ans=INF;
50     memset(vis,0,sizeof(vis));
51     memset(inq,0,sizeof(inq));
52     memset(pre,0,sizeof(pre));
53     memset(sum,0,sizeof(sum));
54     for(int i=1; i<=n; i++)
55     {
56         if(!vis[i])
57             DFS(i);
58     }
59     return ans==INF? 0: ans;
60 }
61 
62 int main()
63 {
64     freopen("input.txt", "r", stdin);
65     int t, a, b, c, n, m;
66     while(~scanf("%d %d", &n, &m))
67     {
68         edge_cnt=0;
69         for(int i=0; i<=n; i++) vect[i].clear();
70 
71         for(int i=0; i<m; i++)
72         {
73             scanf("%d%d%d",&a,&b,&c);
74             add_node(a, b, c);
75             add_node(b, a, c);
76         }
77         
78         int ans=cal(n);
79         if(ans) printf("%d\n", ans);
80         else    printf("It's impossible.\n");
81     }
82     return 0;
83 }
TLE代码
```
　　（2）dijkstra。枚举删除某一条边，求该边两点间的最短距离。要注意的是，重边只留1条权最小的，其他删掉，这样就能保证至少出现3个点。
　　608ms
```
1 #include <bits/stdc++.h>
  2 #include <iostream>
  3 #include <cstdio>
  4 #include <vector>
  5 #include <cstring>
  6 #include <deque>
  7 #define INF 0x7f7f7f7f
  8 #define pii pair<int,int>
  9 #define LL unsigned long long
 10 using namespace std;
 11 const int N=110;
 12 struct node
 13 {
 14     int from, to, cost, tag;
 15     node(){};
 16     node(int from,int to,int cost):from(from),to(to),cost(cost),tag(1){};
 17 }edge[N*N];
 18 int edge_cnt;
 19 vector<int> vect[N];
 20 int g[N][N];
 21 void add_node(int from,int to,int cost)
 22 {
 23     edge[edge_cnt]=node(from, to, cost);
 24     vect[from].push_back(edge_cnt++);
 25 }
 26 
 27 
 28 int vis[N], cost[N];
 29 int dijkstra(int s,int e)
 30 {
 31     memset(cost, 0x7f, sizeof(cost));
 32     memset(vis, 0, sizeof(vis));
 33     cost[s]=0;
 34     priority_queue<pii,vector<pii>,greater<pii> > que;
 35     que.push(make_pair(0, s));
 36 
 37     while(!que.empty())
 38     {
 39         int x=que.top().second;
 40         que.pop();
 41         if(vis[x])  continue;
 42         vis[x]=1;
 43         for(int i=0; i<vect[x].size(); i++)
 44         {
 45             node e=edge[vect[x][i]];
 46             if( e.tag && cost[e.to]>cost[x]+e.cost )
 47             {
 48                 cost[e.to]=cost[x]+e.cost;
 49                 que.push(make_pair(cost[e.to], e.to));
 50             }
 51         }
 52     }
 53     return cost[e];
 54 
 55 }
 56 
 57 int cal()
 58 {
 59     int ans=INF;
 60     for(int i=0; i<edge_cnt; i+=2)
 61     {
 62         edge[i].tag=0;
 63         edge[i^1].tag=0;
 64         ans=min(ans, edge[i].cost+dijkstra(edge[i].from, edge[i].to));
 65         edge[i].tag=1;
 66         edge[i^1].tag=1;
 67     }
 68     return ans==INF? 0: ans;
 69 }
 70 
 71 int main()
 72 {
 73     freopen("input.txt", "r", stdin);
 74     int t, a, b, c, n, m;
 75     while(~scanf("%d %d", &n, &m))
 76     {
 77         edge_cnt=0;
 78         for(int i=0; i<=n; i++) vect[i].clear();
 79         memset(g, 0x7f, sizeof(g));
 80 
 81         for(int i=0; i<m; i++)
 82         {
 83             scanf("%d%d%d",&a,&b,&c);
 84             g[b][a]=g[a][b]=min(g[a][b], c);
 85         }
 86         for(int i=1; i<=n; i++) //为了去重边
 87         {
 88             for(int j=i+1; j<=n; j++)
 89             {
 90                 if(g[i][j]<INF)
 91                 {
 92                     add_node(i, j, g[i][j]);
 93                     add_node(j, i, g[i][j]);
 94                 }
 95             }
 96         }
 97         int ans=cal();
 98         if(ans) printf("%d\n", ans);
 99         else    printf("It's impossible.\n");
100     }
101     return 0;
102 }
AC代码
```
　　（3）floyd。dijkstra是枚举删某条边，而floyd是枚举相连的两条边。先理解floyd的思想，穷举每个点k作为中间节点来更新其他点a和b之间的距离，而当某个点未被k枚举到时，是不可能有一条路径将其包含在中间的，它顶多可以作为路径的起点或者终点。利用这点，在未枚举到某点k作为中间点时，可以枚举一下与k相连的两条边，即i->k->j。
 　　78ms
```
1 #include <bits/stdc++.h>
 2 #define INF 0x7f7f7f7f
 3 #define pii pair<int,int>
 4 #define LL unsigned long long
 5 using namespace std;
 6 const int N=110;
 7 int g[N][N], dist[N][N];
 8 
 9 
10 int cal(int n)  //floyd
11 {
12     int ans=INF;
13     for(int k=1; k<=n; k++) //注意枚举顺序。
14     {
15         //枚举两条边i->k->j。
16         for(int i=1; i<=n; i++ )
17         {
18             if(g[i][k]==INF || k==i) continue;
19             for(int j=i+1; j<=n; j++)   //i和j不能相等，才能保证至少3个点。
20             {
21                 if(g[k][j]==INF || dist[i][j]==INF || k==j )    continue;
22                 int dis=g[i][k] + g[k][j] + dist[i][j];
23                 ans=min(ans, dis);
24             }
25         }
26         for(int i=1; i<=n; i++)
27         {
28             for(int j=1; j<=n; j++)
29             {
30                 if(dist[i][k]==INF || dist[k][j]==INF) continue;
31                 dist[i][j]=min(dist[i][j], dist[i][k]+dist[k][j]);
32             }
33         }
34     }
35 
36     return ans==INF? 0: ans;
37 }
38 
39 int main()
40 {
41     freopen("input.txt", "r", stdin);
42     int t, a, b, c, n, m;
43     while(~scanf("%d %d", &n, &m))
44     {
45         memset(g, 0x7f, sizeof(g));
46         for(int i=0; i<m; i++)
47         {
48             scanf("%d%d%d",&a,&b,&c);
49             g[b][a]=g[a][b]=min(g[a][b], c);
50         }
51         memcpy(dist, g, sizeof(g));
52         for(int i=1; i<=n; i++) dist[i][i]=0;
53 
54         int ans=cal(n);
55         if(ans) printf("%d\n", ans);
56         else    printf("It's impossible.\n");
57     }
58     return 0;
59 }
AC代码
```

