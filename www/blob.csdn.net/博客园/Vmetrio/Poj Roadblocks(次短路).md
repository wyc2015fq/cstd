# Poj  Roadblocks(次短路) - Vmetrio - 博客园







# [Poj  Roadblocks(次短路)](https://www.cnblogs.com/wangmengmeng/p/5297320.html)





Description


Bessie has moved to a small farm and sometimes enjoys returning to visit one of her best friends. She does not want to get to her old home too quickly, because she likes the scenery along the way. She has decided to take the second-shortest rather than the shortest path. She knows there must be some second-shortest path.

The countryside consists of *R* (1 ≤ *R* ≤ 100,000) bidirectional roads, each linking two of the N (1 ≤ *N* ≤ 5000) intersections, conveniently numbered 1..*N*. Bessie starts at intersection 1, and her friend (the destination) is at intersection *N*.

The second-shortest path may share roads with any of the shortest paths, and it may backtrack i.e., use the same road or intersection more than once. The second-shortest path is the shortest path whose length is longer than the shortest path(s) (i.e., if two or more shortest paths exist, the second-shortest path is the one whose length is longer than those but no longer than any other path).


Input

Line 1: Two space-separated integers: *N* and *R*
Lines 2..*R*+1: Each line contains three space-separated integers: *A*, *B*, and *D* that describe a road that connects intersections *A* and *B* and has length *D* (1 ≤ *D* ≤ 5000)

Output

Line 1: The length of the second shortest path between node 1 and node *N*

Sample Input
4 4
1 2 100
2 4 200
2 3 250
3 4 100
Sample Output
450
Hint

Two routes: 1 -> 2 -> 4 (length 100+200=300) and 1 -> 2 -> 3 -> 4 (length 100+250+100=450)

Source

[USACO 2006 November Gold](http://poj.org/searchproblem?field=source&key=USACO+2006+November+Gold)



记最短路径长度为dis[ ]，次短路径长度为ddis[ ]，则dis[v] = min（ dis[u] + cost[u][v],  ddis[u] + cost[u][v] ），所以我们只需要计算出最短路径和次短路径即可。这就跟最短路径不一样了，在实现Dijkstra算法的时候，我们既要记录最短路径，还要记录次短路径。





代码:


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
14 #define MAX 5055
15 
16 int N,R;
17 struct node{
18     int to;
19     int cost;
20 };
21 vector<node> g[MAX];
22 int dis[MAX],ddis[MAX];
23 
24 void solve()
25 {
26     typedef pair<int,int> P;
27     priority_queue<P,vector<P>,greater<P> > que;
28     fill(dis,dis+N,INF);
29     fill(ddis,ddis+N,INF);
30     dis[0]=0;
31     que.push(P(0,0));
32 
33     while(!que.empty()){
34         P p=que.top();  que.pop();
35         int v=p.second,d1=p.first;
36         if(ddis[v]<d1)  continue;
37         for(int i=0; i<g[v].size(); i++){
38             node s=g[v][i];
39             int d2=d1+s.cost;
40             if(dis[s.to]>d2){
41                 swap(dis[s.to],d2);
42                 que.push(P(dis[s.to],s.to));
43             }
44             if(ddis[s.to]>d2 && dis[s.to]<d2){
45                 ddis[s.to]=d2;
46                 que.push(P(ddis[s.to],s.to));
47             }
48         }
49     }
50     printf("%d\n",ddis[N-1]);
51 }
52 
53 int main()
54 {
55     int from,to,cost;
56     node e;
57     while(~scanf("%d%d",&N,&R)){
58         while(R--){
59             scanf("%d%d%d",&from,&to,&cost);
60             from--;to--;
61             e.to=to;e.cost=cost;
62             g[from].push_back(e);
63             e.to=from;
64             g[to].push_back(e);
65         }
66         solve();
67     }
68 }
```

















