# UVA 11374 Airport Express 机场快线（单源最短路，dijkstra，变形） - xcw0754 - 博客园
# [UVA 11374 Airport Express 机场快线（单源最短路，dijkstra，变形）](https://www.cnblogs.com/xcw0754/p/4673006.html)
题意：
　　给一幅图，要从s点要到e点，图中有两种无向边分别在两个集合中，第一个集合是可以无限次使用的，第二个集合中的边只能挑1条。问如何使距离最短？输出路径，用了第二个集合中的哪条边，最短距离。
思路：
（1）简单易操作方法：既然第二个集合的边只能有1条，就穷举下这些边，可能的边集进行求最短路，同时记录3个答案。复杂度是O(m*k)。
（2）时间复杂度低：不妨先求从s到每个其他点的距离d1[i]，再求e到其他每个点的距离d2[i]，接下来穷举第二个集合中的每条边u-v，那么最短距离为d1[u]+dis[u][v]+d2[v]，注意边是无向的。麻烦在记录路径而已，还是挺容易操作的。复杂度是O(2m+k)。
下面是第一种方法的代码：
```
1 #include <bits/stdc++.h>
  2 #define LL long long
  3 #define pii pair<int,int>
  4 #define INF 0x7f7f7f7f
  5 using namespace std;
  6 const int N=1010;
  7 int n, m, k;
  8 vector<int> vect[N];
  9 int edge_cnt;
 10 struct node
 11 {
 12     int from, to, dis, tag;
 13     node(){};
 14     node(int from,int to,int dis,int tag):from(from),to(to),dis(dis),tag(tag){};
 15 }edge[N*5];
 16 
 17 
 18 
 19 void add_node(int from,int to,int dis,int tag)
 20 {
 21     edge[edge_cnt]=node(from,to,dis,tag);
 22     vect[from].push_back(edge_cnt++);
 23 }
 24 
 25 
 26 int tag[N], dist[N], vis[N], path[N];
 27 int dijkstra(int s,int e)
 28 {
 29     memset(dist,0x7f,sizeof(dist));
 30     memset(tag,0,sizeof(tag));
 31     memset(vis,0,sizeof(vis));
 32     memset(path,0,sizeof(path));
 33 
 34     priority_queue<pii,vector<pii>,greater<pii> > que;
 35     que.push(make_pair(0, s));
 36     dist[s]=0;
 37 
 38     while(!que.empty())
 39     {
 40         int x=que.top().second;
 41         que.pop();
 42         if(vis[x])  continue;
 43         vis[x]=1;
 44         for(int i=0; i<vect[x].size(); i++)
 45         {
 46             node e=edge[vect[x][i]];
 47             if( e.tag>0 && dist[e.to]>dist[x]+e.dis )
 48             {
 49                 dist[e.to]=dist[x]+e.dis;
 50                 path[e.to]=x;
 51                 if(e.tag==2)
 52                 {
 53                     tag[e.to]=true; //到这个点用了快线
 54                 }
 55                 que.push( make_pair(dist[e.to],e.to) );
 56             }
 57         }
 58     }
 59     return dist[e];
 60 }
 61 
 62 void cal(int s ,int e)
 63 {
 64     vector<int> ans;
 65     int quick=0, big=dijkstra(s,e), d=e;//先跑了一遍不用快线的
 66 
 67     while( d )
 68     {
 69         ans.push_back(d);
 70         d=path[d];
 71     }
 72 
 73     for(int i=m*2; i<(m+k)*2; i+=2 )   //穷举每条可以用的快线
 74     {
 75         edge[i].tag=2;
 76         edge[i+1].tag=2;
 77 
 78         int dis=dijkstra(s,e);
 79         if( dis<big )
 80         {
 81             big=dis;
 82             ans.clear();
 83             int ed=e, tmp=0;
 84             while( ed )
 85             {
 86                 if( tag[ed] )    tmp=path[ed];    //有可能没有用到
 87                 ans.push_back(ed);
 88                 ed=path[ed];
 89             }
 90             quick=tmp;//如果没有用到，也会及时更新为0
 91         }
 92         edge[i].tag=0;
 93         edge[i+1].tag=0;
 94     }
 95 
 96     reverse(ans.begin(),ans.end());
 97     printf("%d",ans[0]);    //千万注意输出格式
 98     for(int i=1; i<ans.size(); i++)    printf(" %d",ans[i]);
 99     printf("\n");
100 
101     if(quick) printf("%d\n",quick);
102     else puts("Ticket Not Used");//别忘了
103 
104     printf("%d\n",big);
105 }
106 
107 int main()
108 {
109     freopen("input.txt", "r", stdin);
110     int s=0, e, a, b, c, ttt=0;
111     while(~scanf("%d%d%d", &n, &s, &e))
112     {
113         if(ttt)   printf("\n");ttt++;//格式啊！
114         edge_cnt=0;
115         memset(edge,0,sizeof(edge));
116         for(int i=0; i<=n; i++) vect[i].clear();
117         scanf("%d",&m);
118         for(int i=0; i<m; i++)
119         {
120             scanf("%d%d%d",&a,&b,&c);
121             add_node(a,b,c,1);
122             add_node(b,a,c,1);
123         }
124         scanf("%d",&k);
125         for(int i=0; i<k; i++)
126         {
127             scanf("%d%d%d",&a,&b,&c);
128             add_node(a,b,c,0);
129             add_node(b,a,c,0);
130         }
131         cal(s,e);
132     }
133     return 0;
134 }
AC代码
```

