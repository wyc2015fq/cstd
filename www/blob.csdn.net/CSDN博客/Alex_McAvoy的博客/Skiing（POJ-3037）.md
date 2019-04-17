# Skiing（POJ - 3037） - Alex_McAvoy的博客 - CSDN博客





2018年07月25日 19:18:44[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：53
个人分类：[POJ																[图论——BellmanFord_SPFA](https://blog.csdn.net/u011815404/article/category/8312431)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

Bessie and the rest of Farmer John's cows are taking a trip this winter to go skiing. One day Bessie finds herself at the top left corner of an R (1 <= R <= 100) by C (1 <= C <= 100) grid of elevations E (-25 <= E <= 25). In order to join FJ and the other cows at a discow party, she must get down to the bottom right corner as quickly as she can by travelling only north, south, east, and west. 

Bessie starts out travelling at a initial speed V (1 <= V <= 1,000,000). She has discovered a remarkable relationship between her speed and her elevation change. When Bessie moves from a location of height A to an adjacent location of eight B, her speed is multiplied by the number 2^(A-B). The time it takes Bessie to travel from a location to an adjacent location is the reciprocal of her speed when she is at the first location. 

Find the both smallest amount of time it will take Bessie to join her cow friends. 

# **Input**

* Line 1: Three space-separated integers: V, R, and C, which respectively represent Bessie's initial velocity and the number of rows and columns in the grid. 

* Lines 2..R+1: C integers representing the elevation E of the corresponding location on the grid.

# Output

A single number value, printed to two exactly decimal places: the minimum amount of time that Bessie can take to reach the bottom right corner of the grid.

# Sample Input

**1 3 31 5 36 3 52 4 3**

# Sample Output

**29.00**


题意：一头牛要从左上角到右下角，每次移动的时间与移动前的速度 v 以及高度差 Ha-Hb 有关，即：1 / v*2^(Ha-Hb)，求最短时间。

思路：从一点到下一点的时间是由前一个点的速度决定的，初始速度是固定的，因而速度的改变只与两点的高度差有关，易得出每一个点的速度实际上都是固定的，因此每一个点到其他点的时间都是确定的，借此建图，再套用 SPFA 算法解决即可。

唯一需要注意的是松弛情况，0x3f3f3f3f 过不了样例，改成 0x7f7f7f7f 即可。

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<set>
#include<map>
#include<stack>
#include<vector>
#define INF 0x7f7f7f7f
#define PI acos(-1.0)
#define N 1001
#define MOD 123
#define E 1e-6
using namespace std;
double v;
int r,c;
int h[N][N];
int vis[N][N];
double g[N][N],dis[N][N];
int dir[4][2]={{0,1},{0,-1},{1,0},{-1,0}};
struct Node{
    int x;
    int y;
}now,next;
void SPFA(int x,int y)
{
    for(int i=1;i<=r;i++)
	   for(int j=1;j<=c;j++)
	   {
	   	   dis[i][j]=INF;
		   vis[i][j]=0;
	   }
    //memset(vis,0,sizeof(vis));
    //memset(dis,INF,sizeof(dis));
    vis[x][y]=1;
    dis[x][y]=0;

    queue<Node> q;
    now.x=x;
    now.y=y;
    q.push(now);

    while(!q.empty())
    {
        now=q.front();
        q.pop();
        vis[now.x][now.y]=0;

        for(int i=0;i<4;i++)
        {
            next.x=now.x+dir[i][0];
            next.y=now.y+dir[i][1];

            if(next.x<1||next.x>r||next.y<1||next.y>c)
                continue;

            double w=1.0/g[now.x][now.y];
            if(dis[now.x][now.y]+w<dis[next.x][next.y])
            {
                dis[next.x][next.y]=dis[now.x][now.y]+w;
                if(!vis[next.x][next.y])
                {
                    q.push(next);
                    vis[next.x][next.y]=1;
                }
            }
        }
    }
}
int main()
{
    scanf("%lf%d%d",&v,&r,&c);

    for(int i=1;i<=r;i++)
    {
        for(int j=1;j<=c;j++)
        {
            scanf("%d",&h[i][j]);//输入每点的高度
            g[i][j]=v*pow( 2.0,double(h[1][1])-double(h[i][j]) );//建图
        }
    }
    g[1][1]=v;

    SPFA(1,1);

    printf("%.2f\n",dis[r][c]);

    return 0;
}
```






