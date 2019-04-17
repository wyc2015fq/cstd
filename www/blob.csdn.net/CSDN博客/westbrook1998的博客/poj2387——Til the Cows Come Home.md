# poj2387——Til the Cows Come Home - westbrook1998的博客 - CSDN博客





2018年08月18日 18:42:08[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：34








> 
Bessie is out in the field and wants to get back to the barn to get as much sleep as possible before Farmer John wakes her for the morning milking. Bessie needs her beauty sleep, so she wants to get back as quickly as possible.  

  Farmer John’s field has N (2 <= N <= 1000) landmarks in it, uniquely numbered 1..N. Landmark 1 is the barn; the apple tree grove in which Bessie stands all day is landmark N. Cows travel in the field using T (1 <= T <= 2000) bidirectional cow-trails of various lengths between the landmarks. Bessie is not confident of her navigation ability, so she always stays on a trail from its start to its end once she starts it. 

  Given the trails between the landmarks, determine the minimum distance Bessie must walk to get back to the barn. It is guaranteed that some such route exists. 

  Input 

  * Line 1: Two integers: T and N  

  * Lines 2..T+1: Each line describes a trail as three space-separated integers. The first two integers are the landmarks between which the trail travels. The third integer is the length of the trail, range 1..100. 

  Output 

  * Line 1: A single integer, the minimum distance that Bessie must travel to get from landmark N to landmark 1. 

  Sample Input 

  5 5 

  1 2 20 

  2 3 30 

  3 4 20 

  4 5 20 

  1 5 100 

  Sample Output 

  90 

  Hint 

  INPUT DETAILS:  

  There are five landmarks.  

  OUTPUT DETAILS:  

  Bessie can get home by following trails 4, 3, 2, and 1.
最短路 用DIjkstra模板搞一搞

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#define _clr(x,a) memset(x,a,sizeof(x))
const int N=1050;
const int INF=0x3f3f3f3f;
int g[N][N];
int lowcost[N];
bool vis[N];
int n,t;
void Dijkstra(int s){
    for(int i=1;i<=n;i++){
        lowcost[i]=INF;
        vis[i]=false;
    }
    lowcost[s]=0;
    for(int i=1;i<=n;i++){
        int k=-1;
        int Min=INF;
        for(int j=1;j<=n;j++){
            if(!vis[j] && lowcost[j]<Min){
                Min=lowcost[j];
                k=j;
            }
        }
        if(k==-1){
            break;
        }
        vis[k]=true;
        for(int j=1;j<=n;j++){
            if(!vis[j] && lowcost[k]+g[k][j]<lowcost[j]){
                lowcost[j]=lowcost[k]+g[k][j];
            }
        }
    }
}
int main(void){
    //freopen("data.txt","r",stdin);
    _clr(g,INF);
    int u,v,w;
    scanf("%d%d",&t,&n);
    while(t--){
        scanf("%d%d%d",&u,&v,&w);
        if(w<g[u][v]){
            g[u][v]=g[v][u]=w;
        }
    }
    Dijkstra(n);
    printf("%d\n",lowcost[1]);
    return 0;
}
```





