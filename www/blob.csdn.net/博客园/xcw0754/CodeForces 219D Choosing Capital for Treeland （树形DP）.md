# CodeForces  219D  Choosing Capital for Treeland （树形DP） - xcw0754 - 博客园
# [CodeForces  219D  Choosing Capital for Treeland （树形DP）](https://www.cnblogs.com/xcw0754/p/4815493.html)
题意：给一个树形图，n个节点，n-1条有向边，要求选一个节点作为根，使需要改变方向的边的数目最少。并输出所有可能作为根的点。
思路：
　　先随便一个点进行DFS，计算将每棵子树的边全部往下时，所需要的费用down[i]。还是那个点进行DFS，这次就要求答案了，尝试将每个点t作为根，那么以t作为根的总费用=down[t]+父亲这棵子树。down[t]已经在第一次DFS中求出，而父亲这棵子树就不是down[父亲]了，而是down[父亲]-down[t]+w(父亲,t)。注：w为边权。
```
1 #include <bits/stdc++.h>
 2 #define pii pair<int,int>
 3 #define INF 0x3f3f3f3f
 4 #define LL long long
 5 using namespace std;
 6 const int N=2e5+10;
 7 
 8 struct node
 9 {
10     int from,to,rev,next;
11     node(){};
12     node(int from,int to,int rev,int next):from(from),to(to),rev(rev),next(next){};
13 }edge[N*2];
14 int head[N],  n, edge_cnt;
15 void add_node(int from,int to,int rev)
16 {
17     edge[edge_cnt]=node(from,to,rev,head[from]);
18     head[from]=edge_cnt++;
19 }
20 
21 int down[N];     //down[i]表示是往下到点i
22 void DFS(int t,int far)
23 {
24     node e;
25     for(int i=head[t]; i!=-1; i=e.next)     //递归先算子树的
26     {
27         e=edge[i];
28         if(e.to^far)    DFS(e.to, t);
29     }
30     int sum=0;
31     for(int i=head[t]; i!=-1; i=e.next)     //再算自己的
32     {
33         e=edge[i];
34         if(e.to^far)    sum+=down[e.to]+e.rev;
35     }
36     down[t]=sum;   //只有1种情况：所有子树全部向下
37 }
38 
39 int ans[N], big;
40 void DFS2(int t,int far,int val)
41 {
42     node e;
43     ans[t]=down[t]+val;     //以本节点为根
44     big=min(big, ans[t]);
45     for(int i=head[t]; i!=-1; i=e.next)
46     {
47         e=edge[i];
48         if(e.to^far)
49         {
50             int r=ans[t]-down[e.to]-e.rev+(e.rev^1);
51             DFS2(e.to, t, r);
52         }
53     }
54 }
55 
56 void init()
57 {
58     memset(head, -1, sizeof(head));
59     memset(down, 0x3f, sizeof(down));
60     edge_cnt=0;
61     big=INF;
62 }
63 
64 int main()
65 {
66     //freopen("input.txt", "r", stdin);
67     init();
68     scanf("%d",&n);
69     for(int i=1,a,b; i<n; i++)
70     {
71         scanf("%d%d",&a,&b);
72         add_node(a,b,0);    //正向
73         add_node(b,a,1);
74     }
75     DFS(1,-1);
76     DFS2(1,-1,0);
77     printf("%d\n",big);
78     for(int i=1; i<=n; i++) //输出解
79         if(big==ans[i])
80             printf("%d ",i);
81     return 0;
82 }
AC代码
```

