# Nyoj   虚拟的城市之旅(bfs) - Vmetrio - 博客园







# [Nyoj   虚拟的城市之旅(bfs)](https://www.cnblogs.com/wangmengmeng/p/5321256.html)




- 描述



 展馆是未来城市的缩影，个人体验和互动是不变的主题。在A国展馆通过多维模式和高科技手段，引领参观者在展示空间踏上一段虚拟的城市之旅。

 梦幻国有N个城市和M条道路，每条道路连接某两个城市。任意两个城市之间最多只有一条道路直接相连。这M条道路中有一部分为单向通行的道路，一部分为双向通行的道路。

 梦幻国幅员辽阔，各地的资源分布情况各不相同，这就导致了同一种商品在不同城市的价格不一定相同。但是，同一种商品在同一个城市的买入价和卖出价始终是相同的。

 现在你已踏上一段虚拟的城市之旅。为了给你一个意外收获，允许你在旅游的同时，利用 X 商品在不同城市中的差价赚回一点旅费，但最多只能交易一次。即,在某个城市买入X 商品，可以走到另外一个城市买掉来获得旅费。当然，在赚不到差价的情况下，你也可以不进行贸易活动。

 设梦幻国N个城市的标号从1~ N，你只能从1 号城市出发，并最终在N 号城市结束自己的旅行。在旅游的过程中，任何城市可以重复经过多次，但不要求经过所有N个城市。

 例如：梦幻国有5个大城市，城市的编号和道路连接情况如下图，单向箭头表示这条道路为单向通行，双向箭头表示这条道路为双向通行。假设 X 商品在1~5 号城市的价格分别为 4，3，5，6，1。

