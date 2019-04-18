# POJ  3162 Walking Race （树的直径，单调队列） - xcw0754 - 博客园
# [POJ  3162 Walking Race （树的直径，单调队列）](https://www.cnblogs.com/xcw0754/p/4814597.html)
题意：给定一棵带边权的n个节点的树，首先要求出每个点的最长路，然后写成序列d[1],d[2]...d[n]，然后求满足 区间最大值-区间最小值<=k 的最大区间长度为多少？
思路：
　　分两步进行：
　　（1）最多仅需3次DFS就可以在O(n)时间内求出每个点的最长路了。先从任意点t出发遍历过有点，记录下从点t出发到每个点的最长路，然后从记录的其中一个距t最远的点root出发，再一次DFS，就可以得到离root最远的点e啦，再从e出发DFS一次，就得到所有点的最长路了。注意3次DFS的代码都是一样的喔~
　　（2）求满足要求的最大区间长度可以用两个单调队列来实现，又是O(n)就可以解决了。[单调队列看这篇](http://blog.csdn.net/f_x_q/article/details/10011805)。
```
1 //#include <bits/stdc++.h>
 2 #include <vector>
 3 #include <iostream>
 4 #include <cstdio>
 5 #include <cstring>
 6 #define pii pair<int,int>
 7 #define INF 0x3f3f3f3f
 8 #define LL long long
 9 using namespace std;
10 const int N=1e6+10;
11 
12 struct node
13 {
14     int from,to,len,next;
15     node(){};
16     node(int from,int to,int len,int next):from(from),to(to),len(len),next(next){};
17 }edge[N*2];
18 int edge_cnt, n, k, a, b, root;
19 int head[N];
20 void add_node(int from,int to,int len)
21 {
22     edge[edge_cnt]=node(from,to,len,head[from]);
23     head[from]=edge_cnt++;
24 }
25 
26 int dis[N];
27 void DFS(int t,int far,int len)
28 {
29     dis[t]=max(dis[t], len);
30     node e;
31     for(int i=head[t]; i!=-1; i=e.next )
32     {
33         e=edge[i];
34         if(e.to!=far)    DFS(e.to, t, len+e.len);
35     }
36     if(dis[t]>dis[root])    root=t; //求最远的点
37 }
38 
39 void get_dis()
40 {
41     DFS(root=1,-1,0);   //随便1个点开始
42     DFS(root,-1,0);     //离点1最远的点开始
43     DFS(root,-1,0);     //离上个root最远的点开始
44 }
45 
46 int max_que[N], min_que[N];
47 int cal()   //单调队列求最大区间
48 {
49     int L=1,R=0,ans=0, st1=0,ed1=-1,st2=0,ed2=-1;
50     while( ++R<=n)
51     {
52         int val=dis[R];
53         while( st1<=ed1 && val>=dis[max_que[ed1]] )    ed1--;
54         max_que[++ed1]=R;
55 
56         while( st2<=ed2 && val<=dis[min_que[ed2]] )    ed2--;
57         min_que[++ed2]=R;
58 
59         while( dis[max_que[st1]]-dis[min_que[st2]]>k )
60         {
61             L++;
62             while( max_que[st1]<L )  st1++;
63             while( min_que[st2]<L )  st2++;
64         }
65         ans=max(ans,R-L+1);
66     }
67     return ans;
68 }
69 
70 int main()
71 {
72     //freopen("input.txt", "r", stdin);
73     scanf("%d%d",&n,&k);
74 
75     edge_cnt=0;
76     for(int i=0; i<=n; i++) head[i]=-1;
77 
78     for(int i=1; i<n; i++)
79     {
80         scanf("%d%d",&a,&b);
81         add_node(i+1,a,b);
82         add_node(a,i+1,b);
83     }
84     get_dis();
85     printf("%d\n",cal());      //单调队列解决
86     return 0;
87 }
AC代码
```

