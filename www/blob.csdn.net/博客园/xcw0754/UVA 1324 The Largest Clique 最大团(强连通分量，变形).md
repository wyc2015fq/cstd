# UVA 1324 The Largest Clique 最大团(强连通分量，变形) - xcw0754 - 博客园
# [UVA 1324 The Largest Clique 最大团(强连通分量，变形)](https://www.cnblogs.com/xcw0754/p/4630692.html)
题意：给一个有向图，要求找出一些点，使得这些点中的任意点对，要么可以互通，要么单向可达。
思路：最低只要求单向可达即可，即a->b都可以算进去。
　　强连通分量内的点肯定是满足要求的，可以全选，但是有多个强连通分量时就不行了，得有取舍。老方法，先缩点，缩完点后是没有环的存在的，所以就是拓扑图了。如果只给一个拓扑图，要求找一条链使得链上的点最多，那么可以用判断拓扑的方式，逐个将入度为0的点删除，且在删除的时候记录下最多有多少个点，删到最后一个点时就出结果了。这样的方法同样适用，只是每个点可能是缩点，而且要将这些缩点内的点数算上去而已。
实现：
　　（1）求强连通分量。
　　（2）统计缩点的度数并建（缩点）图。
　　（3）按判断拓扑图的方式来进行点数的统计。
```
1 #include <bits/stdc++.h>
  2 #define LL long long
  3 #define pii pair<int,int>
  4 using namespace std;
  5 const int N=1000+5;
  6 const int INF=0x7f7f7f7f;
  7 vector<int> vect[N], g[N];  //原图，缩点后的图
  8 int n, m;
  9 int dfn[N], lowlink[N], scc_no[N], dfn_clock, scc_cnt; //强连通分量必备
 10 stack<int> stac;    //强联通分量用栈
 11 unordered_map<int,int> chu[N],ru[N];    //仅仅为了防止重复统计
 12 int  r[N]; //出入度
 13 int num[N];     //强联通分量中的点数
 14 int dp[N];      //答案
 15 
 16 void DFS(int x)
 17 {
 18     stac.push(x);
 19     dfn[x]=lowlink[x]=++dfn_clock;
 20     for(int i=0; i<vect[x].size(); i++)
 21     {
 22         int t=vect[x][i];
 23         if(!dfn[t])
 24         {
 25             DFS(t);
 26             lowlink[x]=min(lowlink[x],lowlink[t]);
 27         }
 28         else if(!scc_no[t])    lowlink[x]=min(lowlink[x], dfn[t]);
 29     }
 30     if(lowlink[x]==dfn[x])
 31     {
 32         ++scc_cnt;
 33         while(true)
 34         {
 35             int t=stac.top();stac.pop();
 36             scc_no[t]=scc_cnt;
 37             if(t==x)    break;
 38         }
 39     }
 40 }
 41 
 42 
 43 int cal()
 44 {
 45     memset(dfn,0,sizeof(dfn));
 46     memset(lowlink,0,sizeof(lowlink));
 47     memset(scc_no,0,sizeof(scc_no));
 48     dfn_clock=scc_cnt=0;
 49     for(int i=1; i<=n; i++) if(!dfn[i]) DFS(i);
 50 
 51     if(scc_cnt==1)  return n;
 52     for(int i=1; i<=scc_cnt; i++)   g[i].clear(),chu[i].clear(),ru[i].clear();
 53     for(int i=1; i<=n; i++) //统计度，建图
 54     {
 55         for(int j=0; j<vect[i].size(); j++)
 56         {
 57             int t=vect[i][j];
 58             if(scc_no[i]!=scc_no[t])
 59             {
 60                 if(!chu[scc_no[i]][scc_no[t]])   //还没出现过
 61                 {
 62                     chu[scc_no[i]][scc_no[t]]=1;
 63                     g[scc_no[i]].push_back(scc_no[t]);
 64                 }
 65                 ru[scc_no[t]][scc_no[i]]=1;
 66             }
 67         }
 68     }
 69     deque<int> que;
 70     memset(r,0,sizeof(r));
 71     for(int i=1; i<=scc_cnt; i++)   //统计出入度
 72     {
 73         r[i]=ru[i].size();
 74         if(!r[i])   que.push_back(i);
 75     }
 76 
 77     memset(num,0,sizeof(num));
 78     for(int i=1; i<=n; i++)    num[scc_no[i]]++;    //统计点数
 79 
 80     memset(dp,0,sizeof(dp));    //按拓扑序来dp
 81     int ans=0;
 82     while(!que.empty())
 83     {
 84         int siz=que.size();
 85         for(int i=0; i<siz; i++)    //所有入度为0的节点
 86         {
 87             int t=que.front();que.pop_front();
 88             ans=max(ans,dp[t]+num[t]);
 89             for(int j=0; j<g[t].size(); j++)    //每条以t出发的边
 90             {
 91                 int d=g[t][j];
 92                 r[d]--;
 93                 if(!r[d])  que.push_back(d);
 94                 dp[d]=max(dp[d],dp[t]+num[t]);
 95             }
 96         }
 97     }
 98     return ans;
 99 }
100 
101 
102 int main()
103 {
104     //freopen("input.txt", "r", stdin);
105 
106     int t, a, b;
107     cin>>t;
108     while(t--)
109     {
110         scanf("%d%d", &n, &m);
111         for(int i=1; i<=n; i++) vect[i].clear();
112         for(int i=0; i<m; i++)
113         {
114             scanf("%d%d",&a,&b);
115             vect[a].push_back(b);
116         }
117         cout<<cal()<<endl;
118     }
119     return 0;
120 }
AC代码
```

