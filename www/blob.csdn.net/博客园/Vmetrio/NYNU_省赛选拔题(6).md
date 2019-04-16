# NYNU_省赛选拔题(6) - Vmetrio - 博客园







# [NYNU_省赛选拔题(6)](https://www.cnblogs.com/wangmengmeng/p/5271873.html)





## 题目描述


有一天，小米找到了一个藏宝的迷宫地图，迷宫在一个沙漠里有，迷宫里面有许多宝藏。迷宫里可能有N个藏宝地点，用1到K标记。藏宝地点之间最多有一条通路相连。标记1为迷宫的进出口。

他已经知道其中K（1<=K<=N）个不同的地点真的藏有宝物。小米决定让他的机器人米多奇去探险。迷宫中的通路不是平坦的，到处都是陷阱。假设每条通路都有一个危险度，其值与通过此路的载重量成正比。米多奇在经过某个藏宝地点时可能会拿走宝物。但它每拿走一个藏宝地点的宝物后，它的载重量就会增加w。

当机器人米多奇进入迷宫时，它的载重量为0。只有当米多奇携带宝物的载重量不大于某个通路的危险度时，它才能顺利通过此条道路，否则就会掉入陷阱，不能出来。

小米希望他的机器人米多奇尽量多的带出宝物，当然他更希望米多奇最后能从标记为1的地点走出去。

米多奇最多能带出多少个藏宝地点的宝物。




## 输入


第1行有四个数 N M K W

接下来有K行， 每行一个整数，表示藏有宝物的地点标号。

再接下来有M行，每行三个整数X，Y，Z，表示地点X与地点Y之间有一条危险度为Z的通路。

1 ≤ N ≤ 8000   1 ≤ K ≤ N   1 ≤ M ≤ 15000  1 ≤ W, Z ≤ 10000

数据保证所有的地点之间都是有道路可以到达的。

提示：机器人米多奇经过一个藏宝地点时可以不拿走宝物, 而且同一个藏宝地点可以经过多次。

数据有多组，处理到文件结束


## 输出

米多奇可以带走的最大宝物数量

## 样例输入
5 4 3 1
2
3
4
1 2 3
2 4 2
1 4 3
3 5 6
## 样例输出
2

不懂可以找东东学长


```
1 #include <stdio.h>
 2 #include <string.h>
 3 #include <stdlib.h>
 4 #include <vector>
 5 #define Max_V 8100
 6 #define inf 0x3f3f3f3f
 7 using namespace std;
 8 int n,m,k,w;
 9 struct Edge{
10     int x,y,z;
11 };
12 vector<Edge> g;
13 bool have[Max_V];
14 int maxPass[Max_V];
15 int ik[Max_V];
16 int p[Max_V];
17 int findMax(int a,int b)
18 {
19     return a>b?a:b;
20 }
21 int findMin(int a,int b)
22 {
23     return a>b?b:a;
24 }
25 void init()
26 {
27     g.clear();
28     memset(have,0,sizeof(have));
29 }
30 void add_edge(int x,int y,int z)
31 {
32     g.push_back((Edge){x,y,z});
33     g.push_back((Edge){y,x,z});
34 }
35 int cmp(const void *a,const void *b)
36 {
37     return *(int *)a-*(int *)b;
38 }
39 int find_way()
40 {
41     memset(maxPass,0,sizeof(maxPass));
42     maxPass[1]=inf;
43     bool flag=true;
44     int u,v,z;
45     while(flag)
46     {
47         flag=false;
48         for(int i=0;i<g.size();i++)
49         {
50             u=g[i].x;v=g[i].y;z=g[i].z;
51             int t=maxPass[v];
52             maxPass[v]=findMax(maxPass[v],findMin(maxPass[u],z));
53             if (maxPass[v]!=t)
54                 flag=true;
55         }
56     }
57     int count=0;
58     for(int j=1;j<=n;j++)
59         if(have[j]&&maxPass[j]>=w)
60             ik[count++]=maxPass[j];
61     return count;
62 }
63 int solve()
64 {
65     int mk=find_way();
66     p[0]=1;
67     qsort(ik,mk,sizeof(int),cmp);
68     for(int i=1;i<mk;i++)
69     {
70         if(ik[i]>=p[i-1]*w+w)
71             p[i]=p[i-1]+1;
72         else p[i]=p[i-1];
73     }
74     return p[mk-1];
75 }
76 int main()
77 {
78     int i;
79     while(~scanf("%d%d%d%d",&n,&m,&k,&w))
80     {
81         init();
82         int loc;
83         for(i=0;i<k;i++)
84         {
85             scanf("%d",&loc);
86             have[loc]=1;
87         }
88         int x,y,z;
89         for(i=0;i<m;i++)
90         {
91             scanf("%d%d%d",&x,&y,&z);
92             add_edge(x,y,z);
93         }
94         printf("%d\n",solve());
95     }
96     return 0;
97 }
```














