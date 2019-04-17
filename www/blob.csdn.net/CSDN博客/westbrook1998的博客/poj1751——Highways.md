# poj1751——Highways - westbrook1998的博客 - CSDN博客





2018年08月13日 22:08:20[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：38








> 
The island nation of Flatopia is perfectly flat. Unfortunately, Flatopia has a very poor system of public highways. The Flatopian government is aware of this problem and has already constructed a number of highways connecting some of the most important towns. However, there are still some towns that you can’t reach via a highway. It is necessary to build more highways so that it will be possible to drive between any pair of towns without leaving the highway system.  

  Flatopian towns are numbered from 1 to N and town i has a position given by the Cartesian coordinates (xi, yi). Each highway connects exaclty two towns. All highways (both the original ones and the ones that are to be built) follow straight lines, and thus their length is equal to Cartesian distance between towns. All highways can be used in both directions. Highways can freely cross each other, but a driver can only switch between highways at a town that is located at the end of both highways.  

  The Flatopian government wants to minimize the cost of building new highways. However, they want to guarantee that every town is highway-reachable from every other town. Since Flatopia is so flat, the cost of a highway is always proportional to its length. Thus, the least expensive highway system will be the one that minimizes the total highways length.  

  Input 

  The input consists of two parts. The first part describes all towns in the country, and the second part describes all of the highways that have already been built.  

  The first line of the input file contains a single integer N (1 <= N <= 750), representing the number of towns. The next N lines each contain two integers, xi and yi separated by a space. These values give the coordinates of i th town (for i from 1 to N). Coordinates will have an absolute value no greater than 10000. Every town has a unique location.  

  The next line contains a single integer M (0 <= M <= 1000), representing the number of existing highways. The next M lines each contain a pair of integers separated by a space. These two integers give a pair of town numbers which are already connected by a highway. Each pair of towns is connected by at most one highway.  

  Output 

  Write to the output a single line for each new highway that should be built in order to connect all towns with minimal possible total length of new highways. Each highway should be presented by printing town numbers that this highway connects, separated by a space.  

  If no new highways need to be built (all towns are already connected), then the output file should be created but it should be empty.  

  Sample Input 

  9 

  1 5 

  0 0  

  3 2 

  4 5 

  5 1 

  0 4 

  5 2 

  1 2 

  5 3 

  3 

  1 3 

  9 7 

  1 2 

  Sample Output 

  1 6 

  3 7 

  4 9 

  5 7 

  8 3
继续最小生成树的练习 

这题是给n个点的坐标，然后给出已有的边，然后求要添加的边构成最小生成树 

思路是用一个pre数组来记录前驱元，然后每次找到一个加入最小生成树的点就判断该点和他的前驱元是否已经在输入的路径中，如果否，就输出该路径
代码：

```
#include <cstdio>
//#include <algorithm>
#include <cstring>
#include <cmath>
#define _clr(x,a) memset(x,a,sizeof(x))
using namespace std;
const int N=755;
const int INF=0x3f3f3f3f;
int x[N];
int y[N];
int n;
int r;
int a,b;
int cnt;
int g[N][N];
bool vis[N];
int pre[N];
int lowcost[N];
int roads[1005][2];
void init(){
    _clr(x,0);
    _clr(y,0);
    _clr(g,INF);
    _clr(lowcost,0);
    _clr(vis,false);
}
int distance(int i,int j){
    return (x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j]);
}
void prim(){
    int pos=1;
    int Min=INF;
    for(int i=1;i<=n;i++){
        if(i!=pos){
            lowcost[i]=g[pos][i];
        }
        pre[i]=1;
    }
    vis[pos]=true;
    for(int i=1;i<n;i++){
        Min=INF;
        for(int j=1;j<=n;j++){
            if(!vis[j] && lowcost[j]<Min){
                pos=j;
                Min=lowcost[j];
            }
        }
        int flag=0;
        if(!vis[pos]){
            for(int k=0;k<cnt;k++){
                if((pre[pos]==roads[k][0] && pos==roads[k][1]) || (pre[pos]==roads[k][1] && pos==roads[k][0])){
                    flag=1;
                    break;
                }
            }
            if(!flag){
                printf("%d %d\n",pre[pos],pos);
            }
        }
        vis[pos]=true;
        for(int j=1;j<=n;j++){
            if(!vis[j] && lowcost[j]>g[pos][j]){
                lowcost[j]=g[pos][j];
                pre[j]=pos;
            }
        }
    }
}
int main(void){
    //freopen("data.txt","r",stdin);
    scanf("%d",&n);
    init();
    for(int i=1;i<=n;i++){
        scanf("%d%d",&x[i],&y[i]);
    }
    for(int i=1;i<=n-1;i++){
        for(int j=i+1;j<=n;j++){
            g[i][j]=g[j][i]=distance(i,j);
        }
    }
    scanf("%d",&r);
    while(r--){
        scanf("%d%d",&a,&b);
        g[a][b]=g[b][a]=0;
        roads[cnt][0]=a;
        roads[cnt++][1]=b;
    }
    prim();
    return 0;
}
```






