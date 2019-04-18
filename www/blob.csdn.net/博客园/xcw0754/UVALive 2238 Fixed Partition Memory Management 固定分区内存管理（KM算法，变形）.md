# UVALive 2238 Fixed Partition Memory Management 固定分区内存管理（KM算法，变形） - xcw0754 - 博客园
# [UVALive 2238 Fixed Partition Memory Management 固定分区内存管理（KM算法，变形）](https://www.cnblogs.com/xcw0754/p/4734050.html)
题意：目前有一部分可用内存，分为m个大小固定的区域。现有n个程序要执行，每个程序在不同大小的内存中运行所需的时间不同，要使运行完所有程序所耗时最少，问每个程序在哪块区域中从什么时间运行到什么时间，以及运行完所有程序的平均周转时间。
思路：各种记录有点麻烦！
　　m个区域看成m台内存大小为该区域大小的计算机，然后n个程序分别要选择在其中一台计算机中运行。由于运行有先后顺序，这也很影响平均周转时间，所以最极端时，其中某台计算机可能有n个程序要运行，那么每台计算机就得有n个位置供挑选。所以一共有n*m个位置啦，而只有n个程序，所以左边有n个点，右边有n*m个点，这样可以用KM算法求最佳匹配（必须最佳），而因为我们要使得周转时间少，所以建图时可以将边取相反数。
　　例如一台计算机：设有k个程序运行在该机，则其运行时间分别为t1,t2……tk，则第i个程序结束时间Ti=t1+t2+……+ti （因为它得等前面的人运行完），则此机器上所有程序运行时间之和为sum=k*t1 + (k-1)*t2 + (k-2)*t3 + …… + 1*tk。对于倒数第p个执行的程序c来说，其对总运行时间的贡献为p*tc（tc该程序在该机器中所需的运行时间）。其他机器也是这样的。
　　那么可以建图了，如果程序c能在该机器x上运行，必定有一个运行时间t，而c可能排在任意一个位置，所以c应该有边连到机器x上的任意一个倒数为p的位置，权值为-t*p。这样子求KM的最佳匹配就行了。记得将点数少的放在左边的集合中。
　　输出才是个技术活！要特别注意的是大小的问题，n<=50，m<=10。
```
1 #include <bits/stdc++.h>
  2 #define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
  3 #define MIN(X,Y) ((X) < (Y) ? (X) : (Y))
  4 #define pii pair<int,int>
  5 #define INF 0x7f7f7f7f
  6 #define LL long long
  7 using namespace std;
  8 const int N=60;
  9 const int M=20;
 10 vector<int> vect[N];
 11 struct node
 12 {
 13     int from, to, w;
 14     node(){};
 15     node(int from,int to,int w ):from(from),to(to),w(w){};
 16 }edge[N*M*M];
 17 
 18 struct node1
 19 {
 20     int region;         //第几个区域
 21     int countdown;      //倒数第几
 22     node1(){};
 23     node1(int re,int cd ):region(re),countdown(cd){};
 24 }pos[N*M] ;
 25 
 26 int edge_cnt, pos_cnt;
 27 int m, n;
 28 int msize[N];  //m块区域
 29 int k[N], req[N][M][2];  //m个程序的要求
 30 
 31 
 32 void add_edge(int from,int to,int w)
 33 {
 34     edge[edge_cnt]=node(from, to, w);
 35     vect[from].push_back(edge_cnt++);
 36 }
 37 void add_pos(int r,int cd)
 38 {
 39     pos[pos_cnt]=node1(r,cd);
 40     pos_cnt++;
 41 }
 42 
 43 int Lx[N], Ly[N*M], slack[N*M];
 44 int girl[N*M], S[N], T[N*M];
 45 int get_time(int p,int siz)       //在reg区域找到能运行siz大小内存的时间
 46 {
 47     int t=0;
 48     for(int r=1; r<=k[p]; r++)
 49     {
 50         if( siz>=req[p][r][0] )    t=req[p][r][1];
 51         else break;
 52     }
 53     return t;
 54 }
 55 int last[N][M];
 56 
 57 void build()
 58 {
 59     //共n*m个位置.
 60     for(int j=1; j<=m; j++) //对于每块区域
 61         for(int r=1; r<=n; r++) //每个位置(倒数的)
 62             add_pos(j, r);
 63 
 64 
 65     for(int i=1; i<=n; i++)     //每个程序
 66     {
 67         for(int j=1; j<=m; j++) //每块区域
 68         {
 69             int t=get_time(i, msize[j] );
 70             if(!t)  continue;   //此程序不可在此区域运行。
 71             for(int r=1; r<=n; r++) //每个位置
 72                 add_edge(i, (j-1)*n+r, -t*r );
 73             last[i][j]=t;           //记录i程序在第j区域中的运行时间
 74         }
 75     }
 76 }
 77 
 78 
 79 bool DFS(int x)
 80 {
 81     S[x]=true;
 82     for(int j=0; j<vect[x].size(); j++)
 83     {
 84         node &e=edge[vect[x][j]];
 85         if(T[e.to]) continue;
 86 
 87         int tmp=Lx[x] +Ly[e.to] -e.w;
 88         if(tmp==0)
 89         {
 90             T[e.to]=true;
 91             if(!girl[e.to] || DFS( girl[e.to] ) )
 92             {
 93                 girl[e.to]=x;
 94                 return true;
 95             }
 96         }
 97         else if(slack[e.to]>tmp)    slack[e.to]=tmp;
 98     }
 99     return false;
100 }
101 
102 
103 void KM()    //用n个程序，匹配n*m个位置
104 {
105     //初始化
106     memset(girl, 0, sizeof(girl));
107     memset(Ly, 0, sizeof(Ly));  //数量比较大
108     for(int i=1; i<=n; i++)
109     {
110         Lx[i]=-INF;
111         for(int j=0; j<vect[i].size(); j++) //n*m个位置要匹配
112         {
113             node &e=edge[vect[i][j]];
114             Lx[i]=max(Lx[i], e.w ); //取最大的一条边
115         }
116     }
117 
118     for(int i=1; i<=n; i++)
119     {
120         for(int j=n*m; j>0; j--) slack[j]=INF;
121         while(true)
122         {
123             memset(T, 0, sizeof(T));
124             memset(S, 0, sizeof(S));
125             if(DFS(i))  break;
126 
127             //找最小slack
128             int d=INF;
129             for(int j=1; j<pos_cnt; j++)
130                 if(!T[j] && d>slack[j])
131                     d=slack[j];
132 
133             //更新S
134             for(int j=1; j<=n; j++)
135                 if(S[j])    Lx[j]-=d;
136 
137             //更新T
138             for(int j=1; j<pos_cnt; j++)
139                 if(T[j])  Ly[j]+=d;
140                 else      slack[j]-=d;
141         }
142     }
143 }
144 
145 inline int cmp(pii a, pii b){return a.second>b.second;}
146 int program[N][3];
147 vector<pii> region[M];
148 void print(int Case)
149 {
150     memset(program, 0 , sizeof(program));
151     for(int i=0; i<=m; i++) region[i].clear();
152 
153     for(int i=1; i<pos_cnt; i++)    //将匹配对的信息绑定在一起
154         if(girl[i]!=0)
155             region[ pos[i].region ].push_back(make_pair( girl[i],  pos[i].countdown ));
156 
157     for(int i=1; i<=m; i++)         //排个序，先执行的在前
158         sort(region[i].begin(), region[i].end(), cmp);
159 
160     double sum=0.0;
161     for(int i=1; i<=m; i++) //计算执行的时间
162     {
163         int now=0;
164         for(int j=0; j<region[i].size(); j++)
165         {
166 
167             pii a=region[i][j];
168             program[a.first][0]=i;
169 
170             program[a.first][1]=now;
171             now+=last[a.first][i];       //所需要的时间呢？
172             program[a.first][2]=now;
173 
174             sum+=now;   //累加时间
175         }
176     }
177 
178     if(Case>1)  printf("\n");
179     printf("Case %d\n", Case);
180     printf("Average turnaround time = %.2f\n", sum/n);
181     for(int i=1; i<=n; i++)
182     {
183         printf("Program %d runs in region %d from %d to %d\n", i, program[i][0], program[i][1], program[i][2] );
184     }
185 }
186 
187 int main()
188 {
189     freopen("input.txt", "r", stdin);
190     int Case=0;
191     while(scanf("%d%d",&m,&n), n+m)
192     {
193         edge_cnt=pos_cnt=1;
194         for(int i=0; i<=n; i++) vect[i].clear();
195 
196 
197         for(int i=1; i<=m; i++)    scanf("%d", &msize[i]);   //m个区域的大小
198         for(int i=1; i<=n; i++) //程序i的内存要求，及时间。
199         {
200             scanf("%d", &k[i]);
201             for(int j=1; j<=k[i]; j++)
202                 scanf("%d %d", &req[i][j][0], &req[i][j][1]);
203         }
204         build();    //建图
205         KM();    //最佳匹配
206         print(++Case);//输出
207     }
208     return 0;
209 }
AC代码
```

