# 2253（Dijkstra变形） - PeterBishop - CSDN博客





2019年01月21日 12:58:24[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：16








题意：

给出两只青蛙的坐标A、B，和其他的n-2个坐标，任一两个坐标点间都是双向连通的。显然从A到B存在至少一条的通路，每一条通路的元素都是这条通路中前后两个点的距离，这些距离中又有一个最大距离。现在要求求出所有通路的最大距离，并把这些最大距离作比较，把最小的一个最大距离作为青蛙的最小跳远距离。lowcost[i]为起点s到当前点i的路径上最小的最大边；

```cpp
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cmath>
using namespace std;

const int INF = 0x3f3f3f3f;
const int MAXN = 210;
struct Point
{
   int x,y;
};

Point point[MAXN];
bool vis[MAXN];
double cost[MAXN][MAXN];
double lowcost[MAXN];

double dist(Point a,Point b)
{
    return sqrt((double)((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)));
}

void Dijkstra(double lowcost[],double cost[][MAXN],int n,int beg)
{
     for(int i=1;i<=n;i++)
     {
        vis[i]=false;
        lowcost[i]=INF;
     }
     lowcost[beg]=0;
     for(int j=0;j<n;j++)
     {
        int k = -1;
        int MIN = INF;
        for(int i=1;i<=n;i++)
        {
            if(!vis[i]&&lowcost[i]<MIN)
            {
                MIN = lowcost[i];
                k=i;
            }
        }
        if(k==-1) break;
        vis[k]=true;
        for(int i=1;i<=n;i++)
        {
            lowcost[i]=min(lowcost[i],max(lowcost[k],cost[k][i]));//找路的时候找最大的，记录的时候记录最小的，不需要加起来不求总和
        }
     }
}


int main()
{
    int T=0;
    int n;
    while(cin>>n&&n)
    {
        for(int i=1;i<=n;i++)
        {
            cin>>point[i].x>>point[i].y;
        }
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
        {
            cost[j][i]=cost[i][j]=dist(point[i],point[j]);
        }
        Dijkstra(lowcost,cost,n,1);
        if(T) cout<<endl;
        printf("Scenario #%d\nFrog Distance = %.3lf\n", ++T, lowcost[2]);
    }
    return 0;
}
```





