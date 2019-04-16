# NSOJ  飞船汇合(经典) - Vmetrio - 博客园







# [NSOJ  飞船汇合(经典)](https://www.cnblogs.com/wangmengmeng/p/5346115.html)






一支分散的飞船舰队，需要汇合到主舰，但是这种飞船在太空中飞行的耗油与质量没有关系，只与发动机打开的时间有关系，为了节省油量，指挥官通知，汇合途中，多台飞船可以串成串飞行，这样只需启动一台发动机，由于安全因素飞船只能走某些航线（某飞船到某飞船的航线）。指挥发现这样的移动方案可能有多种，但最短汇合时间相同，指挥官想考察你是否知道在总耗油最小的情况下，最短多久汇合完毕。




### Input


T（T<10）组数据每组第一行有一个整数N（飞船个数<=300），之后第一行是其他飞船到主舰的时间，再有n行，每行n个数Aij表示Ai到Aj的时间。（均在int范围内）




### Output


每组数据输出总耗油最小的情况下，总耗油量与最短汇合时间。




### Sample Input
1
5
5 7 4 3 6
0 2 1 6 3
2 0 3 1 4
1 3 0 4 5
6 1 4 0 2
3 4 5 2 0
### Sample Output
9 7


题解:   铭轩出的题,真是好题啊....比赛的时候,最小生成树建图建错了....最后才懂,最小生成树(Prim和kruskal都可以)+搜索(最小值最大化)....存图的时候必须是半张图..不然会WA...蒟蒻啊....



WA代码:

```
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](https://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


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
 13 
 14 #define MAX 1010
 15 #define INF 0x3f3f3f3f
 16 
 17 struct edge
 18 {
 19     int x,y,cost;
 20 };
 21 
 22 edge e[MAX*MAX];
 23 int f[MAX];
 24 int dis[MAX];
 25 int v,n,ans;
 26 int mapp[305][305],pan[305];
 27 
 28 bool cmp(edge a,edge b)
 29 {
 30     return a.cost<b.cost;
 31 }
 32 
 33 void init(int n)
 34 {
 35     for(int i=0;i<=n;i++)
 36         f[i]=i;
 37 }
 38 
 39 int find(int x)
 40 {
 41     if(x!=f[x])
 42         f[x]=find(f[x]);
 43     return f[x];
 44 }
 45 
 46 void Union(int x,int y)
 47 {
 48     x=find(x);
 49     y=find(y);
 50     f[y]=x;
 51 }
 52 
 53 int same(int x,int y)
 54 {
 55     return find(x)==find(y);
 56 }
 57 
 58 int kruskal(int n,int m)
 59 {
 60     sort(e,e+m,cmp);
 61     init(n);
 62     int ans=0;
 63     for(int i=0;i<m;i++)
 64     {
 65         if(!same(e[i].x,e[i].y))
 66         {
 67             Union(e[i].x,e[i].y);
 68             ans+=e[i].cost;
 69             mapp[e[i].x][e[i].y]=e[i].cost;
 70             mapp[e[i].y][e[i].x]=e[i].cost;
 71         }
 72     }
 73     return ans;
 74 }
 75 
 76 int maxx=0;
 77 void dfs(int x,int step) {
 78     for (int i=0; i<=n; i++) {
 79         if (mapp[x][i] && !pan[i]) {
 80             pan[i]=1;
 81             dfs(i,step+mapp[x][i]);
 82         }
 83     }
 84     if (maxx<step) maxx=step;
 85 }
 86 
 87 int main()
 88 {
 89     int t;
 90     scanf("%d",&t);
 91     while(t--){
 92         memset(mapp,0,sizeof(mapp));
 93         memset(pan,0,sizeof(pan));
 94         memset(e,0,sizeof(e));
 95         int v=0;
 96         scanf("%d",&n);
 97         int count;
 98         for(int i=0;i<=n;i++)
 99             for(int j=1;j<=n;j++){
100                 e[v].x=i;
101                 e[v].y=j;
102                 scanf("%d",&count);
103                 e[v++].cost=count;
104             }
105         int p=kruskal(n,v);
106         
107         pan[0]=1;
108         dfs(0,0);
109         
110         printf("%d %d\n",p,maxx);
111     }
112 }


View Code
```



AC代码:

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
 13 
 14 #define MAX 1010
 15 #define INF 0x3f3f3f3f
 16 
 17 struct edge
 18 {
 19     int x,y,cost;
 20 };
 21 
 22 edge e[MAX*MAX];
 23 int f[MAX];
 24 int dis[MAX];
 25 int v,n,ans;
 26 int mapp[305][305],pan[305];
 27 
 28 bool cmp(edge a,edge b)
 29 {
 30     return a.cost<b.cost;
 31 }
 32 
 33 void init(int n)
 34 {
 35     for(int i=0;i<=n;i++)
 36         f[i]=i;
 37 }
 38 
 39 int find(int x)
 40 {
 41     if(x!=f[x])
 42         f[x]=find(f[x]);
 43     return f[x];
 44 }
 45 
 46 void Union(int x,int y)
 47 {
 48     x=find(x);
 49     y=find(y);
 50     f[y]=x;
 51 }
 52 
 53 int same(int x,int y)
 54 {
 55     return find(x)==find(y);
 56 }
 57 
 58 int kruskal(int n,int m)
 59 {
 60     sort(e,e+m,cmp);
 61     init(n);
 62     int ans=0;
 63     for(int i=0;i<m;i++)
 64     {
 65         if(!same(e[i].x,e[i].y))
 66         {
 67             Union(e[i].x,e[i].y);
 68             ans+=e[i].cost;
 69             mapp[e[i].x][e[i].y]=e[i].cost;
 70             mapp[e[i].y][e[i].x]=e[i].cost;
 71         }
 72     }
 73     return ans;
 74 }
 75 
 76 int maxx=0;
 77 void dfs(int x,int step) {
 78     for (int i=0; i<=n; i++) {
 79         if (mapp[x][i] && !pan[i]) {
 80             pan[i]=1;
 81             dfs(i,step+mapp[x][i]);
 82         }
 83     }
 84     if (maxx<step) maxx=step;
 85 }
 86 
 87 int main()
 88 {
 89     int t;
 90     scanf("%d",&t);
 91     while(t--){
 92         memset(mapp,0,sizeof(mapp));
 93         memset(pan,0,sizeof(pan));
 94         memset(e,0,sizeof(e));
 95         int v=0;
 96         scanf("%d",&n);
 97         int count;
 98         for(int i=0;i<=n;i++)
 99             for(int j=1;j<=n;j++){
100                 scanf("%d",&count);
101                 if(i<j){
102                 e[v].x=i;
103                 e[v].y=j;    
104                 e[v++].cost=count;
105                 }
106             }
107         int p=kruskal(n,v);
108         
109         pan[0]=1;
110         dfs(0,0);
111         
112         printf("%d %d\n",p,maxx);
113     }
114 }
```














