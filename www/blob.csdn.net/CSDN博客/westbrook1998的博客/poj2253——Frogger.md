# poj2253——Frogger - westbrook1998的博客 - CSDN博客





2018年08月18日 18:51:38[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：30








> 
Freddy Frog is sitting on a stone in the middle of a lake. Suddenly he notices Fiona Frog who is sitting on another stone. He plans to visit her, but since the water is dirty and full of tourists’ sunscreen, he wants to avoid swimming and instead reach her by jumping.  

  Unfortunately Fiona’s stone is out of his jump range. Therefore Freddy considers to use other stones as intermediate stops and reach her by a sequence of several small jumps.  

  To execute a given sequence of jumps, a frog’s jump range obviously must be at least as long as the longest jump occuring in the sequence.  

  The frog distance (humans also call it minimax distance) between two stones therefore is defined as the minimum necessary jump range over all possible paths between the two stones.  

  You are given the coordinates of Freddy’s stone, Fiona’s stone and all other stones in the lake. Your job is to compute the frog distance between Freddy’s and Fiona’s stone.  

  Input 

  The input will contain one or more test cases. The first line of each test case will contain the number of stones n (2<=n<=200). The next n lines each contain two integers xi,yi (0 <= xi,yi <= 1000) representing the coordinates of stone #i. Stone #1 is Freddy’s stone, stone #2 is Fiona’s stone, the other n-2 stones are unoccupied. There’s a blank line following each test case. Input is terminated by a value of zero (0) for n. 

  Output 

  For each test case, print a line saying “Scenario #x” and a line saying “Frog Distance = y” where x is replaced by the test case number (they are numbered from 1) and y is replaced by the appropriate real number, printed to three decimals. Put a blank line after each test case, even after the last one. 

  Sample Input 

  2 

  0 0 

  3 4 

  3 

  17 4 

  19 4 

  18 5 

  0 

  Sample Output 

  Scenario #1 

  Frog Distance = 5.000 

  Scenario #2 

  Frog Distance = 1.414
这题上次练最小生成树好像也见过 wa了十几次 这次又碰到了 还是wa了好多次 

题意是要求从起点到终点的所有路径中，每条路径中最长边的最小值，就是先是每条路径的最长边，就是青蛙必须要跳的距离，然后要保证这个距离最小 

用了个变形的dijkstra 

主要是这个lowcost数组意义的不同以及更新的时候的不同 
`lowcost[j]=min(lowcost[j],max(lowcost[k],g[k][j]));`

这里lowcost记录的是起点到i这段路径里最长的一条边 

这里有max和min 

max是表示当前这条起点到j的路径（因为可能会有多条路径，这里表示当前这一条，即经过k的着一条），分成两段，从起点到k，k到j的距离，求出路径最长边的距离，所以用max 

而min是用来和其他从起点到j的边比较的，找出最长边最短的那一条
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cmath>
#define _clr(x,a) memset(x,a,sizeof(x))
using namespace std;
const int N=250;
const double INF=0x3f3f3f3f;
double g[N][N];
double lowcost[N];
bool vis[N];
double x[N];
double y[N];
int n,t;
void Dijkstra(int s){
    for(int i=1;i<=n;i++){
        lowcost[i]=INF;
        vis[i]=false;
    }
    lowcost[s]=0;
    for(int i=1;i<=n;i++){
        double Min=INF;
        int k=-1;
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
            //与求最短路不同
            //lowcost[j]记录的是j到起点的路径上的最大边权
            // if(!vis[j] && lowcost[j]>max(lowcost[k],g[k][j])){
            //     lowcost[j]=max(lowcost[k],g[k][j]);
            // }
            lowcost[j]=min(lowcost[j],max(lowcost[k],g[k][j]));
        }
    }
}
int main(void){
    //freopen("data.txt","r",stdin);
    int c=1;
    while(~scanf("%d",&n) && n){
        _clr(g,0.0);
        for(int i=1;i<=n;i++){
            scanf("%lf%lf",&x[i],&y[i]);
        }
        for(int i=1;i<n;i++){
            for(int j=i+1;j<=n;j++){
                g[i][j]=g[j][i]=sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j]));
            }
        }
        Dijkstra(1);
        printf("Scenario #%d\nFrog Distance = %.3f\n\n",c++,lowcost[2]);
    }
    return 0;
}
```






