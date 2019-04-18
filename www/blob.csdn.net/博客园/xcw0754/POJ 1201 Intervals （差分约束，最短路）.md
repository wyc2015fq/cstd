# POJ 1201 Intervals （差分约束，最短路） - xcw0754 - 博客园
# [POJ 1201 Intervals （差分约束，最短路）](https://www.cnblogs.com/xcw0754/p/4693889.html)
题意：
　　有一个集合Z，其元素都是整整数，但是数量未知。现有n个约束，形如 [a,b]=c 表示整数区间[a,b]中有c个元素在Z中出现。问集合Z最小可能含多少个元素？
思路：
　　对于所给的区间 cnt[b-a]>=k这可以保证了该区间内个数不少于k。但是由于两边都是闭区间，所以要变cnt[b-(a-1)]>=k，表示b到a之间的个数。也就是说，转成式子是b-(a-1)>=k，变换一下为(a-1)-b<=-k，就满足常见的式子b-a<=k啦，可以建边b指向(a-1)，权值为-k。
　　但是还有没有其他的约束条件呢？如果我们只是这样建图，那很多个点都不会有联系的啊，比如[3,7]>=2和 [5,9]>=4，建边为2->7和4>9，完全不搭边啊！肯定还有其他联系，相邻的两个数字之间是否可以有边？可以的，至少可以满足a-(a-1)>=0吧？表示a只能被选与不被选。还有呢？每个点最多只能被选一次吧？则有a-(a-1)<=1。
　　综合3条式子，可以变形得到：
　　（1）(a-1)-b<=-k
　　（2）(a-1)-a<=0
　　（3）a-(a-1)<=1
　　都满足了常见的式子。那么可以建边了。这下每两个相邻数字就起码有两条边。注意：求的值是最小的，所以标准形式用<=号。
　　我们要求的是什么？满足要求的，整个区间所挑的最小个数。从最大数字出发，进行求最短路后，最小的数的那个点通常就会得到一个负权路径和啦,cost[最小]=负值。而cost[最小]取相反数就是答案了。记得开始前要将cost[起点]置为0。大概思路是这样，但是区间是[0,50000]，所以0-1时还会出现负数下标，可以将整个区间右移一位变成[1,50001]。
```
1 //#include <bits/stdc++.h>
 2 #include <cstdio>
 3 #include <cstring>
 4 #include <map>
 5 #include <iostream>
 6 #include <deque>
 7 #include <vector>
 8 #define INF 0x7f7f7f7f
 9 #define pii pair<int,int>
10 #define LL unsigned long long
11 using namespace std;
12 const int N=51010;
13 struct node
14 {
15     int from,to,cost;
16     node(){};
17     node(int from,int to,int cost):from(from),to(to),cost(cost){};
18 }edge[N*4];
19 vector<int> vect[N];
20 int edge_cnt;
21 
22 void add_node(int from,int to,int cost)
23 {
24     edge[edge_cnt]=node(from,to,cost);
25     vect[from].push_back(edge_cnt++);
26 }
27 
28 bool inq[N];
29 int cost[N];
30 int spfa(int small,int big)
31 {
32     memset(inq,  0, sizeof(inq));
33     memset(cost, 0x7f, sizeof(cost));
34     cost[big]=0;
35     deque<int> que(1,big);
36 
37     while(!que.empty())
38     {
39         int x=que.front();que.pop_front();
40         inq[x]=0;
41 
42         for(int i=0; i<vect[x].size(); i++)
43         {
44             node e=edge[vect[x][i]];
45             if(cost[e.to]>cost[x]+e.cost)
46             {
47                 cost[e.to]=cost[x]+e.cost;
48                 if(!inq[e.to])
49                 {
50                     inq[e.to]=1;
51                     if(!que.empty() && cost[e.to]<cost[que.front()])    que.push_front(e.to);
52                     else    que.push_back(e.to);
53                 }
54             }
55         }
56     }
57     return -cost[small];
58 }
59 
60 int main()
61 {
62     //freopen("input.txt", "r", stdin);
63     int n, m, a, b, c;
64     while(~scanf("%d",&n))
65     {
66         edge_cnt=0;
67         memset(edge,0,sizeof(edge));
68         for(int i=0; i<N; i++)  vect[i].clear();
69         int small=INF, big=0;
70 
71         for(int i=0; i<n; i++)
72         {
73             scanf("%d%d%d",&a,&b,&c);
74             add_node(b+1,a,-c);   //右移了1位
75 
76             small=min(small, a+1);
77             big=max(big, b+1);
78         }
79         //每两个点之间的
80         for(int i=small; i<=big; i++)
81         {
82             add_node(i-1, i, 1);
83             add_node(i, i-1, 0);
84         }
85         cout<<spfa(small-1, big)<<endl;
86     }
87     return 0;
88 }
AC代码
```

