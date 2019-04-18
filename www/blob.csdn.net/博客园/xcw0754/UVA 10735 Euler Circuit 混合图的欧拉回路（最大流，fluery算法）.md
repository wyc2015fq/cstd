# UVA 10735 Euler Circuit 混合图的欧拉回路（最大流，fluery算法） - xcw0754 - 博客园
# [UVA 10735 Euler Circuit 混合图的欧拉回路（最大流，fluery算法）](https://www.cnblogs.com/xcw0754/p/4656462.html)
题意：给一个图，图中有部分是向边，部分是无向边，要求判断是否存在欧拉回路，若存在，输出路径。
分析：欧拉回路的定义是，从某个点出发，每条边经过一次之后恰好回到出发点。
　　无向边同样只能走一次，只是不限制方向而已，那么这个情况下就不能拆边。不妨先按照所给的start和end的顺序，初步定下该无向边的顺序（若不当，一会再改）。那么有个问题，我们需要先判断其是否存在欧拉回路先。
　　混合图不满足欧拉回路因素有：（1）一个点的度（无论有无向）是奇数的，那么其肯定不能满足出边数等于入边数。（2）有向边的出入度过于悬殊（悬殊是指，拿所有无向边来怎么抵消都是不平衡）。
　　首先可以先将不满足上述两个条件的case结束掉，无解。
　　那么还有个问题，这样随便地定下一条无向边为随意一个方向不是太随意了吗？当然，这样做不可能保证每个点的出入度就平衡了，所以我们得想办法让每个点的出入度都平衡。考虑到，如果一个点的出度多了，那么该点可以将多出的部分出度换回来入度，这相当于将出度“流向”其他需要出度的点。好像可以最大流解决，好吧，接下来讲建图。
　　建图。对于任意一条无向边（被我们已经随意定向的那些)，假设其方向初定位u-v。那么u有出度可以赠人了，所以新图中u到v有边，容量是1，表示将出度送给v，当然自己就会获得入度1个。但是并不是所有的点u的出度都很多以至于可以送人，不过其出度可以赠人这倒是没错的，看它肯不肯了。当chudu[u]>rudu[u]时，必定可以给人，且可以给(chudu[u]-rudu[u])/2，这很明显。那么应该有边从源点到u，容量为(chudu[u]-rudu[u])/2，表示其可以流出这么多个出度（注意别重复建边）。同理，对于缺入度的点，应该有边到汇点，容量为(rudu[v]-chudu[v])/2。这样图就建成。
　　建完图后还要再判断一次是否有解，即当从源点出来的容量和到达汇点的容量不一致时，无解。因为缺入度的点和缺出度的点数不一样多，平衡不了。
　　若有解，再对新图来求一次最大流，最大流应该等于源点到其他点的容量之和。及所有点都平衡好了。所有有flow>0的边都是需要反过来的。全部在原图上修改后，重新建邻接表，进行求欧拉回路路径，这个用fluery算法即可。
　　特别需要注意的是：2个case间要空行。
```
1 #include <bits/stdc++.h>
  2 #define LL long long
  3 #define pii pair<int,int>
  4 #define INF 0x7f7f7f7f
  5 using namespace std;
  6 const int N=150;
  7 vector<int> vect[N], vec[N], ans;
  8 
  9 int can[N], notru[N], notchu[N], chu[N], ru[N], edge_cnt, sum_flow1, sum_flow2, vis[550];
 10 
 11 bool check(int n)   //保证有解
 12 {
 13     for(int i=1; i<=n; i++)    if( 1==(1&(ru[i]+chu[i])) )  return false;    //奇数个度
 14     for(int i=1; i<=n; i++)     //每个点可以补救。若不可改出度为100,入度为2，肯定不行。得补得上才行
 15     {
 16         if( can[i]-abs(notru[i]-notchu[i])>=0 )    continue;
 17         else    return false;
 18     }
 19     return true;
 20 }
 21 
 22 struct node //网络流用的边
 23 {
 24     int from;
 25     int to;
 26     int cap;
 27     int flow;
 28     int has;
 29     int isU;
 30 }edge[4000], edg[550];
 31 
 32 void add_node(int from,int to,int cap,int flow,int has)
 33 {
 34     edge[edge_cnt].from=from;
 35     edge[edge_cnt].to=to;
 36     edge[edge_cnt].cap=cap;
 37     edge[edge_cnt].flow=flow;
 38     edge[edge_cnt].has=has;
 39     vect[from].push_back(edge_cnt++);
 40 }
 41 
 42 bool vis1[N], vis2[N];
 43 void build_graph(int n,int m)    //根据无向边建图。
 44 {
 45     memset(vis1,0,sizeof(vis1));
 46     memset(vis2,0,sizeof(vis2));
 47     for(int i=0; i<m; i++)
 48     {
 49         if(edg[i].isU)
 50         {
 51             int a=edg[i].from;
 52             int b=edg[i].to;
 53             add_node(a, b, 1, 0, i);  //a的出度可给人
 54             add_node(b, a, 0, 0, i);
 55 
 56             if(!vis1[a] && chu[a]>ru[a] )      //出度多，可流向别人
 57             {
 58                 sum_flow1+=(chu[a]-ru[a])/2;
 59                 vis1[a]=1;
 60                 add_node(0, a, (chu[a]-ru[a])/2, 0, -1);  //源点-（出边多的点）
 61                 add_node(a, 0, 0, 0, -1);
 62             }
 63             if(!vis2[b] && ru[b]>chu[b])    //所有缺边的都连到汇点
 64             {
 65                 sum_flow2+=(ru[b]-chu[b])/2;
 66                 vis2[b]=1;
 67                 add_node(b, n+1, (ru[b]-chu[b])/2, 0, -1 );
 68                 add_node(n+1, b, 0, 0, -1 );
 69             }
 70         }
 71     }
 72 }
 73 
 74 int flow[N], path[N];
 75 int BFS(int s,int e)
 76 {
 77     deque<int> que(1,s);
 78     flow[s]=INF;
 79     while(!que.empty())
 80     {
 81         int x=que.front();
 82         que.pop_front();
 83         for(int i=0; i<vect[x].size(); i++)
 84         {
 85             node e=edge[vect[x][i]];
 86             if(!flow[e.to] && e.cap>e.flow )
 87             {
 88                 flow[e.to]=min(flow[e.from],e.cap-e.flow );
 89                 path[e.to]=vect[x][i];
 90                 que.push_back(e.to);
 91             }
 92         }
 93         if(flow[e]) return flow[e];
 94     }
 95     return flow[e];
 96 }
 97 
 98 int cal(int s,int e)   //求最大流。只能满流有解
 99 {
100     int ans_flow=0;
101     while(true)
102     {
103         memset(flow,0,sizeof(flow));
104         memset(path,0,sizeof(path));
105         int tmp=BFS(s,e);
106         if(tmp==0)  return ans_flow;
107         ans_flow+=tmp;
108         int ed=e;
109         while(ed!=s)
110         {
111             int t=path[ed];
112             edge[t].flow+=tmp;
113             edge[t^1].flow-=tmp;
114             ed=edge[t].from;
115         }
116     }
117 }
118 
119 void change_edge(int m)      //改变边的方向，重新建邻接表。
120 {
121     for(int i=0; i<edge_cnt; i+=2)
122         if(edge[i].has>=0 && edge[i].flow>0 )  //有流过的才需要改
123             swap(edg[edge[i].has].from , edg[edge[i].has].to );
124 
125     for(int i=0; i<N; i++)    vec[i].clear();
126     for(int i=0; i<m; i++)    vec[edg[i].from].push_back(i);  //重新建立临接表
127 }
128 
129 void fluery(int x)   //任意一个点开始即可
130 {
131     for(int i=0; i<vec[x].size(); i++)
132     {
133         int t=vec[x][i];
134         if(!vis[t]) //该边没遍历过
135         {
136             vis[t]=1;
137             fluery(edg[t].to);
138         }
139     }
140     ans.push_back(x);
141 }
142 
143 void init()
144 {
145     edge_cnt=0;
146     sum_flow1=0;
147     sum_flow2=0;
148     memset(can, 0, sizeof(can));
149     memset(notru, 0, sizeof(notru));
150     memset(notchu, 0, sizeof(notchu));
151     memset(chu, 0, sizeof(chu));
152     memset(ru, 0, sizeof(ru));
153     memset(edge, 0, sizeof(edge));
154     memset(edg, 0, sizeof(edg));
155     for(int i=0; i<N; i++)
156         vec[i].clear(),vect[i].clear();
157 }
158 
159 int main()
160 {
161     freopen("input.txt", "r", stdin);
162     int t, a, b, n, m;
163     char c;
164     cin>>t;
165     while(t--)
166     {
167         init();
168         scanf("%d%d",&n,&m);
169         for(int i=0; i<m; i++)
170         {
171             scanf("%d%d",&a,&b);
172             while((c=getchar())==' ' );
173             //cin>>c;
174             //原图*************************
175             edg[i].from=a;
176             edg[i].to=b;
177             edg[i].isU=(c=='U'?1:0);
178             vec[a].push_back(i);
179             //统计度***********************
180             chu[a]++,ru[b]++;                         //总出入度
181             if(c=='U')    can[a]++,can[b]++;          //保存无向边的度
182             else          notru[b]++,notchu[a]++;     //登记有向边的出入度
183         }
184 
185         if(!check(n))    puts("No euler circuit exist");  //检查是否有解
186         else
187         {
188             build_graph(n, m);  //建临时图edge
189             if(sum_flow1!=sum_flow2 || cal(0, n+1)!=sum_flow1   )        //增广路求最大流
190             {
191                 puts("No euler circuit exist");
192                 if(t)   printf("\n");
193                 continue;
194             }
195             change_edge(m);     //改变有流的边，重建原图edg的邻接表
196             memset(vis, 0, sizeof(vis));
197             ans.clear();
198             fluery(n);  //求欧拉回路路径
199             for(int i=ans.size()-1; i>0; i--)    printf("%d ",ans[i]); //反向输出路径
200             printf("%d\n",ans[0]);
201         }
202         if(t)   printf("\n");
203     }
204     return 0;
205 }
AC代码
```