![](http://acm.nyist.edu.cn/JudgeOnline/admin/kind/attached/20110423172507_23501.jpg)

 你可以选择如下一条线路：1235，并在2 号城市以3 的价格买入X 商品，在3号城市以5 的价格卖出X 商品，赚取的旅费数为2。

 你也可以选择如下一条线路14545，并在第1次到达5号城市时以1的价格买入X 商品，在第2次到达4号城市时以6 的价格卖出X 商品，赚取的旅费数为5。

现在给出N个城市的X 商品价格，M条道路的信息（每条道路所连接的两个城市的编号以及该条道路的通行情况）。请问你能赚取尽可能多的旅费吗。


- 输入
有多组测试数据（以EOF为文件结束的标志） 
每组测试数据的格式如下：
第一行：N M 分别表示城市的数目和道路的数目。
第二行：N个正整数，每两个整数之间用一个空格隔开，分别表示1到N个城市的商品价格。
接下来 M行，每行有3个正整数，X，Y，Z，每两个整数之间用一个空格隔开。
如果 Z=1，表示这条道路是城市X到城市Y之间的单向道路；
如果Z=2，表示这条道路为城市X 和城市Y之间的双向道路。

1≤N≤100000，1≤M≤500000，
1≤X，Y≤N，1≤Z≤2，1≤商品价格≤100。- 输出
输出1个整数，表示最多能赚取的旅费。如果没有进行贸易，则输出0。- 样例输入
5 5
4 3 5 6 1
1 2 1
1 4 1
2 3 2
3 5 1
4 5 2- 样例输出
5- 来源
[第三届河南省程序设计大赛](http://acm.nyist.edu.cn/JudgeOnline/search_result.php?source=%E7%AC%AC%E4%B8%89%E5%B1%8A%E6%B2%B3%E5%8D%97%E7%9C%81%E7%A8%8B%E5%BA%8F%E8%AE%BE%E8%AE%A1%E5%A4%A7%E8%B5%9B)- 上传者
[张云聪](http://acm.nyist.edu.cn/JudgeOnline/profile.php?userid=%E5%BC%A0%E4%BA%91%E8%81%AA)




问题就是找一条路径，路径上价格最高的点和价格最低的点差值最大且价格最高的节点的位置在价格最低的节点位置的后面。

设任意一点p，原点s,终点t

求从s到p可以经过的城市中价格最低的值。再求t到p可以经过的城市中价格最高的值（逆建图）。

这样的话求出一个min,max.因为min是从s->p ，max是从t->p，所以满足位置关系。





错误的代码:

```
1 #include <vector>
  2 #include <map>
  3 #include <set>
  4 #include <algorithm>
  5 #include <iostream>
  6 #include <cstdio>
  7 #include <cmath>
  8 #include <cstdlib>
  9 #include <string>
 10 #include <cstring>
 11 #include <queue>
 12 using namespace std;
 13 #define INF 0x3f3f3f3f
 14 #define MAX 100010
 15 
 16 //double dis[MAX],mp[MAX][MAX];
 17 int vis[MAX],viss[MAX];
 18 int num[MAX],maxx[MAX],minn[MAX];
 19 int n,m;
 20 vector<int> list[MAX];
 21 
 22 void min_bfs()
 23 {
 24     queue<int> q;
 25     memset(vis,0,sizeof(vis));
 26     
 27     while(!q.empty())
 28         q.pop();
 29 
 30     q.push(1);
 31     vis[1]=1;
 32     while(!q.empty()){
 33         int s=q.front();
 34         q.pop();
 35         for(int i=0; i<list[s].size(); i++){
 36             int e=list[s][i];
 37             minn[e]=min(minn[e],minn[s]);
 38             if(!vis[e]){
 39                 q.push(e);
 40                 vis[e]=1;
 41             }
 42         }
 43     }
 44 }
 45 
 46 void max_bfs()
 47 {
 48     queue<int> q;
 49     memset(viss,0,sizeof(viss));
 50 
 51     while(!q.empty())
 52         q.pop();
 53 
 54     q.push(1);
 55     viss[1]=1;
 56     while(!q.empty()){
 57         int s=q.front();
 58         q.pop();
 59         for(int i=0; i<list[s].size(); i++){
 60             int e=list[s][i];
 61             maxx[e]=max(maxx[e],maxx[s]);
 62             if(!viss[e]){
 63                 q.push(e);
 64                 viss[e]=1;
 65             }
 66         }
 67     }
 68 }
 69 
 70 int main()
 71 {
 72     int x,y,z;
 73 
 74     while(~scanf("%d%d",&n,&m)){
 75         for(int i=1; i<=n; i++){
 76             scanf("%d",&num[i]);
 77             maxx[i]=minn[i]=num[i];
 78             list[i].clear();
 79         }
 80         for(int i=1; i<=m; i++){
 81             scanf("%d%d%d",&x,&y,&z);
 82             if(z==2){
 83                 list[y].push_back(x);
 84             }
 85             list[x].push_back(y);
 86         }
 87         min_bfs();
 88         max_bfs();
 89         //printf("%d\n",minn[4]);
 90         //printf("%d\n",maxx[5]);
 91         /*for (int i = 1; i <= n; ++i)
 92         {
 93             printf("%d\n",minn[i]);
 94             printf("%d\n",maxx[i]);
 95         }*/
 96         int ans=0;
 97         for(int i=1; i<=n; i++){
 98             if(vis[i] && viss[i] &&ans<maxx[i]-minn[i]){
 99                 ans=maxx[i]-minn[i];
100             }
101         }
102         printf("%d\n",ans);
103     }
104 }
```





AC代码:     (逆建图)需要好好理解.....

```
1 #include <vector>
  2 #include <map>
  3 #include <set>
  4 #include <algorithm>
  5 #include <iostream>
  6 #include <cstdio>
  7 #include <cmath>
  8 #include <cstdlib>
  9 #include <string>
 10 #include <cstring>
 11 #include <queue>
 12 using namespace std;
 13 #define INF 0x3f3f3f3f
 14 #define MAX 100010
 15 
 16 //double dis[MAX],mp[MAX][MAX];
 17 int vis[MAX],viss[MAX];
 18 int num[MAX],maxx[MAX],minn[MAX];
 19 int n,m;
 20 vector<int> list[MAX],llist[MAX];
 21 
 22 void min_bfs()
 23 {
 24     queue<int> q;
 25     memset(vis,0,sizeof(vis));
 26     
 27     while(!q.empty())
 28         q.pop();
 29 
 30     q.push(1);
 31     vis[1]=1;
 32     while(!q.empty()){
 33         int s=q.front();
 34         q.pop();
 35         for(int i=0; i<list[s].size(); i++){
 36             int e=list[s][i];
 37             minn[e]=min(minn[e],minn[s]);
 38             if(!vis[e]){
 39                 q.push(e);
 40                 vis[e]=1;
 41             }
 42         }
 43     }
 44 }
 45 
 46 void max_bfs()
 47 {
 48     queue<int> q;
 49     memset(viss,0,sizeof(viss));
 50 
 51     while(!q.empty())
 52         q.pop();
 53 
 54     q.push(n);
 55     viss[n]=1;
 56     while(!q.empty()){
 57         int s=q.front();
 58         q.pop();
 59         for(int i=0; i<llist[s].size(); i++){
 60             int e=llist[s][i];
 61             maxx[e]=max(maxx[e],maxx[s]);
 62             if(!viss[e]){
 63                 q.push(e);
 64                 viss[e]=1;
 65             }
 66         }
 67     }
 68 }
 69 
 70 int main()
 71 {
 72     int x,y,z;
 73 
 74     while(~scanf("%d%d",&n,&m)){
 75         for(int i=1; i<=n; i++){
 76             scanf("%d",&num[i]);
 77             maxx[i]=minn[i]=num[i];
 78             list[i].clear();
 79             llist[i].clear();
 80         }
 81         for(int i=1; i<=m; i++){
 82             scanf("%d%d%d",&x,&y,&z);
 83             if(z==2){
 84                 list[y].push_back(x);
 85                 llist[x].push_back(y);
 86             }
 87             list[x].push_back(y);
 88             llist[y].push_back(x);
 89         }
 90         min_bfs();
 91         max_bfs();
 92         //printf("%d\n",minn[4]);
 93         //printf("%d\n",maxx[5]);
 94         /*for (int i = 1; i <= n; ++i)
 95         {
 96             printf("%d\n",minn[i]);
 97             printf("%d\n",maxx[i]);
 98         }*/
 99         int ans=0;
100         for(int i=1; i<=n; i++){
101             if(vis[i] && viss[i] &&ans<maxx[i]-minn[i]){
102                 ans=maxx[i]-minn[i];
103             }
104         }
105         printf("%d\n",ans);
106     }
107 }
```














