# UVALive 5532 King(差分约束，spfa) - xcw0754 - 博客园
# [UVALive 5532 King(差分约束，spfa)](https://www.cnblogs.com/xcw0754/p/4694427.html)
题意：假设一个序列S有n个元素，现在有一堆约束，限制在某些连续子序列之和上，分别有符号>和<。问序列S是否存在？（看题意都看了半小时了！）
　　注意所给的形式是(a,b,c,d)，表示：区间之和：sum[a,a+b]<d或者sum[a,a+b]>d。而c是两个字符构成，判断前1个字符足矣。
思路：
　　首先考虑要用点来表示什么，可以看到所给的是区间，也就是首尾的位置，可令sum(a)表示序列a[1...a]的和，那么表达式大概为sum(a+b)-sum(a-1)<k，由于小于号的存在，得换成小于等于号，所以表达式改为sum(a+b)-sum(a-1)<=k-1就行了。>号也是同理。所给的m个限制就被转换成边了。
　　但是好像建完图后，里面有些块完全没联系啊（即不连通）？比如a[1...7]有个限制，a[4...9]也有个限制，但是这4个点压根就是两个帮派的！没有关系的哈，如果不是有交点的话，完全不会产生任何冲突的，比如sum(a[1...7])是与sum(a[4...9])没有任何联系的，因为他们的相交区间sum(a[4...7])的大小无论如何取值，只要在另外一部分另外取合适的值即可（可以为负数），不会冲突，比如sum(a[4....7])=10086，而sum(a[1...7])=0，则sum(a[1...3])=-10086即可。也可以这么说，子区间只要有一个数不同时被限制，无论如何都不会有冲突。
　　再举例，a[1...7]和a[4...9]和a[1...9]这3个限制总算有联系了吧！自己画图吧，太难解释了。他们还是无法造成冲突。
```
1 #include <bits/stdc++.h>
 2 #define INF 0x7f7f7f7f
 3 #define pii pair<int,int>
 4 #define LL unsigned long long
 5 using namespace std;
 6 const int N=150;
 7 struct node
 8 {
 9     int from, to, cost;
10     node(){};
11     node(int from,int to,int cost):from(from),to(to),cost(cost){};
12 }edge[N*N*2];
13 vector<int> vect[N];
14 int edge_cnt;
15 
16 void add_node(int from,int to,int cost)
17 {
18     edge[edge_cnt]=node(from,to,cost);
19     vect[from].push_back(edge_cnt++);
20 }
21 
22 set<int> sett;
23 int cost[N], cnt[N];
24 bool inq[N];
25 int spfa(int up)
26 {
27     memset(inq,  1, sizeof(inq));
28     memset(cnt,  0, sizeof(cnt));
29     memset(cost, 0, sizeof(cost));
30     deque<int> que;
31     for(set<int>::iterator it=sett.begin(); it!=sett.end(); it++)    que.push_back(*it);//全部进！
32 
33     while(!que.empty())
34     {
35         int x=que.front();que.pop_front();
36         inq[x]=0;
37         for(int i=0; i<vect[x].size(); i++)
38         {
39             node e=edge[vect[x][i]];
40             if(cost[e.to]>cost[x]+e.cost)
41             {
42                 cost[e.to]=cost[x]+e.cost;
43                 if(!inq[e.to])
44                 {
45                     inq[e.to]=1;
46                     if(++cnt[e.to]>up)  return false;
47                     if(!que.empty()&&cost[e.to]<cost[que.front()])//优化
48                         que.push_front(e.to);
49                     else
50                         que.push_back(e.to);
51                 }
52             }
53         }
54     }
55     return true;
56 }
57 
58 int main()
59 {
60     freopen("input.txt", "r", stdin);
61     int n, m, a, b, d, L, U;
62     char c[10];
63     while(scanf("%d", &n), n)
64     {
65         sett.clear();
66         edge_cnt=0;
67         memset(edge,0,sizeof(edge));
68         for(int i=0; i<=n; i++)  vect[i].clear();
69 
70         scanf("%d",&m);
71         for(int i=0; i<m; i++)
72         {
73             scanf("%d %d %s %d ", &a, &b, &c, &d);
74             sett.insert(a-1);
75             sett.insert(a+b);
76             if(c[0]=='g')   //大于
77             {
78                 add_node( a+b, a-1, -(d+1));
79             }
80             else
81             {
82                 add_node( a-1, a+b, d-1);
83             }
84         }
85         if(spfa(sett.size()))   puts("lamentable kingdom");
86         else    puts("successful conspiracy");
87     }
88     return 0;
89 }
AC代码
```

