# UVA 1479 Graph and Queries （Treap） - xcw0754 - 博客园
# [UVA 1479 Graph and Queries （Treap）](https://www.cnblogs.com/xcw0754/p/4759920.html)
题意：
　　给一个无向图，再给一系列操作（以下3种），输出最后的平均查询结果。
（1）D X 删除第x条边。
（2）Q X k  查询与点X相连的连通分量中第k大的点的权值。
（3）C X v  将点X的权值改为v。
思路：
　　第一点，由于需要删除边，不是很方便，所以可以先将所有操作存起来，反序来操作，这样删边就变成加边，方便了不少。每次加边时若两点不属于同个连通分量，就将点少的整个连通分量中的点逐个插入到另一个连通分量中。
　　第二点，查第k大，这个比较简单，只需要维护Treap上每个点的的左右孩子数量就可以了。查的只是某个点所属连通分量中的第k大值，与真实图是怎样的无关。
　　第三点，改权值，这个麻烦。可以先删除该点，再插入改过权的该点。删除方式就是将该点旋转到成为叶子，然后删除它就方便了，因为还需要维护堆的性质，所以往下旋转比较方便的。
坑点：随时要注意更新每个连通分量的根节点，也就是说必须用引用的时候，尽量用引用。在第一点操作时，需要插很多点，每插1个点都可能成为根，所以要随时更新根节点。
```
1 #include <bits/stdc++.h>
  2 #define pii pair<int,int>
  3 #define INF 0x3f3f3f3f
  4 #define LL long long
  5 using namespace std;
  6 const int N=20010;
  7 bool single[N];
  8 int w[N], from[N*3], to[N*3], cut[N*3], pre[N], root[N];
  9 struct opera
 10 {
 11     char type;
 12     int a, b;
 13     opera(char t,int a,int b):type(t),a(a),b(b){};
 14 };
 15 deque<opera> que;
 16 struct node         //Treap
 17 {
 18     int val, pre, rank;  //ran是优先级
 19     int ch[2], son[2];
 20     node(){};
 21     node(int val):val(val)
 22     {
 23         rank=rand();
 24         ch[0]=ch[1]=son[0]=son[1]=pre=0;
 25     };
 26 }nod[N];
 27 
 28 int find(int x){return pre[x]==x? x: pre[x]=find(pre[x]);}
 29 
 30 int Query(int t,int k)  //找第k大
 31 {
 32     if( k<1 || nod[t].son[0] + nod[t].son[1] + 1 < k)     return 0;  //k太小or太大
 33     if( nod[t].son[1]==k-1 )    return nod[t].val;
 34     if( nod[t].son[1]>k-1 )    return  Query(nod[t].ch[1], k);
 35     else       return  Query(nod[t].ch[0], k-nod[t].son[1]-1);
 36 }
 37 
 38 void Rotate(int t, int d)       //d为方向,0是左旋，1是右
 39 {
 40     int far=nod[t].pre;
 41     int son=nod[t].ch[d];       //far的孩子
 42     int gra=nod[far].pre;       //far的父亲
 43 
 44     nod[son].pre=far;
 45     nod[t].pre=gra;
 46     nod[far].pre=t;
 47 
 48     nod[far].ch[d^1]=son;
 49     nod[t].ch[d]=far;
 50     nod[gra].ch[nod[gra].ch[1]==far]=t;
 51 
 52     //子树中的节点要更新
 53     nod[far].son[d^1]=nod[t].son[d];
 54     nod[t].son[d]+=1+nod[far].son[d];   //别忘了还有far也是个节点
 55 
 56     if(gra==0)  root[find(far)]=t;      //更新连通分量的根节点
 57 }
 58 
 59 int get_child(int t)    //返回孩子编号，总是返回rank较大的那个。0表示无孩子
 60 {
 61     if(!nod[t].ch[0] && !nod[t].ch[1])    return 0;
 62     if(nod[t].ch[0] && nod[t].ch[1])
 63     {
 64         int L=nod[t].ch[0], R=nod[t].ch[1];
 65         if(nod[L].rank < nod[R].rank)   return R;
 66         else    return L;
 67     }
 68     if(nod[t].ch[0])    return nod[t].ch[0];
 69     else                return nod[t].ch[1];
 70 }
 71 
 72 void Insert(int root,int t) //将t插入到root中
 73 {
 74     if(nod[t].val<nod[root].val)
 75     {
 76         nod[root].son[0]++;
 77         if(nod[root].ch[0])     Insert(nod[root].ch[0], t);
 78         else    nod[root].ch[0]=t, nod[t].pre=root;
 79     }
 80     else
 81     {
 82         nod[root].son[1]++;
 83         if(nod[root].ch[1])     Insert(nod[root].ch[1], t);
 84         else    nod[root].ch[1]=t, nod[t].pre=root;
 85     }
 86     int son=nod[root].ch[1];
 87     if( nod[son].rank > nod[root].rank )    Rotate(son, 0);  //孩子的rank比我还大
 88 }
 89 void clear(int t,int v) //清空点t，值改为v
 90 {
 91     nod[t].pre=nod[t].ch[0]=nod[t].ch[1]=nod[t].son[0]=nod[t].son[1]=0;
 92     nod[t].val=v;
 93 }
 94 
 95 void Merge(int &root, int t) //递归将t中的节点逐个拆出
 96 {
 97     if(nod[t].ch[0]>0)    Merge(root, nod[t].ch[0]);
 98     if(nod[t].ch[1]>0)    Merge(root, nod[t].ch[1]);
 99     clear(t, nod[t].val);   //值仍不变
100     single[root]=0;         //非单身了
101     single[t]=0;
102     Insert(root, t);        //将t插入到root中
103 }
104 
105 
106 void add_edge(int i)
107 {
108     int u=find(from[i]), v=find(to[i]); //两点所在连通分量
109     if(u!=v)           //这里也会去掉重边
110     {
111         int &uu=root[u], &vv=root[v];   //找到连通分量的树根。注意：必须是引用，有可能每插一次就换根了。
112         if( nod[uu].son[0]+nod[uu].son[1] < nod[vv].son[0]+nod[vv].son[1])  //uu小
113             pre[u]=v, Merge(vv, uu);    //uu拆出来装到vv上面去
114         else
115             pre[v]=u, Merge(uu, vv);
116     }
117 }
118 
119 void change(int t,int v)    //将t的权值改为v
120 {
121     if(single[t]==true){nod[t].val=v;return ;}     //仅有1个点,直接改
122     int far, son, tmp;
123     while( (son=get_child(t))!=0 )    Rotate( son, nod[t].ch[0]==son);    //先把t转到底端
124     tmp=t;
125     while( nod[tmp].pre!=0 )   //调整到根节点的孩子数量,沿途减1。
126     {
127         far=nod[tmp].pre;
128         nod[far].son[nod[far].ch[1]==tmp]--;
129         tmp=far;
130     }
131     far=nod[t].pre;
132     nod[far].ch[ nod[far].ch[1]==t ]=0; //需要做的只是改变父亲的孩子指针。
133     clear(t, v);
134     Insert(root[find(t)], t);   //改完值插到root中。
135 }
136 
137 double cal(int n,int m)
138 {
139     for(int i=1; i<=n; i++)     pre[i]=root[i]=i,nod[i]=node(w[i]);   //初始时，root和连通分量编号都是自己
140     for(int i=1; i<=m; i++)     if(cut[i]==0)    add_edge(i);
141     LL ans=0, cnt=0;
142     while(!que.empty())
143     {
144         opera p=que.front();que.pop_front();
145         if(p.type=='D')    add_edge(p.a);       //加边
146         if(p.type=='Q')    cnt++,ans+=Query(root[find(p.a)], p.b);
147         if(p.type=='C')    change(p.a, p.b);   //改权值
148     }
149     return ans/(double)cnt;
150 }
151 
152 int main()
153 {
154     freopen("input.txt", "r", stdin);
155     int  a, b, c, n, m, Case=0;
156     char op;
157     while(cin>>n>>m, n+m)
158     {
159         memset(single, 1, sizeof(single));
160         memset(cut, 0, sizeof(cut));
161         for(int i=1; i<=n; i++)    scanf("%d", &w[i]);
162         for(int i=1; i<=m; i++)    scanf("%d %d", &from[i], &to[i]);
163 
164         while(cin>>op,op!='E')
165         {
166             if(op=='D'){scanf("%d",&a);cut[a]=1;}             //删边
167             if(op=='Q') scanf("%d%d",&a,&b);                  //查询
168             if(op=='C'){scanf("%d%d",&a,&b),swap(w[a], b);}   //改权
169             que.push_front(opera(op, a, b));
170         }
171         printf("Case %d: %.6f\n", ++Case, cal(n, m));
172     }
173     return 0;
174 }
AC代码
```

