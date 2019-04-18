# HDU 3749 Financial Crisis 经济危机（点双连通分量） - xcw0754 - 博客园
# [HDU 3749 Financial Crisis 经济危机（点双连通分量）](https://www.cnblogs.com/xcw0754/p/4617176.html)
题意：
　　给一个图n个点m条边（不一定连通），接下来又q个询问，询问两个点是为“不相连”，“仅有一条路径可达”，“有两条及以上的不同路径可达”三种情况中的哪一种。注：两条以上的路径指的是路径上的点连1个点也不重复。
思路：并查集+tarjan求割点。
　　（1）情况一：先并查集处理，如果两个点从一开始就不连通，直接输出zero
　　（2）情况二和情况三：两点既然连通，那么可能是只有1条路径，比如中间隔着一个割点；也可能有多条路径，比如在同一个双连通分量内。那么直接判断其是否在同一个双连通分量内即可，若在同一个双连通分量内，那么路径起码有2条不重复的。
　　并查集算法看其他题目。
　　tarjan用刘汝佳那个版本。只是我用了unordered_set来存储双连通分量，对于每个询问就在每个双连通分量中查是否同时存在（因为其中一个可能是割点），若同时存在且size>2才是输出 two or more。
　　具体实现看代码。
```
1 #include <bits/stdc++.h>
  2 using namespace std;
  3 const int N=5000+5;
  4 stack< pair<int,int> >  stac;   //tarjan用的栈
  5 vector<int> vect[N];            //图
  6 unordered_set<int>  bi[N];      //双连通分量
  7 
  8 int pre[N];            //并查集用的，保存领导
  9 
 10 int find(int x)        //查
 11 {
 12     return pre[x]==x? x: pre[x]=find(pre[x]);
 13 }
 14 
 15 void joint(int x,int y)  //并
 16 {
 17     x=find(x);
 18     y=find(y);
 19     if(x!=y)    pre[x]=y;
 20 }
 21 
 22 
 23 int dfn[N], low[N];
 24 int bccno[N];
 25 int cnter, bcc_cnt;
 26 
 27 void DFS(int x, int far)
 28 {
 29     dfn[x]= low[x]= ++cnter;
 30     int chd=0;  //孩子
 31     for(int i=0; i<vect[x].size(); i++)
 32     {
 33         int t=vect[x][i];
 34 
 35         if(!dfn[t]) //树边
 36         {
 37             chd++;
 38             stac.push(make_pair(x,t));      //进栈
 39             DFS(t,x);
 40             low[x]=min(low[x],low[t]);
 41             //if((far&&low[t]>=dfn[x]) || (!far&&chd>1) )      //割点
 42             if(low[t]>=dfn[x] )      //割点。如果根只有0或1个孩子呢？那根就不是连通分量中的一个点。
 43             {
 44                 bi[++bcc_cnt].clear(); //连通分量
 45                 while(1)
 46                 {
 47                     int a=stac.top().first;
 48                     int b=stac.top().second;
 49                     stac.pop();
 50                     if(bccno[a]!=bcc_cnt)
 51                     {
 52                         bccno[a]=bcc_cnt;
 53                         bi[bcc_cnt].insert(a);
 54                     }
 55                     if(bccno[b]!=bcc_cnt)
 56                     {
 57                         bccno[b]=bcc_cnt;
 58                         bi[bcc_cnt].insert(b);
 59                     }
 60                     if(a==x && b==t)  break;
 61                 }
 62             }
 63         }
 64         else if(dfn[t]<dfn[x] && t!=far)
 65         {
 66             stac.push(make_pair(x,t));      //进栈
 67             low[x]=min(low[x],dfn[t]);
 68         }
 69     }
 70 }
 71 
 72 int cal_bcc(int n)
 73 {
 74     memset(bccno,0,sizeof(bccno));
 75     memset(dfn,0,sizeof(dfn));
 76     memset(low,0,sizeof(low));
 77 
 78     while(!stac.empty())    stac.pop();
 79 
 80     bcc_cnt= cnter=0;
 81     for(int i=n; i>0; i--)
 82         if(!dfn[i])    DFS(i,0);
 83 }
 84 
 85 
 86 int check(int n,int a, int b)
 87 {
 88     if(find(a)!=find(b))    return 0;   //不连通
 89 
 90     for(int i=1; i<=bcc_cnt; i++)
 91     {
 92         if(bi[i].find(a)!=bi[i].end() && bi[i].find(b)!=bi[i].end() && bi[i].size()>2)  //对于只有两个点的双连通分量，仅有1条边，输出one。
 93         {
 94             printf("two or more\n");
 95             return 1;
 96         }
 97     }
 98     printf("one\n");
 99     return 1;
100 }
101 
102 void init(int n)
103 {
104     memset(pre,0,sizeof(pre));
105     for(int i=1; i<=n; i++) vect[i].clear();
106     for(int i=1; i<=n; i++) pre[i]=i;
107 }
108 
109 int main()
110 {
111     freopen("input.txt", "r", stdin);
112     int n, m, q, a, b, j=0;
113     while(scanf("%d%d%d",&n, &m, &q), n+m+q)
114     {
115         init(n);
116         for(int i=0; i<m; i++)
117         {
118             scanf("%d%d", &a, &b);
119             vect[++a].push_back(++b);
120             vect[b].push_back(a);
121             joint(a,b);             //并查集
122         }
123 
124         cal_bcc(n);                 //tarjan算法
125         printf("Case %d:\n",++j);
126 /*
127         for(int i=1; i<=bcc_cnt; i++)     //输出每个双连通分量。
128         {
129             printf("第%d个双连通分量包含以下点:",i);
130             for(unordered_set<int>::iterator it=bi[i].begin(); it!=bi[i].end();it++)
131             {
132                 printf("%d ",*it);
133             }
134             printf("\n");
135         }
136 */
137         for(int i=0; i<q; i++)
138         {
139             scanf("%d%d", &a, &b);
140             if(!check(n, ++a, ++b)) printf("zero\n");
141         }
142     }
143     return 0;
144 }
AC代码
```

