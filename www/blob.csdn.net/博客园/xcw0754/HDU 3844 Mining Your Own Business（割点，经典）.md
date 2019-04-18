# HDU 3844 Mining Your Own Business（割点，经典） - xcw0754 - 博客园
# [HDU 3844 Mining Your Own Business（割点，经典）](https://www.cnblogs.com/xcw0754/p/4626277.html)
题意：
　　给出一个连通图，要求将某些点涂黑，使得无论哪个点（包括相关的边）撤掉后能够成功使得剩下的所有点能够到达任意一个涂黑的点，颜料不多，涂黑的点越少越好，并输出要涂几个点和有多少种涂法。
思路：
　　要使得任意撤掉一个点都能使其他点能够到达黑点，那么点双连通分量能保证这点，那么就在同个点双连通分量内涂黑1个点。但是每个【点双连通分量】都涂吗？太浪费颜料了，那就缩点成树，只需要涂叶子即可，那就找度为1的缩点。但是种数呢？叶子内的点除了割点外都是可以涂黑的，因为如果黑色割点被撤掉，那么叶子中的其他点怎么办?所以不能涂割点，每个黑点有【叶子中的点数-1】种涂法，所有黑店的涂法相乘为第2个结果。
　　特殊情况，因为给的是连通图且至少有2个点，那么还可能会出现没有割点的情况（仅1个点双连通分量），那就直接涂黑两个，以防一个黑点被撤掉。
此题出现的连续的点可能多达10万个，DFS就会爆栈。在C++下可以手动开栈，G++下的还不清楚怎么开。
```
1 #pragma comment(linker,"/STACK:102400000,102400000")//开栈
  2 //#include <bits/stdc++.h>
  3 #include <iostream>
  4 #include <cstdio>
  5 #include <cstring>
  6 #include <algorithm>
  7 #include <vector>
  8 #include <unordered_map>
  9 #include <stack>
 10 #define LL long long
 11 #define pii pair<int,int>
 12 using namespace std;
 13 const int N=100000+5;
 14 const int INF=0x7f7f7f7f;
 15 int up;
 16 int low[N], dfn[N];
 17 bool iscut[N];
 18 int dfn_clock, bcc_cnt, bcc_no[N];
 19 unordered_map<int,int> mapp;
 20 stack< pii >  stac;
 21 vector<int> bcc[N], vect[N];
 22 
 23 void DFS(int x, int far)//tarjan
 24 {
 25     dfn[x]=low[x]=++dfn_clock;
 26 
 27     int chd=0;
 28     for(int i=0; i<vect[x].size(); i++)
 29     {
 30         int t=vect[x][i];
 31         if(!dfn[t])
 32         {
 33             chd++;
 34             stac.push(make_pair(x,t));
 35             DFS(t,x);
 36             low[x]=min( low[x], low[t]);
 37             if(low[t]>=dfn[x])
 38             {
 39                 iscut[x]=true;    //需要标记割点
 40                 bcc[++bcc_cnt].clear();
 41                 while(true)
 42                 {
 43                     int a=stac.top().first;
 44                     int b=stac.top().second;
 45                     stac.pop();
 46                     if(bcc_no[a]!=bcc_cnt)
 47                     {
 48                         bcc[bcc_cnt].push_back(a);
 49                         bcc_no[a]=bcc_cnt;
 50                     }
 51                     if(bcc_no[b]!=bcc_cnt)
 52                     {
 53                         bcc[bcc_cnt].push_back(b);
 54                         bcc_no[b]=bcc_cnt;
 55                     }
 56                     if(a==x&&b==t)    break;
 57                 }
 58             }
 59         }
 60         else if( dfn[t]<dfn[x] && t!=far)
 61         {
 62             stac.push(make_pair(x,t));
 63             low[x]=min(low[x],dfn[t]);
 64         }
 65     }
 66     if(chd==1&&far==0)    iscut[x]=false;        //根
 67 }
 68 
 69 void find_bcc(int Case)
 70 {
 71     memset(low,0,sizeof(low));
 72     memset(dfn,0,sizeof(dfn));
 73     memset(iscut,0,sizeof(iscut));
 74     memset(bcc_no,0,sizeof(bcc_no));
 75 
 76     dfn_clock=bcc_cnt=0;
 77     for(int i=1; i<=up; i++)    if(!dfn[i])    DFS(i,0);   //深搜
 78     LL ans1=0,ans2=1;
 79 
 80     for(int i=1; i<=bcc_cnt; i++)    //统计度为多少
 81     {
 82         int cnt=0;
 83         for(int j=0; j<bcc[i].size(); j++)    if(iscut[bcc[i][j] ])    cnt++;    //有割点就统计连通分量i的度。
 84         if(cnt==1)    ans1++, ans2*=bcc[i].size()-1;
 85     }
 86     if(bcc_cnt==1)    ans1=2,ans2=(LL)bcc[1].size()*(bcc[1].size()-1)/2;
 87     printf("Case %d: %lld %lld\n", Case, ans1, ans2);
 88 }
 89 
 90 
 91 int main()
 92 {
 93     freopen("input.txt", "r", stdin);
 94     int a, b, n, j=0;
 95     while(scanf("%d",&n), n)
 96     {
 97         mapp.clear();
 98         for(int i=1; i<N; i++)    vect[i].clear();
 99         up=0;
100         for(int i=0; i<n; i++)
101         {
102             scanf("%d%d",&a,&b);
103             if(!mapp[a])    mapp[a]=++up;
104             if(!mapp[b])    mapp[b]=++up;//点号缩小为连续
105 
106             vect[mapp[a]].push_back(mapp[b]);
107             vect[mapp[b]].push_back(mapp[a]);
108         }
109         find_bcc(++j);
110     }
111     return 0;
112 }
AC代码
```

