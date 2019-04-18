# HYSBZ   1086 王室联邦  （树的分块） - xcw0754 - 博客园
# [HYSBZ   1086 王室联邦  （树的分块）](https://www.cnblogs.com/xcw0754/p/4761273.html)
题意：国王想把他的国家划分成若干个省。他的国家有n个城市，是一棵树，即n-1条边，编号为1..n。为了防止管理太过分散，每个省至少要有B个城市，为了能有效的管理，每个省最多只有3B个城市。每个省必须有一个省会，这个省会可以位于省内，也可以在该省外。但是该省的任意一个城市到达省会所经过的道路上的城市（除了最后一个城市，即该省省会）都必须属于该省。一个城市可以作为多个省的省会。输出有多少个省会，每个城市属于哪个省会，每个省的省会。
思路：暂时先不管省会应该在哪的问题，其实就是要对树进行分块，每块必须有b~3b的点。
　　那么如何分块？按常理，只要搜索满b个点就立刻进行组块，而且块中的点最好是连通的，如若不巧，非连通，一会再说，能解决。由于要尽量使得所分的块是连通的，那么可以用DFS的回溯，将回溯过程收集的点装进stack，一般来说，任意一个点为根的子树中的所有点都是在stack中是连在一块的（因为先收集完孩子才会收集到自己，所以这是肯定的）。
　　收集完这个回溯序列有什么用？分块其实可以从里面分出来，只是不能随便就按照b个点就分一块，这样子可能会不连通。但是可以利用“以任意一点为根的子树中所有点在stack中肯定是相连的”这个特点，如果遍历到某个点x，它的某1个分叉中的点已经够b个了，那就组成一块；如果这一分叉不够b个，那么可以跟另一分叉组成一块，这样子虽然是不连通的，但是没有关系，只要省会设置在当前点x，不就连通了？不够点数的分叉都是可以组成1块，注意将一棵子树遍历完了，才能考虑将这个子树中的点分块。
　　如果stack中仍有剩下的点，肯定是不足b个，那么可以全部归到最后一个块中，必定不超过3b个点。
```
1 #include <bits/stdc++.h>
 2 #define pii pair<int,int>
 3 #define INF 0x3f3f3f3f
 4 #define LL long long
 5 using namespace std;
 6 const int N=1010;
 7 
 8 vector<int> vect[N];
 9 int n, m;
10 
11 void add_edge(int from,int to)
12 {
13     vect[from].push_back(to);
14     vect[to].push_back(from);
15 }
16 
17 stack<int> stac;
18 int belongto[N], vis[N], block, pro[N];
19 void DFS(int x)     //num表示后面还剩下几个。
20 {
21     vis[x]=1;
22     int cur=stac.size();//x上面的都不要碰。x只能碰它的子树。
23     for(int i=0; i<vect[x].size(); i++)
24     {
25         int t=vect[x][i];
26         if(!vis[t])
27         {
28             DFS(t);
29             if(stac.size()-cur >= m)  //够m个就组。
30             {
31                 //把这m个弄出来
32                 pro[++block]=x;//以x作为省会
33                 while(stac.size()>cur)
34                 {
35                     int p=stac.top();
36                     stac.pop();
37                     belongto[p]=block;
38 
39                 }
40             }
41         }
42     }
43     stac.push(x);
44 }
45 
46 
47 
48 int main()
49 {
50     freopen("input.txt", "r", stdin);
51     int  a, b;
52     while(cin>>n>>m)
53     {
54         block=0;
55         memset(vis, 0, sizeof(vis));
56         for(int i=0; i<=n; i++)    vect[i].clear();
57         for(int i=1; i<n; i++)
58         {
59             scanf("%d%d",&a,&b);
60             add_edge(a, b);
61         }
62 
63         DFS(1);//从任意点遍历
64         while(!stac.empty())        //可能有余下的点
65         {
66             int p=stac.top();
67             stac.pop();
68             belongto[p]=block;
69         }
70         if(n<m)    printf("0\n");//不够1块
71         else
72         {
73             printf("%d\n",block);
74             for(int i=1; i<=n; i++) printf("%d ", belongto[i]);
75             printf("\n");
76             for(int i=1; i<=block; i++) printf("%d ", pro[i]);
77             printf("\n");
78         }
79     }
80 
81     return 0;
82 }
AC代码
```

