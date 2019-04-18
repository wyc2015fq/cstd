# HDU 4276 The Ghost Blows Light  （树形DP，变形） - xcw0754 - 博客园
# [HDU 4276 The Ghost Blows Light  （树形DP，变形）](https://www.cnblogs.com/xcw0754/p/4835367.html)
题意：给定一棵n个节点的树，起点是1，终点是n，每经过一条边需要消耗Ti天，每个点上有一定量的珠宝，要求必须在t天内到达终点才可以将珠宝带出去，问至多能带多少珠宝？
思路：
　　注意Ti可以为0，而且有可能t太小以至于不能到达n，则输出不可达。这样想会简单点，将"1->n"路径上的每条边断开，变成一片森林，而且路径上的这些点是根。我们需要计算每棵树在j天内最多能获得多少珠宝，这只需要一次DFS就可以完成了。然后除了森林中的根（即1->n路径上的点），其他都可以不用考虑了，按照"1->n"路径的方向，逐个点进行转移，更新dp值，到最后dp[n][t]就是答案了。最后这步是“至少带走1件物品”分组背包模型，只需要先将1件物品丢进背包，有好的就更新，没有的话也保证了至少带1件物品。
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
 12 #define INF 0x3f3f3f3f
 13 #define LL  long long
 14 using namespace std;
 15 const double PI  = acos(-1.0);
 16 const int N=110;
 17 int n, edge_cnt, head[N], w[N], dp[N][550], cap[550];
 18 struct node
 19 {
 20     int from, to, cost, next, tag;
 21     node(){};
 22     node(int from,int to,int cost,int next):from(from),to(to),cost(cost),next(next){tag=0;};
 23 }edge[N*2];
 24 
 25 deque<pii> que;
 26 void add_node(int from,int to,int cost)
 27 {
 28     edge[edge_cnt]=node(from,to,cost,head[from]);
 29     head[from]=edge_cnt++;
 30 }
 31 
 32 bool dfs(int t,int far,int m)
 33 {
 34     int flag=false;
 35     if(m<0) return false;   //不可到达
 36 
 37     for(int i=0; i<=m; i++) dp[t][i]=w[t];      //到达本点至少获得w[t]
 38     node e;
 39     for(int i=head[t]; i!=-1; i=e.next)
 40     {
 41         e=edge[i];
 42         if(e.to==far)   continue;
 43         if(dfs(e.to, t, m-e.cost)==true)
 44         {
 45             que.push_back(make_pair(e.to, e.cost));
 46             flag=true;
 47             edge[i].tag=1;
 48         }
 49         else
 50         {
 51             for(int j=m; j>=0; j--)
 52                 for(int k=2*e.cost; k<=j; k++) //枚举给此孩子k-2*cost天的时间
 53                     dp[t][j]=max(dp[t][j], dp[t][j-k]+dp[e.to][k-2*e.cost] );
 54         }
 55     }
 56     if(t==n)    return true;    //终点站
 57     else        return flag;
 58 }
 59 
 60 int main()
 61 {
 62     freopen("input.txt", "r", stdin);
 63     int m, a, b, c;
 64     while(~scanf("%d%d",&n,&m))
 65     {
 66         que.clear();
 67         edge_cnt=0;
 68         memset(head,-1,sizeof(head));
 69         memset(dp,0,sizeof(dp));
 70         memset(cap,0,sizeof(cap));
 71 
 72         for(int i=1; i<n; i++)
 73         {
 74             scanf("%d%d%d",&a,&b,&c);
 75             add_node(a,b,c);
 76             add_node(b,a,c);
 77         }
 78         for(int i=1; i<=n; i++) scanf("%d",&w[i]);
 79         if(dfs(1,-1,m)==true)
 80         {
 81             reverse(que.begin(),que.end()); //变成1条链,逐个节点进行“至少带1件物品的分组背包”
 82             que.push_front(make_pair(1,0));
 83             int t, c, st=0;
 84             while(!que.empty())
 85             {
 86                 t=que.front().first;    //当前点已经在t
 87                 c=que.front().second;   //转移到t的花费
 88                 que.pop_front();
 89                 st+=c;
 90 
 91                 for(int j=m; j>=st; j--)
 92                 {
 93                     cap[j]=cap[j-c]+dp[t][0];  //从上个点转移到t至少需先消耗c
 94                     for(int k=1; k+st<=j; k++)
 95                         cap[j]=max(cap[j], cap[j-k-c]+dp[t][k]);
 96                 }
 97             }
 98             printf("%d\n", cap[m]);
 99         }
100         else puts("Human beings die in pursuit of wealth, and birds die in pursuit of food!");//不可达
101     }
102     return 0;
103 }
AC代码
```

