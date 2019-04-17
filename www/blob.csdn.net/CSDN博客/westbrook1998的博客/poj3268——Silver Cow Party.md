# poj3268——Silver Cow Party - westbrook1998的博客 - CSDN博客





2018年08月18日 18:56:10[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：31








> 
One cow from each of N farms (1 ≤ N ≤ 1000) conveniently numbered 1..N is going to attend the big cow party to be held at farm #X (1 ≤ X ≤ N). A total of M (1 ≤ M ≤ 100,000) unidirectional (one-way roads connects pairs of farms; road i requires Ti (1 ≤ Ti ≤ 100) units of time to traverse. 

  Each cow must walk to the party and, when the party is over, return to her farm. Each cow is lazy and thus picks an optimal route with the shortest time. A cow’s return route might be different from her original route to the party since roads are one-way. 

  Of all the cows, what is the longest amount of time a cow must spend walking to the party and back? 

  Input 

  Line 1: Three space-separated integers, respectively: N, M, and X  

  Lines 2.. M+1: Line i+1 describes road i with three space-separated integers: Ai, Bi, and Ti. The described road runs from farm Ai to farm Bi, requiring Ti time units to traverse. 

  Output 

  Line 1: One integer: the maximum of time any one cow must walk. 

  Sample Input 

  4 8 2 

  1 2 4 

  1 3 2 

  1 4 7 

  2 1 1 

  2 3 5 

  3 1 2 

  3 4 4 

  4 2 3 

  Sample Output 

  10 

  Hint 

  Cow 4 proceeds directly to the party (3 units) and returns via farms 1 and 3 (7 units), for a total of 10 time units.
有向图的最短路，回去的时候是单源最短路，但是来的时候有多个起点，枚举起点应该不行，所以把图转一下，然后以终点为起点再求一次单源最短路即可，然后对应相加，求最大值

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int N=1050;
const int INF=0x3f3f3f3f;
int n,m,x;
int g[N][N];
bool vis[N];
int dis[N];
int rdis[N];
int u,v,w;
void Dijkstra(int s,int *dis){
    for(int i=1;i<=n;i++){
        dis[i]=INF;
        vis[i]=false;
    }
    dis[s]=0;
    for(int i=1;i<n;i++){
        int Min=INF;
        int k=-1;
        for(int j=1;j<=n;j++){
            if(!vis[j] && dis[j]<Min){
                Min=dis[j];
                k=j;
            }
        }
        if(k==-1){
            break;
        }
        vis[k]=true;
        for(int j=1;j<=n;j++){
            if(!vis[j] && dis[j]>dis[k]+g[k][j]){
                dis[j]=dis[k]+g[k][j];
            }
        }
    }
}
int main(void){
    //freopen("data.txt","r",stdin);
    scanf("%d%d%d",&n,&m,&x);
    memset(g,INF,sizeof(g));
    while(m--){
        scanf("%d%d%d",&u,&v,&w);
        if(w<g[u][v]){
            g[u][v]=w;
        }
    }
    Dijkstra(x,dis);
    for(int i=1;i<n;i++){
        for(int j=i+1;j<=n;j++){
            int t=g[i][j];
            g[i][j]=g[j][i];
            g[j][i]=t;
        }
    }
    Dijkstra(x,rdis);
    int ans=0;
    for(int i=1;i<=n;i++){
        if(dis[i]+rdis[i]>ans){
            ans=dis[i]+rdis[i];
        }
    }
    printf("%d\n",ans);
    return 0;
}
```





