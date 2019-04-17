# poj1797——Heavy Transportation - westbrook1998的博客 - CSDN博客





2018年08月18日 18:54:10[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：29标签：[最大的路径最短边																[最短路																[DIjkstra](https://so.csdn.net/so/search/s.do?q=DIjkstra&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)





> 
Background  

  Hugo Heavy is happy. After the breakdown of the Cargolifter project he can now expand business. But he needs a clever man who tells him whether there really is a way from the place his customer has build his giant steel crane to the place where it is needed on which all streets can carry the weight.  

  Fortunately he already has a plan of the city with all streets and bridges and all the allowed weights.Unfortunately he has no idea how to find the the maximum weight capacity in order to tell his customer how heavy the crane may become. But you surely know.  

  Problem  

  You are given the plan of the city, described by the streets (with weight limits) between the crossings, which are numbered from 1 to n. Your task is to find the maximum weight that can be transported from crossing 1 (Hugo’s place) to crossing n (the customer’s place). You may assume that there is at least one path. All streets can be travelled in both directions. 

  Input 

  The first line contains the number of scenarios (city plans). For each city the number n of street crossings (1 <= n <= 1000) and number m of streets are given on the first line. The following m lines contain triples of integers specifying start and end crossing of the street and the maximum allowed weight, which is positive and not larger than 1000000. There will be at most one street between each pair of crossings. 

  Output 

  The output for every scenario begins with a line containing “Scenario #i:”, where i is the number of the scenario starting at 1. Then print a single line containing the maximum allowed weight that Hugo can transport to the customer. Terminate the output for the scenario with a blank line. 

  Sample Input 

  1 

  3 3 

  1 2 3 

  1 3 4 

  2 3 5 

  Sample Output 

  Scenario #1: 

  4
这题和上面那题相反，求的是最大的路径中的最小边，基本和上一题相同，但在初始化的时候还是要注意

代码：

```cpp
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath>
#define _clr(x,a) memset(x,a,sizeof(x));
using namespace std;
const int N=1050;
const int INF=0x3f3f3f3f;
bool vis[N];
int cost[N];
int n,m;
int g[N][N];
void Dijkstra(int s){
    for(int i=1;i<=n;i++){
        vis[i]=false;
        cost[i]=g[s][i];
    }
    vis[s]=true;
    for(int i=1;i<n;i++){
        int Min=-INF;
        int k=-1;
        for(int j=1;j<=n;j++){
            if(!vis[j] && cost[j]>Min){
                Min=cost[j];
                k=j;
            }
        }
        if(k==-1){
            break;
        }
        vis[k]=true;
        for(int j=1;j<=n;j++){
            if(!vis[j]){
                cost[j]=max(cost[j],min(cost[k],g[k][j]));
            }
        }
    }
}
int main(void){
    //freopen("data.txt","r",stdin);
    int t;
    int c=1;
    int u,v,w;
    scanf("%d",&t);
    while(t--){
        _clr(g,0);
        scanf("%d%d",&n,&m);
        while(m--){
            scanf("%d%d%d",&u,&v,&w);
            g[u][v]=g[v][u]=w;
        }
        Dijkstra(1);
        printf("Scenario #%d:\n%d\n\n",c++,cost[n]);
    }
    return 0;
}
```](https://so.csdn.net/so/search/s.do?q=最短路&t=blog)](https://so.csdn.net/so/search/s.do?q=最大的路径最短边&t=blog)




