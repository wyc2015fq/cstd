# HDU 4635 Strongly connected（强连通分量，变形） - xcw0754 - 博客园
# [HDU 4635 Strongly connected（强连通分量，变形）](https://www.cnblogs.com/xcw0754/p/4628700.html)
题意：给出一个有向图（不一定连通），问最多可添加多少条边而该图仍然没有强连通。
思路：
强连通分量必须先求出，每个强连通分量包含有几个点也需要知道，每个点只会属于1个强连通分量。
　　在使图不强连通的前提下，要添加尽可能多的边。边至多有n*(n-1)条，而已经给了m条，那么所能添加的边数不可能超过k=n*(n-1)-m。
这k条边还有部分不能添加，一添加立刻就强连通。一个强连通图最少只需要n条边，根据强连通的特性，缩点之后必定是不会有环的存在的，那么只要继续保持没有环的存在即可。我们只要让其中1个强连通分量没有出度，或者没有入度就行了。到底选哪个强连通分量？选点数少的，这点在后面会了解。因为1*10=10，而2*9=18啊，也就是两个数越接近，两者之积越大。
　　如果挑出1个出度（入度也行的，同理）为0的强连通分量，该强连通分量的点数为d，那么答案就是k-d*(n-d)，即不允许有其他n-d个点有任何一条边指向此强连通分量，其他边都是可以存在的。那么要使得k-d*(n-d)最大，那么d*(n-d)要最小，就是上面讲的要选哪个强连通分量的原因。
　　那么应该找到缩点后出/入度为0的且包含点数最少的强连通分量来计算即可。
```
1 #include <bits/stdc++.h>
 2 #define LL long long
 3 #define pii pair<int,int>
 4 using namespace std;
 5 const int N=100000+5;
 6 const int INF=0x7f7f7f7f;
 7 vector<int> vect[N];
 8 stack<int> stac;
 9 int num[N], chu[N], ru[N];         //统计强连通分量中点的个数
10 int scc_no[N], lowlink[N], dfn[N], dfn_clock, scc_cnt;  //SCC必备
11 int n, m;
12 unordered_map<int,int>  mapp;
13 void DFS(int x) //常规tarjan
14 {
15     stac.push(x);
16     dfn[x]=lowlink[x]=++dfn_clock;
17     for(int i=0; i<vect[x].size(); i++)
18     {
19         int t=vect[x][i];
20         if(!dfn[t])
21         {
22             DFS(t);
23             lowlink[x]=min(lowlink[x],lowlink[t]);
24         }
25         else if(!scc_no[t]) //如果t不属于任何一个强连通分量
26             lowlink[x]=min(lowlink[x],dfn[t]);
27     }
28     if(lowlink[x]==dfn[x])
29     {
30         scc_cnt++;
31         while(true)
32         {
33             int t=stac.top();
34             stac.pop();
35             scc_no[t]=scc_cnt;
36             //scc[scc_cnt].push_back(t);
37             if(t==x)    break;
38         }
39     }
40 }
41 
42 
43 LL cal()
44 {
45     memset(dfn,0,sizeof(dfn));
46     memset(lowlink,0,sizeof(lowlink));
47     memset(scc_no,0,sizeof(scc_no));
48 
49     dfn_clock=scc_cnt=0;
50     for(int i=1; i<=n; i++)    if(!dfn[i])    DFS(i);   //深搜
51     if(scc_cnt==1)  return -1;  //已经强连通
52 
53     memset(chu,0,sizeof(chu));
54     memset(ru,0,sizeof(ru));
55     for(int i=1; i<=n; i++) //统计出入度数量
56         for(int j=0; j<vect[i].size(); j++)
57             if(scc_no[i]!=scc_no[vect[i][j]] )  chu[scc_no[i]]=ru[scc_no[vect[i][j]]]=true;
58 
59     memset(num,0,sizeof(num));
60     for(int i=1; i<=n; i++)    num[scc_no[i]]++;    //统计每个scc中的点的数目
61     int small=INF;
62     for(int i=1; i<=scc_cnt; i++)    if(!chu[i]||!ru[i])   small=min(small,num[i]);//挑量小的，只要满足出/入度为0。如果都已经有出和入度了，那还计算啥！
63     return (LL)n*(n-1) - m - small*(n-small);   //注意10万*10万
64 }
65 
66 
67 int main()
68 {
69     freopen("input.txt", "r", stdin);
70     int a, b, t, j=0;
71     cin>>t;
72     while(t--)
73     {
74         scanf("%d%d",&n,&m);
75         mapp.clear();
76         int up=0;
77         for(int i=1; i<=n; i++) vect[i].clear();
78         for(int i=0; i<m; i++)
79         {
80             scanf("%d%d", &a, &b);
81             if(!mapp[a])    mapp[a]=++up;   //只是怕点号不连续，好像多余了
82             if(!mapp[b])    mapp[b]=++up;
83             vect[mapp[a]].push_back(mapp[b]);
84         }
85         printf("Case %d: %lld\n",++j,cal());
86     }
87     return 0;
88 }
AC代码
```

