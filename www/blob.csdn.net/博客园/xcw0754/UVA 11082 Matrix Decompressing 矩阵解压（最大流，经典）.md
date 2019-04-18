# UVA 11082 Matrix Decompressing 矩阵解压（最大流，经典） - xcw0754 - 博客园
# [UVA 11082 Matrix Decompressing 矩阵解压（最大流，经典）](https://www.cnblogs.com/xcw0754/p/4646140.html)
题意：
　　知道矩阵的前i行之和，和前j列之和（任意i和j都可以）。求这个矩阵。每个格子中的元素必须在1~20之间。矩阵大小上限20*20。
思路：
　　这么也想不到用网络流解决，这个模型很不错。假设这个矩阵的每一行是水管，每一列是水管，每行有出水口流到每一列，这样想比较好理解。然后每行的流量和每列的流量知道，就可以建图了。
　　建图过程，每行对应一个点，每列对应1个点，每行都可以流到每列，所以他们之间有边。我们得假设他们是如何流向的，不如设从行流向列，那么添加源点，流向每行；添加汇点，被每列汇流。容量怎么设？我们要让每行都满流就行了，那么行之和就是源点到该行的容量，同理汇点也如此。但是每行流向每列呢？注意每个格子的元素必须在1~20之间，所以把容量设为20，别让它流太多了。
　　注意到元素必须在1~20之间！！！那么这样增广路的话会出现有的完全0流怎么办？先将每个格子中的元素自减1，它的流下限总不会为负吧，计算完输出时再加回去不就行了。
```
1 #include <bits/stdc++.h>
  2 #define LL long long
  3 #define pii pair<int,int>
  4 #define INF 0x7f7f7f7f
  5 using namespace std;
  6 const int N=10000+100;
  7 
  8 struct node
  9 {
 10     int from;
 11     int to;
 12     int cap;
 13     int flow;
 14 }edge[N];
 15 
 16 int row[30];
 17 int col[30];
 18 int edge_cnt;
 19 vector<int> vect[1000];
 20 
 21 void add_node(int from,int to,int cap,int flow)
 22 {
 23     edge[edge_cnt].from=from;
 24     edge[edge_cnt].to=to;
 25     edge[edge_cnt].cap=cap;
 26     edge[edge_cnt].flow=flow;
 27     vect[from].push_back(edge_cnt);
 28     edge_cnt++;
 29 }
 30 
 31 void build_graph(int n,int m)
 32 {
 33     for(int i=1; i<=n; i++) //加源点
 34     {
 35         add_node(0, i, row[i], 0);
 36         add_node(i ,0, 0, 0);
 37     }
 38 
 39     for(int i=1; i<=m; i++) //汇点
 40     {
 41         add_node(n+i, n+m+1, col[i], 0);
 42         add_node(n+m+1, n+i, 0, 0);
 43     }
 44 
 45     for(int i=1; i<=n; i++) //行-列
 46     {
 47         for(int j=1; j<=m; j++)
 48         {
 49             add_node(i,n+j,19,0);
 50             add_node(n+j, i,0,0);
 51         }
 52     }
 53 }
 54 
 55 int flow[1000];
 56 int path[1000];
 57 int matrix[50][50];
 58 
 59 int BFS(int s,int e)
 60 {
 61     deque<int> que;
 62     que.push_back(s);
 63     flow[s]=INF;
 64 
 65     while(!que.empty())
 66     {
 67         int x=que.front();
 68         que.pop_front();
 69         for(int i=0; i<vect[x].size(); i++)
 70         {
 71             node e=edge[vect[x][i]];
 72             if(!flow[e.to] && e.cap>e.flow)
 73             {
 74                 path[e.to]=vect[x][i];
 75                 flow[e.to]=min(flow[e.from],e.cap-e.flow);
 76                 que.push_back(e.to);
 77             }
 78         }
 79         if(flow[e]) return flow[e];
 80     }
 81     return flow[e];
 82 }
 83 
 84 int cal(int s, int e)
 85 {
 86     int ans=0;
 87     while(true)
 88     {
 89         memset(flow,0,sizeof(flow));
 90         memset(path,0,sizeof(path));
 91 
 92         int tmp=BFS(s,e);
 93         if(tmp==0)  return ans;
 94 
 95         ans+=tmp;
 96         int ed=e;
 97         while(ed!=s)
 98         {
 99             int t=path[ed];
100             edge[t].flow+=tmp;
101             edge[t^1].flow-=tmp;
102             ed=edge[t].from;
103         }
104 
105     }
106     return 0;
107 }
108 
109 void get_ans(int n,int m)
110 {
111     for(int i=0; i<edge_cnt; i+=2)
112     {
113         node e=edge[i];
114         if(e.to<n)  continue;
115         matrix[e.from][e.to-n]=e.flow;
116     }
117     for(int i=1; i<=n; i++)
118     {
119         for(int j=1; j<=m; j++)
120             printf("%d ", matrix[i][j]+1);
121         printf("\n");
122     }
123 }
124 
125 
126 int main()
127 {
128     //freopen("input.txt", "r", stdin);
129     int r, c, t, k, Case=0;
130     cin>>t;
131     while(t--)
132     {
133         for(int i=0; i<1000; i++)    vect[i].clear();
134         memset(matrix,0,sizeof(matrix));
135         memset(row,0,sizeof(row));
136         memset(col,0,sizeof(col));
137         memset(edge,0,sizeof(edge));
138         edge_cnt=0;
139 
140 
141         scanf("%d%d",&r,&c);
142         for(int i=1; i<=r; i++)  //行
143             scanf("%d",&row[i]);
144 
145         for(int i=1; i<=c; i++)  //列
146             scanf("%d",&col[i]);
147 
148         for(int i=r; i>0; i--)  //换成每行-c
149             row[i]-=row[i-1]+c;//细心点
150         for(int i=c; i>0; i--)  //换成每列-r
151             col[i]-=col[i-1]+r;
152 
153         printf("Matrix %d\n",++Case);
154         build_graph(r, c);
155         cal(0, r+c+1);
156         get_ans(r,c);
157         if(t)   cout<<endl;
158     }
159     return 0;
160 }
AC代码
```

