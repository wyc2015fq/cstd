# POJ 3177 Redundant Paths (桥，边双连通分量，有重边) - xcw0754 - 博客园
# [POJ 3177 Redundant Paths (桥，边双连通分量，有重边)](https://www.cnblogs.com/xcw0754/p/4620547.html)
题意：给一个无向图，问需要补多少条边才可以让整个图变成【边双连通图】，即任意两个点对之间的一条路径全垮掉，这两个点对仍可以通过其他路径而互通。
思路：POJ 3352的升级版，听说这个图会给重边。先看3352的题解http://www.cnblogs.com/xcw0754/p/4619594.html。
　　其实与3352不同的就是重边出现了怎么办？假如出现的重边刚好是桥呢？
　　首先要知道，【割点】可以将两个【点双连通分量】隔开来，因为仅一个【点双连通分量】中肯定无割点，那么每两个点对都同时处于若干个简单环中才能当一个点撤掉仍然可以互通。
　　而【桥】可以将两个【边双连通分量】隔开来，因为仅仅一个【边双连通分量】中肯定无桥，那么每两个点对之间肯定有多条路径可达（边都是不同的），当任意1条边撤掉后每两个点对仍然可达。
　　以上两点很重要，根据第二点，我们知道，如果在一个【边双连通分量】中任意两个有边相连的点再添加任意条边都是不影响的，任意两点间依然有多条不同的【边】路径可达。但是如果桥出现了重边呢？那么该桥上的两点会同时属于一个【边双连通分量】，此时就会将两个【边双连通分量】合并为一。
　　如何判别桥是否出现重边？我们一般以t!=far来判别是否遇到一条边通往父亲，但是现在要变了，我们要判断是否遇到这条边多次，如果多次，要更新本节点x的low值了，应该用low[x]=min(low[x],dfn[far])来更新，更新完之后他们的low值自然会处于同一个领导之下，此时再按照3352中用的类似于并查集的方法就可以将其彻底归到同一个【边双连通分量】中。
 特别注意：求点双连通分量时要注意在t！=far前加上这个条件“ dfn[t]<dfn[x] ”。如果不求这个可以不加，因为t如果已经遍历过，且dfn[t]>dfn[x]，那么肯定在x之上访问过，此时dfn[t]可能可以更新到low[x]，但是如果dfn[t]<dfn[x]，且在x之下访问过，那么dfn[t]肯定比dfn[x]还小，那肯定比low[x]小，则更新不到，也不产生影响，所以也不用去判断dfn[t]<dfn[x]。
```
1 #include <iostream>
 2 #include <cmath>
 3 #include <cstdio>
 4 #include <vector>
 5 #include <cstring>
 6 #include <set>
 7 using namespace std;
 8 const int N=5000+5;
 9 vector<int> vect[N];
10 int low[N], dfn[N], pre[N], cnter;
11 int du[N];
12 vector<pair<int,int> >  cutt;
13 int find(int x) //寻找x的low值
14 {
15     if(low[x]==dfn[x])  return low[x];
16     return low[x]=find( pre[low[x] ] );
17 }
18 
19 void DFS(int x, int far)
20 {
21     low[x]= dfn[x]= ++cnter;
22     pre[cnter]=x;   //第cnter个访问的是x
23     int times=0;    //记录桥是否有两条，若有，则要更新low值
24     for(int i=0; i<vect[x].size(); i++)
25     {
26         int t=vect[x][i];
27         if(!dfn[t])
28         {
29             DFS(t,x);
30             low[x]=min(low[x],low[t]);
31             if(low[t]>dfn[x])   cutt.push_back(make_pair(x,t));
32         }
33         else if(t!=far)    low[x]=min(low[x],dfn[t]);
34         else
35         {
36             if(times>0) //重边，low应该更新为far才对。
37                 low[x]=min(low[x],dfn[far]);
38             times++;
39         }
40     }
41 }
42 
43 
44 int cal_bcc(int f)  //找桥
45 {
46     cutt.clear();
47     memset(du,0,sizeof(du));
48     memset(low,0,sizeof(low));
49     memset(dfn,0,sizeof(dfn));
50     memset(pre,0,sizeof(pre));
51     cnter=0;
52     DFS(1,0);
53 
54     for(int i=0; i<cutt.size(); i++)
55     {
56         int a=cutt[i].first;
57         int b=cutt[i].second;
58         du[find(a)]++;
59         du[find(b)]++;
60     }
61     int ans=0;
62     for(int i=1; i<=f; i++)
63         if(du[i]==1) ans++;
64 /*
65     如果叶子数为偶数，可以采用互缠。
66     比如叶子顺序1234可以1-3，2-4。
67     注意奇数个叶子的情况。
68 */
69     return ((ans+1)/2);
70 }
71 
72 int main()
73 {
74     freopen("input.txt", "r", stdin);
75     int f, r, a, b, j=0;
76     char s[N];
77     while(cin>>f>>r)
78     {
79         for(int i=1; i<=f; i++) vect[i].clear();
80         while(r--)
81         {
82             scanf("%d%d", &a, &b);
83             vect[a].push_back(b);
84             vect[b].push_back(a);
85         }
86         printf("%d\n",cal_bcc(f));
87     }
88     return 0;
89 }
AC代码
```

