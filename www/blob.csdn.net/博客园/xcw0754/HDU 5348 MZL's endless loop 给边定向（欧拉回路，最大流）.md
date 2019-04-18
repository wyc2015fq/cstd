# HDU 5348 MZL's endless loop 给边定向（欧拉回路，最大流） - xcw0754 - 博客园
# [HDU 5348 MZL's endless loop 给边定向（欧拉回路，最大流）](https://www.cnblogs.com/xcw0754/p/4705829.html)
题意：
　　给一个所有你可能想得到的奇葩无向图，要求给每条边定向，使得每个点的入度与出度之差不超过1。输出1表示定向往右，输出0表示定向往左。
思路：
　　网络流也是可以解决的！！应该挺简单理解的。但是由于复杂度的问题，EK和Dinic都搞不定，ISAP才行。
　　利用欧拉回路的思想。既然每个点的入度与出度之差不超过1，那么在每两个奇数度的点添加1条无向边并不会影响到什么。那么先添加一些边进去，使得所有点的度都是偶数的。先随意帮每条边定个方向，进行建新图（用于跑最大流），假设u->v，那么u的出度就是可以给v的（当且仅当这条边反转时），建边u->v，容量为1。添加超级源点s连接到所有【出度大于入度的点】，容量为(出度-入度)/2，表示它有这么多个出度可以赠人。将所有【入度大于出度的点】连一条边到超级汇点t，容量为(入度-出度)/2，表示需要这么多的入度。接着跑一遍最大流（结果必定是满流的，即所有点的出度=入度），每条有流的边就需要将原来随意定下的方向反过来。[参考](http://www.cnblogs.com/xcw0754/p/4656462.html)
```
1 #include <bits/stdc++.h>
  2 #define INF 0x7f7f7f7f
  3 #define pii pair<int,int>
  4 #define LL long long
  5 using namespace std;
  6 const int N=101000;
  7 struct node
  8 {
  9     int from, to, cap, flow, has;
 10     node(){};
 11     node(int from, int to,int cap,int flow,int has):from(from),to(to),cap(cap),flow(flow),has(has){};
 12 }edge[N*15];
 13 int chu[N], ru[N], vis1[N], ans[N*10], edge_cnt;
 14 vector<int> vect[N], rev[N], md;    //图的反向边在这，反向BFS时用的。
 15 vector<pii> g;
 16 
 17 void init(int up)
 18 {
 19     md.clear();
 20     g.clear();
 21     memset(chu,0,sizeof(chu));
 22     memset(ru, 0,sizeof(ru));
 23     edge_cnt=0;
 24     for(int i=0; i<=up; i++)    vect[i].clear(),rev[i].clear();
 25 }
 26 
 27 void add_node(int from,int to,int cap,int flow,int has)
 28 {
 29     edge[edge_cnt]=node(from, to, cap, flow, has);
 30     rev[to].push_back(edge_cnt);    //反向，用于ISAP的BFS
 31     vect[from].push_back(edge_cnt++);
 32 }
 33 
 34 void change()   //修改原来定下方向的边
 35 {
 36     for(int i=0; i<edge_cnt; i++)
 37     {
 38         node e=edge[i];
 39         if( e.has>=0 && e.flow>0 )    ans[e.has]= 1-ans[e.has];
 40     }
 41 }
 42 
 43 bool isok(int n)    //判断是否已经满足要求
 44 {
 45     for(int j=1; j<=n; j++)    if( abs(chu[j]-ru[j])>1 )    return false;
 46     return true;
 47 }
 48 
 49 
 50 int num[N], p[N], d[N], cur[N], vis[N]; //最大流用的
 51 deque<int> que;
 52 void BFS(int s,int e)   //模板
 53 {
 54     memset(vis, 0, sizeof(vis));
 55     que.clear();
 56     que.push_back(e);
 57     vis[e]=1;
 58     d[e]=0;
 59     num[0]=1;   //gap可以在这更新
 60 
 61     while(!que.empty())
 62     {
 63         int x=que.front();que.pop_front();
 64         for(int i=0; i<rev[x].size(); i++)
 65         {
 66             node e=edge[rev[x][i]];
 67             if(!vis[e.from])
 68             {
 69                 vis[e.from]=1;
 70                 d[e.from]=d[e.to]+1;
 71                 ++num[d[e.from]];
 72                 que.push_back(e.from);
 73             }
 74         }
 75     }
 76 }
 77 
 78 int Augment(int s,int e)  //模板
 79 {
 80     int ed=e, big=INF;
 81     while(ed!=s)
 82     {
 83         node &e=edge[p[ed]];
 84         big=min(big, e.cap-e.flow);
 85         ed=edge[p[ed]].from;
 86     }
 87     ed=e;
 88     while(ed!=s)
 89     {
 90         edge[p[ed]].flow+=big;
 91         edge[p[ed]^1].flow-=big;
 92         ed=edge[p[ed]].from;
 93     }
 94     return big; //找到的流
 95 }
 96 
 97 int max_flow(int n, int s, int e)   //最大流模板
 98 {
 99     int ans_flow=0;
100     memset(num, 0, sizeof(num));
101     BFS(s, e);       //更新d数组
102     int x=s;
103     memset(cur, 0,sizeof(cur));
104     while(d[s]<n)
105     {
106         if(x==e)
107         {
108             ans_flow+=Augment(s,e);
109             x=s;
110         }
111         int ok=0;
112         for(int i=cur[x]; i<vect[x].size(); i++)
113         {
114             node &e=edge[vect[x][i]];
115             if(e.cap>e.flow && d[x]==d[e.to]+1)
116             {
117                 ok=1;
118                 p[e.to]=vect[x][i]; //记录边号
119                 cur[x]=i;
120                 x=e.to;
121                 break;
122             }
123         }
124         if(!ok)
125         {
126             int m=n-1;
127             for(int i=0; i<vect[x].size(); i++)
128             {
129                 node e=edge[vect[x][i]];
130                 if(e.cap>e.flow)    m=min(m, d[e.to]);
131             }
132             if(--num[d[x]]==0) break;
133             d[x]=m+1;
134             num[d[x]]++;
135             cur[x]=0;
136             if(x!=s)    x=edge[p[x]].from;
137         }
138     }
139     return ans_flow;
140 }
141 
142 
143 int cal(int n, int s, int e)
144 {
145     if(isok(n))    return 1;    //已经满足要求了，不用跑最大流
146     int mcnt=g.size(), ncnt=0;;
147 
148     //奇数度的点需要补边
149     md.clear();
150     for(int i=1; i<=n; i++)    if((chu[i]+ru[i])&1)    md.push_back(i);
151     for(int i=0; i<md.size(); i+=2)
152     {
153         g.push_back(make_pair(md[i],md[i+1]));
154         chu[md[i]]++, ru[md[i+1]]++;
155     }
156 
157     //建图***************
158     memset(vis1,0,sizeof(vis1));
159     for(int i=0; i<g.size(); i++)
160     {
161         int a=g[i].first;
162         int b=g[i].second;
163         int h= i<mcnt? i: -1;
164 
165         if(!vis1[a]) ncnt++;        //统计新图中的点数
166         if(!vis1[b]) ncnt++;
167 
168         add_node(a, b, 1, 0, h);    //这是给定的边
169         add_node(b, a, 0, 0, -1);
170 
171         if(!vis1[a]&&chu[a]>ru[a])    //添加源点
172         {
173             vis1[a]=1;
174             int cnt=(chu[a]-ru[a])/2;
175             add_node(0, a, cnt, 0, -1);
176             add_node(a, 0, 0, 0, -1);
177 
178         }
179         if(!vis1[b]&&ru[b]>chu[b])    //添加汇点
180         {
181             vis1[b]=1;
182             int cnt=(ru[b]-chu[b])/2;
183             add_node(b, n+1, cnt, 0, -1);
184             add_node(n+1, b, 0, 0, -1);
185         }
186     }
187 
188     //最大流************
189     max_flow(ncnt+2, s, e); //ncnt+2是表示点数
190 
191     //修改一下答案******
192     change();
193 }
194 
195 int main()
196 {
197     freopen("input.txt", "r", stdin);
198     int t, n, m, a, b;
199     cin>>t;
200     while(t--)
201     {
202         scanf("%d%d",&n,&m);
203         init(n+1);
204         for(int i=0; i<m; i++)
205         {
206             scanf("%d %d", &a, &b);
207             chu[a]++, ru[b]++;
208             ans[i]=1;
209             g.push_back(make_pair(a,b));
210         }
211         cal(n, 0, n+1);
212         for(int i=0; i<m; i++)    printf("%d\n",ans[i]);
213     }
214     return 0;
215 }
AC代码
```
附送个数据产生程序（跑完最大流后再测试一下是否满足要求了）：
```
1 #include <bits/stdc++.h>
 2 #define INF 0x7f7f7f7f
 3 #define pii pair<int,int>
 4 #define LL long long
 5 using namespace std;
 6 const int N=101000;
 7 
 8 
 9 int main()
10 {
11     freopen("output.txt", "w+", stdout);
12     int t=100;//例子数
13     int a, b;
14     puts("100");
15     srand(time(0));
16     while(t--)
17     {
18         a=((int)rand())%100+1;//点数
19         b=((int)rand())%min((a*a),3000)+1;//边数
20         printf("\n\n%d %d\n\n", a, b);
21 
22         for(int i=0; i<b; i++)
23         {
24             int u=((int)rand())%a+1;
25             int v=((int)rand())%a+1;
26             printf("%d %d\n",u,v);
27         }
28     }
29     return 0;
30 }
数据产生器
```

