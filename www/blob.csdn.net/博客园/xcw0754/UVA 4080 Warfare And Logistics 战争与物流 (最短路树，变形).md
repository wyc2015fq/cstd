# UVA  4080 Warfare And Logistics 战争与物流 (最短路树，变形) - xcw0754 - 博客园
# [UVA  4080 Warfare And Logistics 战争与物流 (最短路树，变形)](https://www.cnblogs.com/xcw0754/p/4674017.html)
题意：
　　给一个无向图，n个点，m条边，可不连通，可重边，可多余边。两个问题，第一问：求任意点对之间最短距离之和。第二问：必须删除一条边，再求第一问，使得结果变得更大。
思路：
　　其实都是在求最短路的过程。
　　第一问可以floyd解决，也可以SSSP解决。注意是任意两个点，（a,b）和(b,a)是不同的，都要算。
　　第二问要穷举删除每条边，再求第一问。为了降低复杂度，假设用dijkstra求最短路，那么可以利用第一问中所生成的树，共n棵，每棵至多n-1条边，如果穷举的边不在该某树上，那么该树的所有路径长不变，不必计算，否则需要计算。所以需要记录路径，并将整棵树的边集存起来，同时保存每棵树的任意两点路径之和。
　　用结构体可以解决重边，问题应该不多，要注意各种细节，错了就重新打，也许更快。
```
1 #include <bits/stdc++.h>
  2 #define LL long long
  3 #define pii pair<int,int>
  4 #define INF 0x7f7f7f7f
  5 using namespace std;
  6 const int N=220;
  7 int n, m, l, edge_cnt;
  8 vector<int> vect[N];
  9 
 10 struct node
 11 {
 12     int from, to, dis,tag;
 13     node(){};
 14     node(int from,int to,int dis,int tag):from(from),to(to),dis(dis),tag(tag){};
 15 }edge[2050];
 16 
 17 void add_node(int from,int to,int dis,int tag)
 18 {
 19     edge[edge_cnt]=node(from, to, dis, tag);
 20     vect[from].push_back(edge_cnt++);
 21 }
 22 
 23 int dist[N], vis[N], path[N];
 24 LL dijkstra(int s)
 25 {
 26     memset(dist,0x7f,sizeof(dist));
 27     memset(vis,0,sizeof(vis));
 28     for(int i=0; i<=n; i++) path[i]=-1;
 29 
 30     priority_queue<pii,vector<pii>,greater<pii> >   que;
 31     dist[s]=0;
 32     que.push(make_pair(0,s));
 33 
 34     while(!que.empty())
 35     {
 36         int x=que.top().second;que.pop();
 37         if(vis[x])  continue;
 38         vis[x]=1;
 39         for(int i=0; i<vect[x].size(); i++)
 40         {
 41             node e=edge[vect[x][i]];
 42             if(e.tag>0 && dist[e.to]>dist[e.from]+e.dis )
 43             {
 44                 path[e.to]=vect[x][i];
 45                 dist[e.to]=dist[e.from]+e.dis;
 46                 que.push(make_pair(dist[e.to], e.to));
 47             }
 48         }
 49     }
 50 
 51     LL sum=0;
 52     for(int i=1; i<=n; i++ )
 53     {
 54         if(dist[i]>=INF)    sum+=l;//不可达的，按L算
 55         else    sum+=dist[i];
 56     }
 57     return sum;
 58 }
 59 
 60 LL ans1[N];
 61 int cal()
 62 {
 63     memset(ans1,0,sizeof(ans1));
 64     LL  first=0;
 65     unordered_set<int> tree[N];
 66     for(int i=1; i<=n; i++)
 67     {
 68         ans1[i]=dijkstra(i);
 69         first+=ans1[i];
 70         //收集边
 71         for(int k=1; k<=n; k++)
 72         {
 73             if(path[k]>=0)//注意如何初始化
 74             {
 75                 tree[i].insert(path[k]);
 76                 tree[i].insert(path[k]^1);
 77             }
 78         }
 79     }
 80     //另一个问
 81     LL second=0;
 82     for(int i=0; i<edge_cnt; i+=2)
 83     {
 84         edge[i].tag=edge[i+1].tag=0;
 85         LL sum=0;
 86         for(int j=1; j<=n; j++)
 87         {
 88             if( tree[j].find(i)==tree[j].end() )  //是点j的树上，要重新算
 89                 sum+=ans1[j];
 90             else
 91                 sum+=dijkstra(j);
 92         }
 93         second=max(second, sum);
 94         edge[i].tag=edge[i+1].tag=1;
 95     }
 96     printf("%lld %lld\n", first, second );//仅1个空格
 97 }
 98 
 99 int main()
100 {
101     freopen("input.txt", "r", stdin);
102     int a, b, c;
103     while(scanf("%d%d%d", &n, &m, &l)==3)
104     {
105         edge_cnt=0;
106         memset(edge,0,sizeof(edge));
107         for(int i=0; i<=n; i++) vect[i].clear();
108         for(int i=0; i<m; i++)
109         {
110             scanf("%d%d%d",&a,&b,&c);
111             if(a==b)    continue;
112             add_node(a,b,c,1);
113             add_node(b,a,c,1);
114         }
115         cal();
116     }
117     return 0;
118 }
AC代码
```

