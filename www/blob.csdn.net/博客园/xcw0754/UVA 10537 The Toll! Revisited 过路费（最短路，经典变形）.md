# UVA 10537 The Toll! Revisited 过路费（最短路，经典变形） - xcw0754 - 博客园
# [UVA 10537 The Toll! Revisited 过路费（最短路，经典变形）](https://www.cnblogs.com/xcw0754/p/4674650.html)
题意：给一个无向图，要从起点s运送一批货物到达终点e，每个点代表城镇/乡村，经过城镇需要留下（num+19）/20的货物，而经过乡村只需要1货物即可。现在如果要让p货物到达e，那么从起点出发最少要准备多少货物？输出答案和路径（多条路径则必须输出字典序最小的）。注：终点需要花费，而起点不需要。
思路：这最短路变形的不错。要逆推过来求最短路径，那么就从e出发到s的距离！只是p比较大，而且城镇还得推出前一站到底需要多少货物，既然直接计算那么麻烦，也可以一直p++直到能留下p为止就推出来了；而乡村就容易推了，只是+1。但是还有个字典序要解决，则在碰到dist[u]==dist[v]+距离的时候就比较一下字典序谁小就记谁。
　　有一点别忘了，这是在逆推！！所以你要用出发点去更新终点时，要以出发点的dist来比较字典序。无向边是要建两条有向的，防止重边。
```
1 #include <bits/stdc++.h>
  2 #define LL long long
  3 #define pii pair<int,int>
  4 #define INF 0x7f7f7f7f
  5 using namespace std;
  6 const int N=200;
  7 int n, m, l, edge_cnt;
  8 LL p;
  9 vector<int> vect[N];
 10 struct node
 11 {
 12     int from, to;
 13     node(){};
 14     node(int from,int to):from(from),to(to){};
 15 }edge[100000];
 16 
 17 void add_node(int from,int to)
 18 {
 19     edge[edge_cnt]=node(from, to);
 20     vect[from].push_back(edge_cnt++);
 21 }
 22 
 23 LL dist[N];
 24 int path[N], vis[N];
 25 LL dijkstra(int s,int e)
 26 {
 27     memset(path, 0, sizeof(path));
 28     memset(vis, 0, sizeof(vis));
 29     memset(dist, 0x7f, sizeof(dist));
 30 
 31     priority_queue<pii,vector<pii>,greater<pii> >  que;
 32     que.push(make_pair(p,s));
 33     dist[s]=p;
 34 
 35     while(!que.empty())
 36     {
 37         int x=que.top().second;que.pop();
 38         if(vis[x])  continue;
 39         vis[x]=1;
 40 
 41         bool flag=isupper(x);   //大写，花费多的
 42         LL t=dist[x]+(dist[x]+19)/20;
 43         while(t-(t+19)/20<dist[x])  t++;    //注意不要超时
 44 
 45         for(int i=0; i<vect[x].size(); i++)
 46         {
 47             node e=edge[vect[x][i]];
 48             if( flag )
 49             {
 50                 if(dist[e.to]>=t)
 51                 {
 52                     if(dist[e.to]==t)
 53                     {
 54                         if( x<edge[path[e.to]].from ) path[e.to]=vect[x][i];   //字典序
 55                     }
 56                     else    path[e.to]=vect[x][i];
 57 
 58                     dist[e.to]=t;
 59                     que.push(make_pair(dist[e.to],e.to));
 60                 }
 61             }
 62             else
 63             {
 64                 if(dist[e.to]>=dist[x]+1)
 65                 {
 66                     if(dist[e.to]==dist[x]+1)
 67                     {
 68                         if( x<edge[path[e.to]].from ) path[e.to]=vect[x][i];   //字典序
 69                     }
 70                     else    path[e.to]=vect[x][i];
 71 
 72                     dist[e.to]=dist[x]+1;
 73                     que.push(make_pair(dist[e.to],e.to));
 74                 }
 75             }
 76         }
 77     }
 78     return dist[e];
 79 }
 80 
 81 
 82 void cal(int s,int e)
 83 {
 84     cout<<dijkstra(s,e)<<endl;
 85     vector<char> ans;
 86     int ed=e;
 87     while(ed!=s)
 88     {
 89         ans.push_back( ed );
 90         int t=path[ed];
 91         ed=edge[t].from;
 92     }
 93     ans.push_back(s);
 94     printf("%c",ans[0]);
 95     for(int i=1; i<ans.size(); i++)    printf("-%c",ans[i]);
 96     cout<<endl;
 97 }
 98 
 99 
100 int main()
101 {
102     freopen("input.txt", "r", stdin);
103     char a, b;
104     int j=0;
105     while(scanf("%d", &n), n>=0)
106     {
107         edge_cnt=0;
108         memset(edge,0,sizeof(edge));
109         for(int i=0; i<N; i++) vect[i].clear();
110         int up=0;
111         for(int i=0; i<n; i++)
112         {
113             getchar();
114             scanf("%c %c",&a,&b);   //%c会接收到换行！！
115             add_node(a, b);
116             add_node(b, a);
117         }
118         scanf("%lld %c %c",&p, &a, &b);
119         printf("Case %d:\n",++j);
120         cal(b, a);
121     }
122     return 0;
123 }
AC代码
```

