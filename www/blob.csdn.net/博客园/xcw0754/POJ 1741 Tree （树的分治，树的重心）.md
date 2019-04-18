# POJ 1741 Tree （树的分治，树的重心） - xcw0754 - 博客园
# [POJ 1741 Tree （树的分治，树的重心）](https://www.cnblogs.com/xcw0754/p/4811525.html)
题意：给一棵树，n个节点，给定一个数k，求任意满足dist(a,b)<=k的点对的数量。
思路：
　　这道题的思路比较简单，但是细节很多。
　　此题可以用分治法，如何分治？
　　（1）如果path(a,b)不经过根，那么肯定在根的某棵子树下，递归解决。
　　（2）如果path(a,b)经过根，那么肯定在根的不同子树下，处理它。
　　怎样处理？如果知道了每棵子树中的节点到根的距离，暂且将每棵子树的节点分到每个独立的桶里。每个节点都可以和其他桶里的点组成点对，只要距离<=k的话就满足要求了。逐个算可能超时了，用个简单点的方法。在这里我们不需要知道节点是谁，只需要知道距离，所以将所有节点到根的距离取出来，排个序，用两个指针在线性复杂度就可以解决本节点下的点对统计了。
　　但是如何去重？因为所有子树中的节点全部都混着排序了，估计会有挺多对是在同棵子树下的节点被统计到了，这暂时不需要，这是要递归解决的，那么就要去掉这些同棵树下的点对。计算的方法都是一样的。
　　但是还有一种情况让你TLE，就是单链时的情况，若每次以孩子来递归下去解决，可能就不行了。但是可以发现到，计算这棵子树时，完全也不用经过根，那么不妨直接找出这棵子树中的重心作为根来解决这个子问题。复杂度主要是在排序的地方，用了找重心的方法可以保证每个点最多被排序2*logn次，所以总复杂度为O(nlog2n)。
　　如下图，红色点为重心，红色的线为虚拟边。左图是原树，右图是分治的过程。就像是每个重心隔开了一些子树一样。
![](https://images2015.cnblogs.com/blog/641737/201509/641737-20150915212415867-1892216927.png)
　　有[论文](http://files.cnblogs.com/files/xcw0754/%E5%9B%BD%E5%AE%B6%E9%9B%86%E8%AE%AD%E9%98%9F2009%E8%AE%BA%E6%96%87%E9%9B%86%E5%88%86%E6%B2%BB%E7%AE%97%E6%B3%95%E5%9C%A8%E6%A0%91%E7%9A%84%E8%B7%AF%E5%BE%84%E9%97%AE.pdf)可以看。
```
1 //#include <bits/stdc++.h>
  2 #include <vector>
  3 #include <iostream>
  4 #include <cstdio>
  5 #include <algorithm>
  6 #include <cstring>
  7 #define pii pair<int,int>
  8 #define INF 0x3f3f3f3f
  9 #define LL long long
 10 using namespace std;
 11 const int N=10010;
 12 int n, k, root, edge_cnt, ans, num, big, vis[N];
 13 vector<int> vect[N];
 14 struct node
 15 {
 16     int from,to,len;
 17     node(){};
 18     node(int from,int to,int len):from(from),to(to),len
 19 
 20 (len){};
 21 }edge[N*2];
 22 
 23 void add_edge(int from,int to,int len)
 24 {
 25     edge[edge_cnt]=node(from,to,len);
 26     vect[from].push_back(edge_cnt++);
 27 }
 28 
 29 int get_size(int t,int far)
 30 {
 31     int cnt=1;
 32     for(int i=0; i<vect[t].size(); i++)
 33     {
 34         node e=edge[vect[t][i]];
 35         if(e.to!=far&&!vis[e.to])    cnt+=get_size(e.to, t);
 36     }
 37     return cnt;
 38 }
 39 
 40 int get_root(int t,int far)   //求重心:root。
 41 {
 42     int cur=0, sum=1;
 43     for(int i=0; i<vect[t].size(); i++)
 44     {
 45         node e=edge[vect[t][i]];
 46         if( e.to!=far && !vis[e.to] )
 47         {
 48             int tmp=get_root(e.to, t);
 49             sum+=tmp;
 50             cur=max(cur, tmp);
 51         }
 52     }
 53     cur=max(cur, num-sum);
 54     if(cur<big)     big=cur, root=t;    //更新root
 55     return sum;
 56 }
 57 
 58 vector<int> seq;
 59 void get_all(int t,int far,int len)     //获取子树t的所有节点
 60 {
 61     seq.push_back(len);
 62     if(len>=k)   return ;   //剪枝
 63     for(int i=0; i<vect[t].size(); i++)
 64     {
 65         node e=edge[vect[t][i]];
 66         if( !vis[e.to] && e.to!=far )    get_all(e.to, t, len+e.len);
 67     }
 68 }
 69 
 70 int cal(int t,int len)      //计算子树t的内部点对。
 71 {
 72     seq.clear();
 73     get_all(t, 0, len);     //将以t为根的整棵子树的len装进去seq
 74     sort(seq.begin(), seq.end());
 75     int tmp=0, L=0, R=seq.size()-1;
 76     while( L<R )
 77     {
 78         if( seq[L]+seq[R]<=k )    tmp+=R-L,L++;
 79         else    R--;
 80     }
 81     return tmp;
 82 }
 83 
 84 void solve(int t)    //主函数
 85 {
 86     vis[t]=1;
 87     ans+=cal(t, 0);         //计算子树t的答案
 88     for(int i=0; i<vect[t].size(); i++)
 89     {
 90         node e=edge[vect[t][i]];
 91         if( !vis[e.to] )
 92         {
 93             ans-=cal(e.to, e.len);  //去重
 94             big=INF;root=-1;num=get_size(e.to, -1); //用于找重心
 95             get_root(e.to, -1);
 96             solve(root);
 97         }
 98     }
 99 }
100 
101 
102 void init()
103 {
104     for(int i=0; i<=n; i++)vect[i].clear();
105     memset(vis, 0, sizeof(vis));
106     ans=edge_cnt=0;
107 }
108 
109 int main()
110 {
111     //freopen("input.txt", "r", stdin);
112     int a,b,c;
113     while( scanf("%d%d",&n,&k), n+k  )
114     {
115         init();
116         for(int i=1; i<n; i++)
117         {
118             scanf("%d%d%d",&a,&b,&c);
119             add_edge(a,b,c);
120             add_edge(b,a,c);
121         }
122         num=n;big=INF;root=-1;  //找重心用的。
123         get_root(1,-1);         //找重心
124         solve( root );
125         printf("%d\n", ans);
126     }
127     return 0;
128 }
AC代码
```

