# T1248 Dungeon Master（#Ⅱ- 8） - Alex_McAvoy的博客 - CSDN博客





2018年05月26日 12:33:09[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：455








> 
# 【题目描述】

这题是一个三维的迷宫题目，其中用‘.’表示空地，‘#’表示障碍物，‘S’表示起点，‘E’表示终点，求从起点到终点的最小移动次数，解法和二维的类似，只是在行动时除了东南西北移动外还多了上下。可以上下左右前后移动，每次都只能移到相邻的空位，每次需要花费一分钟，求从起点到终点最少要多久。

# 【输入】

多组测试数据。

一组测试测试数据表示一个三维迷宫：

前三个数，分别表示层数、一个面的长和宽，后面是每层的平面图。前三个数据为三个零表示结束。

# 【输出】

最小移动次数。

# 【输入样例】

**3 4 5S.....###..##..###.#############.####...###########.#######E1 3 3S###E####0 0 0**

# 【输出样例】

**Escaped in 11 minute(s).Trapped!**


# 【源程序】

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 101
#define MOD 2520
#define E 1e-12
using namespace std;
int X,Y,Z;
char a[N][N][N];
bool vis[N][N][N];
int dir[6][3]={{1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1}};
struct node
{
    int x;
    int y;
    int z;
    int step;
}q[N*10000];
void bfs(int sx,int sy,int sz,int ex,int ey,int ez)
{
    int head=1,tail=1;
    bool flag=true;
    memset(vis,0,sizeof(vis));

    vis[sx][sy][sz]=1;
    q[tail].x=sx;
    q[tail].y=sy;
    q[tail].z=sz;
    q[tail].step=0;
    tail++;

    while(head<tail)
    {
        int x=q[head].x;
        int y=q[head].y;
        int z=q[head].z;
        int step=q[head].step;
        if(x==ex&&y==ey&&z==ez)
        {
            flag=false;
            printf("Escaped in %d minute(s).\n",step);
            break;
        }
        for(int i=0;i<6;i++)
        {
            int nx=x+dir[i][0];
            int ny=y+dir[i][1];
            int nz=z+dir[i][2];
            if(0<=nx&&nx<X&&0<=ny&&ny<Y&&0<=nz&&nz<Z&&a[nx][ny][nz]=='.'&&vis[nx][ny][nz]==0)
            {
                vis[nx][ny][nz]=1;
                q[tail].x=nx;
                q[tail].y=ny;
                q[tail].z=nz;
                q[tail].step=step+1;
                tail++;
            }
        }
        head++;
    }
    if(flag)
        printf("Trapped!\n");

}
int main()
{
    int sx,sy,sz,ex,ey,ez;
    while(scanf("%d%d%d",&X,&Y,&Z)!=EOF&&X&&Y&&Z)
    {
        for(int i=0;i<X;i++)
            for(int j=0;j<Y;j++)
                scanf("%s",a[i][j]);
        for(int i=0;i<X;i++)
            for(int j=0;j<Y;j++)
                for(int k=0;k<Z;k++)
                {
                    if(a[i][j][k]=='S')
                    {
                        sx=i;
                        sy=j;
                        sz=k;
                    }
                    if(a[i][j][k]=='E')
                    {
                        ex=i;
                        ey=j;
                        ez=k;
                        a[i][j][k]='.';
                    }
                }
        bfs(sx,sy,sz,ex,ey,ez);
    }
    return 0;
}
```





