# HDU 3586  Information Disturbing （树形DP，二分） - xcw0754 - 博客园
# [HDU 3586  Information Disturbing （树形DP，二分）](https://www.cnblogs.com/xcw0754/p/4836878.html)
题意：　　
　　给定一个敌人的通信系统，是一棵树形，每个节点是一个敌人士兵，根节点是commander，叶子是前线，我们的目的是使得敌人的前线无法将消息传到commander，需要切断一些边，切断每条边需要一定的power，而我们有一台具有m点power的机器，问在使用此机器切断敌人通信系统的情况下，使得所切断边的最大边权达到最小是多少？（m<=100w，n<=1000）
思路：
　　其实就是要求所切断的边的最小瓶颈边。由于m比较大，不好枚举或者DP。但是他们具有线性的关系，就是瓶颈边越大，肯定越容易有解。可以二分枚举一下这条边的power值mid，然后进行一次DFS判断在此power在小于等于mid的情况下，花费最小是多少。如果花费<=m，则可以继续二分枚举。
```
1 //#include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 #include <cstring>
 5 #include <cmath>
 6 #include <map>
 7 #include <algorithm>
 8 #include <vector>
 9 #include <iostream>
10 #define pii pair<int,int>
11 #define INF 0x3f3f3f3f
12 #define LL  long long
13 using namespace std;
14 const double PI  = acos(-1.0);
15 const int N=1010;
16 
17 struct node
18 {
19     int from,to,cost,next;
20     node(){};
21     node(int from,int to,int cost,int next):from(from),to(to),cost(cost),next(next){};
22 }edge[N*2];
23 int head[N], n, m, edge_cnt;
24 void add_node(int from,int to,int cost)
25 {
26     edge[edge_cnt]=node(from,to,cost,head[from]);
27     head[from]=edge_cnt++;
28 }
29 
30 int DFS(int t,int far,int mid)
31 {
32     node e;
33     int sum=0;
34     for(int i=head[t]; i!=-1; i=e.next)
35     {
36         e=edge[i];
37         if( e.to==far )   continue;
38         int tmp=DFS(e.to, t, mid);
39         if( tmp>0 )   //有满足要求的
40         {
41             if( e.cost<tmp && e.cost<=mid )    sum+=e.cost; //更优解
42             else                               sum+=tmp;
43         }
44         else                        //子树已经不满足要求，则必须在这一叉断开
45         {
46             if(e.cost>mid)  return 0;  //这叉都不能断开，则无解
47             else            sum+=e.cost;
48         }
49     }
50     return sum;         //只要sum就能使得t子树与叶子断开
51 }
52 
53 
54 int main()
55 {
56     //freopen("input.txt", "r", stdin);
57     int a, b, c;
58     while(scanf("%d%d",&n,&m), n+m)
59     {
60         edge_cnt=0;
61         memset(head, -1, sizeof(head));
62         for(int i=1; i<n; i++)
63         {
64             scanf("%d%d%d",&a,&b,&c);
65             add_node(a,b,c);
66             add_node(b,a,c);
67         }
68         int val=DFS(1,-1,1000); //试试最大值1000，若不行，则无解
69         if(n==1 || val>m){puts("-1");continue;} //无解
70 
71         int L=1, R=1000;
72         while(L<R)      //二分求最小的瓶颈边
73         {
74             int mid=L+(R-L)/2;
75             int val=DFS(1,-1,mid);
76             if(val>0 && val<=m)  R=mid;  //满足要求
77             else                 L=mid+1;
78         }
79         printf("%d\n",R);
80     }
81     return 0;
82 }
AC代码
```

