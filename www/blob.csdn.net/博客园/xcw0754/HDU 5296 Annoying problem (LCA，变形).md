# HDU 5296 Annoying problem (LCA，变形) - xcw0754 - 博客园
# [HDU 5296 Annoying problem (LCA，变形)](https://www.cnblogs.com/xcw0754/p/4667347.html)
题意：
　　给一棵n个节点的树，再给q个操作，初始集合S为空，每个操作要在一个集合S中删除或增加某些点，输出每次操作后：要使得集合中任意两点互可达所耗最小需要多少权值。（记住只能利用原来给的树边。给的树边已经有向。10万个点，10万个操作）
思路：只能用 O(nlogn)的复杂度。官方题解：
![](https://images0.cnblogs.com/blog2015/641737/201507/221415257405547.png)
　　重点也就是要找到集合S中的以x和y为端点一条链，使得操作点u到达这条链是最近的。删除也是这样，找到这条链，删除u到这链的路长。
步骤：
　　（1）记录从根遍历的DFS序。
　　（2）计算每个点到根的路径所经过边的权之和。
　　（3）对于每个操作，无论哪种，如果u的序最大，那么在比u的序小的里面挑一个最大x和一个最小y。再按照式子就可算出，如果是插入就加上这个权和，否则就是减去。
```
1 #include <bits/stdc++.h>
  2 #define LL long long
  3 #define pii pair<int,int>
  4 #define INF 0x7f7f7f7f
  5 using namespace std;
  6 const int N=100100;
  7 vector<int> chld[N];    //孩子
  8 unordered_map<int,int> weight[N];
  9 int dis[N];
 10 int mapp[N];
 11 int seq[N];          //seq记录DFS序
 12 int anti_seq[N];    //anti记录第几个访问的是谁
 13 bool inset[N];
 14 int ans, num;
 15 set<int> sett;
 16 
 17 void DFS(int x)
 18 {
 19     seq[x]=++num;
 20     for(int i=0; i<chld[x].size(); i++)
 21     {
 22         int t=chld[x][i];
 23         if(!seq[t])    DFS(t);
 24     }
 25 }
 26 
 27 unordered_map<int,int> vis;
 28 int LCA(int a,int b)
 29 {
 30     vis.clear();
 31     while(mapp[a])
 32     {
 33         vis[a]=1;
 34         a=mapp[a];
 35     }
 36     vis[a]=1;
 37     while( !vis[b] )    b=mapp[b];
 38     return b;
 39 }
 40 
 41 void cal_weight(int n)
 42 {
 43     for(int i=1; i<=n; i++)
 44     {
 45         int sum=0, s=i;
 46         while(mapp[s])
 47         {
 48             sum+=weight[mapp[s]][s];
 49             s=mapp[s];
 50         }
 51         dis[i]=sum;
 52     }
 53 }
 54 
 55 int ins(int u)
 56 {
 57     sett.insert(seq[u]);
 58     if(sett.size()==1)  {ans=0;return 0;}
 59 
 60     set<int>::iterator it=sett.find(seq[u]);
 61 
 62     int x=0, y=0;
 63     if(*it==*sett.begin())    //已经是最小
 64     {
 65         x=anti_seq[*(++it)];
 66         y=anti_seq[*(sett.rbegin())];
 67     }
 68     else if(*it==*sett.rbegin()) //已经是最大
 69     {
 70         x=anti_seq[*(--it)];
 71         y=anti_seq[*(sett.begin())];
 72     }
 73     else
 74     {
 75         x=anti_seq[ *(--it)];
 76         y=anti_seq[ *(++(++it))  ];
 77     }
 78 
 79     ans+=dis[u]-dis[LCA(u,x )]- dis[LCA(u,y )] + dis[LCA(x,y)];
 80     return ans;
 81 }
 82 
 83 int del(int u)
 84 {
 85     if(sett.size()==2)
 86     {
 87         sett.erase(seq[u]);
 88         ans=0;
 89         return 0;
 90     }
 91 
 92     set<int>::iterator it=sett.find(seq[u]);
 93     int x=0, y=0;
 94     if(*it==*sett.begin())    //已经是最小
 95     {
 96         x=anti_seq[*(++it)];
 97         y=anti_seq[*(sett.rbegin())];
 98     }
 99     else if(*it==*sett.rbegin()) //已经是最大
100     {
101         x=anti_seq[*(--it)];
102         y=anti_seq[*(sett.begin())];
103     }
104     else
105     {
106         x=anti_seq[ *(--it)];
107         y=anti_seq[ *(++(++it))  ];
108     }
109     ans-=dis[u]-dis[LCA(u,x )]- dis[LCA(u,y )] + dis[LCA(x,y)];
110     sett.erase(seq[u]);
111     return ans;
112 }
113 
114 int main()
115 {
116     freopen("input.txt", "r", stdin);
117     int a, b, c, t, q, n, j=0;
118     cin>>t;
119     while(t--)
120     {
121         scanf("%d%d", &n, &q);
122         for(int i=0; i<=n+10; i++)    chld[i].clear(),weight[i].clear();
123 
124         memset(inset, 0, sizeof(inset));
125         memset(seq, 0, sizeof(seq));
126         memset(anti_seq, 0, sizeof(anti_seq));
127         memset(mapp, 0, sizeof(mapp));
128         memset(dis,0,sizeof(dis));
129         sett.clear();
130         ans=0;
131         num=0;
132 
133         for(int i=1; i<n; i++)
134         {
135             scanf("%d%d%d",&a,&b,&c);
136             chld[a].push_back(b);
137             weight[a][b]=c;
138             mapp[b]=a;
139         }
140         printf("Case #%d:\n",++j);
141         int root=n;        //获得树根
142         while(mapp[root])   root=mapp[root];
143         DFS(root);          //获得DFS序
144 
145         for(int i=1; i<=n; i++)      anti_seq[seq[i]]=i;      //反向索引
146         cal_weight(n);        //计算每个点到根的权
147         while(q--)
148         {
149             scanf("%d%d",&a,&b);
150             if(a==1)    //如果不存在于集合中，则插，有则不插
151             {
152                 if(inset[b])    printf("%d\n", ans);  //已经存在，不必计算
153                 else
154                 {
155                     inset[b]=1;
156                     printf("%d\n", ins(b));
157                 }
158             }
159             else    //如果存在于集合中，则删，否则不删。
160             {
161                 if(inset[b])    //存在，要删
162                 {
163                     inset[b]=0;
164                     printf("%d\n",del(b));
165                 }
166                 else    printf("%d\n",ans);
167             }
168         }
169     }
170     return 0;
171 }
AC代码
```

