# SPOJ  COT2  Count on a tree II （树上莫队，倍增算法求LCA） - xcw0754 - 博客园
# [SPOJ  COT2  Count on a tree II （树上莫队，倍增算法求LCA）](https://www.cnblogs.com/xcw0754/p/4763804.html)
题意：给一个树图，每个点的点权（比如颜色编号），m个询问，每个询问是一个区间[a,b]，图中两点之间唯一路径上有多少个不同点权（即多少种颜色）。n<40000，m<100000。
思路：无意中看到树上莫队，只是拿来练练，没有想到这题的难点不在于树上莫队，而是判断LCA是否在两点之间的路径上的问题。耗时1天。
　　树上莫队的搞法就是：
　　（1）DFS一次，对树进行分块，分成sqrt(n)块，每个点属于一个块。并记录每个点的DFS序。
　　（2）将m个询问区间用所属块号作为第一关键字，DFS序作为第二关键字进行排序。
　　（3）转移都是差不多的，靠具体问题分析转移方式。
　　这题的搞法：
　　（1）点权可能过大，但是只有4万点权，所以映射到1~4w进行处理。
　　（2）DFS对树进行分块，标记dfs序，记录每个点的深度（倍增用）。
　　（3）LCA预处理，以及求LCA的函数。
　　（4）对m个询问进行排序。
　　（5）莫队开始搞起。
　　如何转移（抄别人的）：
　　（1）定义S(u,v)为u−v路径上的顶点集合，root表示根节点。
　　（2）S(u,v)=S(root,u)△S(root,v)△lca(u,v) (△表示集合中的对称差，相当于xor)
　　（3）定义T(u,v)=S(root,u)△S(root,v)，我们先不管lca的事情
　　（4）如果我们从u−v的路径变成u−v′的路径的话对答案有什么影响呢？
　　（5）根据定义我们可以得到T(u,v′)=S(root,u)△S(root,v′)
　　（6）T(u,v)△T(u,v′)=**S(root,u)△S(root,v)**△**S(root,u)△S(root,v′)**=S(root,v)△S(root,v′)=T(v,v′)。
　　也就是说，(a,b)要转移到(a,c)的话，将a点固定不动，b点逐个点走到c点就行了，肯定是将b走到Lca，c也走到Lca就行了。假设(a,b)路径上的点已经作了标记，只需要在走的时候，(b,c)路径上有标记的点就去掉，无标记的就加标记。
　　这样的做法理论上很容易理解，想起来不简单。因为当你走过a,b,c其中两个点的Lca时，你得想清楚这个Lca究竟要不要，而一共有3个Lca可以玩。当然如果某个点在(a,c)路径上，那么肯定是要的，所以任务就是判断这3个Lca是否在这段路径上。这才是难点！
　　虽然有很多种情况，但归类为3种：（1）单纯缩短 （2）单纯伸长 （3）先缩短+再伸长。
　　只要分这3类处理就OK了。而LCA(a,c)肯定是要的，这就可以利用了，判断LCA(b,c)是否在(a,c)上，可以根据从c走到LCA(b,c)遍历的点的顺序，是先到达LCA(a,c)还是LCA(b,c)？如果先到达LCA(b,c)，后达LCA(a,c)，则LCA(b,c)就要，否则就不要。同理这样判断LCA(a,b)要不要。
3500ms+
```
1 #include <bits/stdc++.h>
  2 #define pii pair<int,int>
  3 #define INF 0x3f3f3f3f
  4 #define LL long long
  5 using namespace std;
  6 const int N=40010;
  7 
  8 struct Que
  9 {
 10     int L, R, pos, ans;
 11     Que(){};
 12     Que(int L,int R,int pos):L(L),R(R),pos(pos){ans=0;};
 13 };
 14 int belongto[N], dfn[N], pre[N], stac[N], w[N];
 15 int depth[N], anc[N][32], inp[N], cnt, bit[101000];
 16 int n, m, block, block_cnt, stac_top, dfn_clock, up;
 17 vector<int> vect[N];
 18 vector<Que> que;
 19 
 20 void add_edge(int from,int to)
 21 {
 22     vect[from].push_back(to);
 23     vect[to].push_back(from);
 24 }
 25 inline int cmp(Que a,Que b) //第一关键字：块。第二关键字：DFS时间戳。
 26 {
 27     if(belongto[a.L]==belongto[b.L])    return dfn[a.R]<dfn[b.R];
 28     return belongto[a.L] < belongto[b.L];
 29 }
 30 
 31 inline int cmp1(Que a,Que b){return  a.pos<b.pos;}
 32 
 33 
 34 void DFS(int x)     //树的分块。
 35 {
 36     dfn[x]=++dfn_clock; //dfs序
 37     int cur=stac_top;
 38     for(int i=0; i<vect[x].size(); i++)
 39     {
 40         int t=vect[x][i];
 41         if(!dfn[t])
 42         {
 43             pre[t]=x;
 44             depth[t]=depth[x]+1;        //深度
 45             DFS(t);
 46             if(stac_top-cur >= block)  //够block个就组
 47             {
 48                 block_cnt++;
 49                 while(--stac_top>=cur)
 50                 {
 51                     int p=stac[stac_top];
 52                     belongto[p]=block_cnt;
 53                 }
 54                 stac_top++;             //栈顶必须为空
 55             }
 56         }
 57     }
 58     stac[stac_top++]=x;//栈
 59 }
 60 
 61 void pre_lca(int n) //倍增
 62 {
 63     memset(anc, 0, sizeof(anc));
 64     for(int i=1; i<=n; i++) anc[i][0]=pre[i];   //0是他们自己的父亲。
 65 
 66     for(int k=1; (1<<k)<=n; k++)    //第2的k次方个父亲。
 67     {
 68         for(int i=1; i<=n; i++)
 69         {
 70             if(anc[i][k-1])
 71             {
 72                 int a=anc[i][k-1];
 73                 anc[i][k]=anc[a][k-1];
 74             }
 75         }
 76     }
 77 }
 78 
 79 int LCA(int a,int b)
 80 {
 81     //提到同层
 82     if(depth[a]<depth[b])   swap(a, b);
 83     int log;
 84     for(log=1; (1<<log)<=depth[a]; log++ );
 85     log--;
 86 
 87     for(int i=log; i>=0; i-- )      //一定要从大到小
 88         if( depth[a]-(1<<i)>=depth[b] )
 89             a=anc[a][i];
 90 
 91     if(a==b)    return a;   //b就是lca
 92 
 93 
 94     for(int i=log; i>=0; i--) //同时往上提。
 95     {
 96         if(anc[a][i] && anc[a][i]!=anc[b][i] )
 97         {
 98             a=anc[a][i];
 99             b=anc[b][i];
100         }
101     }
102     return pre[a];
103 }
104 
105 
106 void deal(int x,int d)    //将值异或一下。
107 {
108     if(d==1)    //加上1个
109     {
110         if(bit[x])  bit[x]+=1;
111         else        bit[x]=1,cnt++;
112     }
113     else        //减去1个
114     {
115         bit[x]--;
116         if(bit[x]==0)   cnt--;
117     }
118 
119 }
120 
121 void backtofar(int ed,int lca)   //但是lca不碰
122 {
123     while( ed!=lca  )
124     {
125         inp[ed]*=-1;
126         deal(w[ed], inp[ed]);
127         ed=pre[ed];
128     }
129 }
130 void biyao(int x,int b)   //保证x在inpath上的状态必为b
131 {
132     if(inp[x]!=b)
133     {
134         inp[x]*=-1;
135         deal(w[x], inp[x]);
136     }
137 }
138 
139 int update(int L,int s,int e,int lca,int lca2,int lca3) //将s转移到e
140 {
141     if(inp[e]==1)   //第一种:缩短
142     {
143         backtofar(s, lca);
144         backtofar(e, lca);
145         biyao(s, -1);
146         biyao(lca, -1); //s和e的LCA一定不要，若L到e经过了，那就肯定是他们的LCA，再补回来。
147     }
148     else            //其他两种
149     {
150         int ed, s_e=INF,L_s=INF, L_e=INF, Clock=1; //靠flag判断是：（1）伸长  （2）缩短+伸长
151         ed=s;
152         while( ed!=lca )  //lca先不碰
153         {
154             if(ed==lca)  s_e=Clock;
155             if(ed==lca3) L_s=Clock;
156             inp[ed]*=-1;    //取反
157             deal(w[ed], inp[ed]);
158             ed=pre[ed];
159             Clock++;
160         }
161 
162         ed=e;
163         int s_e1=INF;
164         while( ed!=lca )  //lca先不碰
165         {
166             if(ed==lca)  s_e1=Clock;
167             if(ed==lca2) L_e=Clock;
168             inp[ed]*=-1;    //取反
169             deal(w[ed], inp[ed]);
170             ed=pre[ed];
171             Clock++;
172         }
173         if(ed==lca)  s_e1=Clock;
174         if(ed==lca2) L_e=Clock;
175 
176         if(L_s>=s_e)     biyao(lca3, -1);
177         if(s_e1>=L_e)    biyao(lca, -1);
178 
179         if(L_s<s_e)     biyao(lca3, 1);
180         if(s_e1<L_e)    biyao(lca, 1);
181     }
182     biyao(L, 1);
183     biyao(e, 1);
184     biyao(lca2, 1);
185     return cnt;
186 }
187 
188 void cal(int n,int m)
189 {
190     for(int i=1; i<=n; i++) inp[i]=-1;  //初始，无妨问过.
191     DFS(1);             //分块
192     while(stac_top>=0)        //可能有余下的点，另开新块
193     {
194         int p=stac[stac_top--];
195         belongto[p]=block_cnt;
196     }
197     pre_lca(n);             //倍增处理lca
198     sort(que.begin(), que.end(), cmp);
199 
200     int ans=0, L=1, R=1;    //先将L和R随便弄到一个区间上，比如(1,1)。
201     cnt=inp[1]=bit[w[1]]=1;
202     for(int i=0; i<que.size(); i++) //莫队
203     {
204         if(R!=que[i].R) ans=update(L, R, que[i].R, LCA(R, que[i].R), LCA(L,que[i].R), LCA(L,R) );        //左
205         R=que[i].R;
206         if(L!=que[i].L) ans=update(R, L, que[i].L, LCA(L, que[i].L), LCA(R,que[i].L), LCA(L,R) );        //右
207         L=que[i].L;
208         que[i].ans=ans;
209     }
210     sort(que.begin(), que.end(), cmp1);     //输出
211     for(int i=0; i<que.size(); i++)    printf("%d\n", que[i].ans);
212 }
213 
214 map<int,int> mapp;
215 void init()
216 {
217     block=sqrt(n);
218     dfn_clock=block_cnt=stac_top=up=0;
219     mapp.clear();
220     que.clear();
221     for(int i=0; i<=n; i++)    vect[i].clear();
222     memset(bit, 0, sizeof(bit));
223     memset(dfn, 0, sizeof(dfn));
224     memset(pre, 0, sizeof(pre));
225     memset(depth, 0, sizeof(depth));
226 }
227 int main()
228 {
229     freopen("input.txt", "r", stdin);
230     int  a, b, t;
231     while(cin>>n>>m)
232     {
233         init();
234         for(int i=1; i<=n; i++)
235         {
236             scanf("%d", &t);        //点权
237             if(mapp[t]==0)  mapp[t]=++up;   //映射为小一点的值。
238             w[i]=mapp[t];
239         }
240         for(int i=1; i<n; i++)      //树边
241         {
242             scanf("%d%d",&a,&b);
243             add_edge(a, b);
244         }
245         for(int i=0; i<m; i++)      //询问
246         {
247             scanf("%d%d",&a,&b);
248             que.push_back(Que(a,b,i));
249         }
250         cal(n,m);
251     }
252     return 0;
253 }
AC代码
```

