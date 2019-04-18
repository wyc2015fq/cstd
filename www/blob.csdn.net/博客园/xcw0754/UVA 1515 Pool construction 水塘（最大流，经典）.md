# UVA 1515 Pool construction 水塘（最大流，经典） - xcw0754 - 博客园
# [UVA 1515 Pool construction 水塘（最大流，经典）](https://www.cnblogs.com/xcw0754/p/4656888.html)
题意：
　　给一个h*w的矩阵，每个格子中是'#'和'.'两个符号之一，分别代表草和洞。现在要将洞给围起来（将草和洞分离），每条边需花费b元（即将一个洞包起来需要4边，将2个连续的洞包起来需要6边，省了2条边）。有个特殊能力，能将洞变成草，花费f。当然也能将草变成洞，花费d。围起洞来需要多少花费。矩阵四周最边边的格子都必须是草，即出现洞就必须转草。
　　转换：洞--->草--->洞
　　    　　  f　　   d   　　(元/格)
　　围洞： b元/边。
思路：
　　如果能够尽量让洞都靠在一起，那肯定是最小花费，因为少了很多条边。但是转换并不是免费，如果太贵，还不如不转，直接围。
　　这很像用一条绳子将洞给围起来，然后量绳子的周长。如果将格子看成点，那么就很像一个'割'（即将点分成两个集合）。问题是如何建模？
　　可以这么想，假设草上面都是水，自然就会流进洞里面，那么相邻的如果是草跟洞，则这条边就会有流通过，容量最多是b（相当于绳子的一部分）。这样子每个相邻的格子都需要建2条边互指（当然需要反向边），容量都是b，使得水可以流向洞中，花费就是流量。如果此时跑一次最大流（得加源汇点），就相当于求出不作'草洞'转换的围洞花费了。
　　但是问题是可以转来转去，如果草转洞很便宜，则应多转成洞。这样就需要协调了。水往低处流。我们在源点S到所有边边的草格子建立1条边，容量INF，接着，S连到所有其他的草格子，容量为d。其他的洞格子连到汇点T，容量为f。还有，每个格子和相邻的格子都有2条边，容量是b。新图就建完了，但是为什么这么做？
　　如下图，9个格子，4个草格子（边边被去掉）：
　　. . .
　　.#.
　　###
　　明显，d比较小时，红色那块应该转洞，3段变1段花费。多小合适？d+b<3b时就合适，因为转的费用d，加上流进该新洞的流b，比原来需要3b要少。我们所设的（S-红草）这条边的容量是d，当满流时，也会从'红草'流向3个方向的洞中去，由红草流出的最大流不超过3b。此时，红草下的黑草，设为黑草A，它最多有b的流量可以流到红草喔，但是没办法，红草能承担的上限也只能是3b。若（S-红草）的流量>0，相当于帮忙减小了红草的压力，红草此时只是承受了来自S的d流量，还有黑草A的流量b而已。这样流来流去，红草流出的上限为3b，但是如果S给红草的d太小，即使加上黑草A的b，也是到达不了上限3b的，那么自动就被限流了。这个模型自动就被限流了，好像自动帮我们选择了“最省钱的方式”。同理，（洞-T） 也是这样，流到草格子B的流量欲超过f的话，还不如直接转成草，限流f就相当于自动转成草，如果少于f则相当于不转。
```
1 #include <bits/stdc++.h>
  2 #define LL long long
  3 #define pii pair<int,int>
  4 #define INF 0x7f7f7f7f
  5 using namespace std;
  6 const int N=10000;
  7 char g[100][100];
  8 vector<int> vect[N];
  9 struct node
 10 {
 11     int from, to, cap, flow;
 12     node(){};
 13     node(int a,int b,int c,int d):from(a), to(b), cap(c), flow(d){};
 14 }edge[N*4];
 15 
 16 int edge_cnt;
 17 void add_node(int a,int b,int c,int d)
 18 {
 19     edge[edge_cnt]=node(a,b,c,d);
 20     vect[a].push_back(edge_cnt++);
 21 }
 22 
 23 bool isOk(int x,int y,int h,int w){return x>0&&x<=h&&y>0&&y<=w? 1: 0;}//判断是否在矩阵内
 24 
 25 void build_graph(int h, int w,int b, int d,int f)//建图。
 26 {
 27     for(int i=1; i<=h; i++)
 28     {
 29         for(int j=1; j<=w; j++)
 30         {
 31             if( isOk(i, j-1, h, w) )    //左边
 32             {
 33                 add_node((i-1)*w+j-1, (i-1)*w+j, b, 0);
 34                 add_node((i-1)*w+j, (i-1)*w+j-1, 0, 0);
 35             }
 36             if( isOk(i, j+1 ,h, w) )  //右边
 37             {
 38                 add_node((i-1)*w+j+1, (i-1)*w+j, b, 0);
 39                 add_node((i-1)*w+j, (i-1)*w+j+1, 0, 0);
 40             }
 41             if( isOk(i-1, j, h, w) )  //上边
 42             {
 43                 add_node((i-1)*w+j, (i-2)*w+j, b, 0);
 44                 add_node((i-2)*w+j, (i-1)*w+j, 0, 0);
 45             }
 46             if( isOk(i+1,j, h, w)  )  //下边
 47             {
 48                 add_node((i-1)*w+j, i*w+j, b, 0);
 49                 add_node(i*w+j, (i-1)*w+j, 0, 0);
 50             }
 51 
 52         }
 53     }
 54 
 55     for(int i=1; i<=h; i++)
 56     {
 57         for(int j=1; j<=w; j++)
 58         {
 59             if(i==1||i==h || j==1||j==w)  //边边的草
 60             {
 61                 add_node(0, (i-1)*w+j, INF, 0);
 62                 add_node((i-1)*w+j, 0, 0, 0);
 63             }
 64             else if(g[i][j]=='#')   //源点-草
 65             {
 66                 add_node(0, (i-1)*w+j, d, 0);
 67                 add_node((i-1)*w+j, 0, 0, 0);
 68             }
 69             else    //洞-汇点
 70             {
 71                 add_node((i-1)*w+j, w*h+1, f, 0);
 72                 add_node(w*h+1, (i-1)*w+j, 0, 0);
 73             }
 74         }
 75     }
 76 }
 77 
 78 int flow[N], path[N];
 79 
 80 int BFS(int s,int e)
 81 {
 82     deque<int> que(1,s);
 83     flow[s]=INF;
 84     while(!que.empty())
 85     {
 86         int x=que.front();
 87         que.pop_front();
 88         for(int i=0; i<vect[x].size(); i++)
 89         {
 90             node e=edge[vect[x][i]];
 91             if(!flow[e.to]&&e.cap>e.flow)
 92             {
 93                 path[e.to]=vect[x][i];
 94                 flow[e.to]=min(flow[e.from], e.cap-e.flow);
 95                 que.push_back(e.to);
 96             }
 97         }
 98         if(flow[e]) break;;
 99     }
100     return flow[e];
101 }
102 
103 
104 
105 int max_flow(int s,int e)
106 {
107     int ans_flow=0;
108     while(true)
109     {
110         memset(flow,0,sizeof(flow));
111         memset(path,0,sizeof(path));
112 
113         int tmp=BFS(s, e);
114         if(tmp==0)  return ans_flow;
115         ans_flow+=tmp;
116 
117         int ed=e;
118         while(ed!=s)
119         {
120             int t=path[ed];
121             edge[t].flow+=tmp;
122             edge[t^1].flow-=tmp;
123             ed=edge[t].from;
124         }
125     }
126 }
127 
128 int main()
129 {
130     freopen("input.txt", "r", stdin);
131     int t, w, h, d, f, b;
132     char c;
133     cin>>t;
134     while(t--)
135     {
136         scanf("%d%d %d%d%d", &w, &h, &d, &f,&b);
137         edge_cnt=0;
138         memset(edge,0,sizeof(edge));
139         for(int i=N-1; i>=0; i--)   vect[i].clear();
140 
141         for(int i=1; i<=h; i++)
142         {
143             for(int j=1; j<=w; j++)
144             {
145                 c=getchar();
146                 if(c=='#'||c=='.')    g[i][j]=c;
147                 else    j--;
148             }
149         }
150         int ans=0;
151         //处理掉四周：洞转草花费f元/格
152         for(int i=1; i<=w; i++) if(g[1][i]!='#')    ans+=f; //第一行
153         for(int i=1; i<=w; i++) if(g[h][i]!='#')    ans+=f; //最后一行
154         for(int i=2; i<h; i++) if(g[i][1]!='#')    ans+=f;
155         for(int i=2; i<h; i++) if(g[i][w]!='#')    ans+=f;
156         build_graph(h, w, b, d, f);
157         printf("%d\n",max_flow(0, w*h+1)+ans);
158     }
159     return 0;
160 }
AC代码
```

