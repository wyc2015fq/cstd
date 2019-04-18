# POJ 3107 Godfather （树的重心） - xcw0754 - 博客园
# [POJ 3107 Godfather （树的重心）](https://www.cnblogs.com/xcw0754/p/4837291.html)
题意：求树的重心，若有多个，全部打印出来。
思路：
　　树的重心：在删除点v后，森林中的每棵树的节点数尽量均匀，若最大的那棵树的节点数最小，称v为树的重心。
　　这道题只是求树的所有重心，当且经当这棵树有对称性质时才有多重心，因此一棵树的重心最多不会超过2个。也是一遍DFS就可以搞定了，[参考](http://www.cnblogs.com/xcw0754/p/4808212.html)这个。
```
1 //#include <bits/stdc++.h>
 2 #include <iostream>
 3 #include <cstdio>
 4 #include <cstring>
 5 #include <cmath>
 6 #include <map>
 7 #include <deque>
 8 #include <algorithm>
 9 #include <vector>
10 #include <iostream>
11 #define pii pair<int,int>
12 #define max(x,y) ((x)>(y)?(x):(y))
13 #define min(x,y) ((x)<(y)?(x):(y))
14 #define INF 0x7f7f7f7f
15 #define LL  long long
16 using namespace std;
17 const double PI  = acos(-1.0);
18 const int N=51000;
19 int n, cnt, edge_cnt, head[N];
20 
21 struct node
22 {
23     int from, to, next;
24     node(){};
25     node(int from,int to,int next):from(from),to(to),next(next){};
26 }edge[N*2];
27 
28 void add_node(int from,int to)
29 {
30     edge[edge_cnt]=node(from,to,head[from]);
31     head[from]=edge_cnt++;
32 }
33 deque<int> que;
34 
35 int DFS(int t,int far)
36 {
37     node e;
38     int big=0, sum=0;
39     for(int i=head[t]; i!=-1; i=e.next)
40     {
41         e=edge[i];
42         if(e.to==far)   continue;
43 
44         int tmp=DFS(e.to, t);
45         big=max(big, tmp);
46         sum+=tmp;
47     }
48     big=max(big, n-sum-1);
49 
50     if(big<=cnt)
51     {
52         if(big<cnt) que.clear();
53         cnt=big;
54         que.push_back(t);
55     }
56     return sum+1;
57 }
58 
59 int main()
60 {
61     //freopen("input.txt", "r", stdin);
62     int a, b;
63     while(~scanf("%d",&n))
64     {
65         edge_cnt=0;
66         cnt=INF;
67         memset(head,-1,sizeof(head));
68 
69         for(int i=1; i<n; i++)
70         {
71             scanf("%d%d",&a,&b);
72             add_node(a,b);
73             add_node(b,a);
74         }
75         DFS(1,-1);
76         sort(que.begin(),que.end());
77         while(!que.empty())
78         {
79             printf("%d ",que.front());
80             que.pop_front();
81         }
82         puts("");
83     }
84     return 0;
85 }
AC代码
```

