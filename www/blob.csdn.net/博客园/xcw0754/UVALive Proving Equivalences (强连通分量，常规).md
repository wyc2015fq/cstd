# UVALive Proving Equivalences (强连通分量，常规) - xcw0754 - 博客园
# [UVALive Proving Equivalences (强连通分量，常规)](https://www.cnblogs.com/xcw0754/p/4628520.html)
题意：
　　给一个有向图，问添加几条边可以使其强连通。
思路：
　　tarjan算法求强连通分量，然后缩点求各个强连通分量的出入度，答案是max(入度为0的缩点个数，出度为0的缩点个数)。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 #define pii pair<int,int>
 4 using namespace std;
 5 const int N=20000+5;
 6 const int INF=0x7f7f7f7f;
 7 
 8 vector<int> vect[N];
 9 stack<int> stac;
10 bool chu[N], ru[N];
11 int scc_no[N], lowlink[N], dfn[N];
12 int dfn_clock, scc_cnt;
13 int n, m;
14 
15 void DFS(int x)
16 {
17     stac.push(x);
18     dfn[x]=lowlink[x]=++dfn_clock;
19     for(int i=0; i<vect[x].size(); i++)
20     {
21         int t=vect[x][i];
22         if(!dfn[t])
23         {
24             DFS(t);
25             lowlink[x]=min(lowlink[x],lowlink[t]);
26         }
27         else if(!scc_no[t]) //如果t不属于任何一个强连通分量
28             lowlink[x]=min(lowlink[x],dfn[t]);
29     }
30     if(lowlink[x]==dfn[x])
31     {
32         scc_cnt++;
33         while(true)
34         {
35             int t=stac.top();
36             stac.pop();
37             scc_no[t]=scc_cnt;
38             if(t==x)    break;
39         }
40     }
41 }
42 
43 
44 int cal()
45 {
46     memset(dfn,0,sizeof(dfn));
47     memset(lowlink,0,sizeof(lowlink));
48     memset(scc_no,0,sizeof(scc_no));
49 
50     dfn_clock=scc_cnt=0;
51     for(int i=1; i<=n; i++)    if(!dfn[i])    DFS(i);   //深搜
52     if(scc_cnt==1)  return -1;
53 
54     memset(chu,0,sizeof(chu));
55     memset(ru,0,sizeof(ru));
56     for(int i=1; i<=n; i++)
57         for(int j=0; j<vect[i].size(); j++)  //统计出入度
58             if(scc_no[i]!=scc_no[vect[i][j]])    chu[scc_no[i]]=ru[scc_no[vect[i][j]]]=true;//这里麻烦了点，小心点出错
59 
60     int c=0, r=0;
61     for(int i=1; i<=scc_cnt; i++)
62     {
63         if(!chu[i])    c++;
64         if(!ru[i])     r++;
65     }
66     return max(c,r)-1;
67 }
68 
69 
70 int main()
71 {
72     freopen("input.txt", "r", stdin);
73     int a, b, t;
74     cin>>t;
75     while(t--)
76     {
77         scanf("%d%d",&n,&m);
78         for(int i=1; i<=n; i++) vect[i].clear();
79         for(int i=0; i<m; i++)
80         {
81             scanf("%d%d", &a, &b);
82             vect[a].push_back(b);
83         }
84         printf("%d\n",cal());
85     }
86     return 0;
87 }
AC代码
```

