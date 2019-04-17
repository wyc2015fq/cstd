# T1250 The Castle（#Ⅱ- 8） - Alex_McAvoy的博客 - CSDN博客





2018年05月26日 12:27:54[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：503








> 
# 【题目描述】

一座城堡被分成ｍ*ｎ个方块（m≤50，n≤50），每个方块可有0～4堵墙（0表示无墙）。下面示出了建筑平面图：

![](http://ybt.ssoier.cn:8088/pic/1250.gif)

图中的加粗黑线代表墙。几个连通的方块组成房间，房间与房间之间一定是用黑线（墙）隔开的。

现在要求你编一个程序，解决以下2个问题：

    1、该城堡中有多少个房间?

    2、最大的房间有多大?

# 【输入】

平面图用一个数字表示一个方块(第1个房间用二进制1011表示，0表示无东墙，用十进制11表示)。

    第一行一个整数m（m≤50），表示房子南北方向的长度。

    第二行一个整数n（n≤50），表示房子东西方向的长度。

    后面的m行，每行有n个整数，每个整数都表示平面图对应位置的方块的特征。每个方块中墙的特征由数字P来描述（0≤P≤15）。数字P是下面的可能取的数字之和：

    1（西墙 west）

    2（北墙 north）

    4（东墙 east）

    8（南墙 south）

室内的墙被定义两次： 例如方块（1，1）中的南墙也被位于其南面的方块（2，1）定义了一次。

    建筑中至少有两个房间。

# 【输出】

第1行：一个整数，表示房间总数；

第2行：一个整数，表示最大房间的面积（方块数）。

# 【输入样例】

**4711 6 11  6  3 10  67  9  6 13  5 15  51 10 12  7 13  7  513 11 10 8 10 12 13**

# 【输出样例】

**59**


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
#define N 51
#define MOD 2520
#define E 1e-12
using namespace std;
int n,m;
int a[N][N];
int b[4]={1,2,4,8};
bool vis[N][N];
int sum,maxx;
int dir[4][2]={{0,-1},{-1,0},{0,1},{1,0}};
struct node
{
    int x;
    int y;
}q[N*100];
void bfs(int x0,int y0)
{
    int head=1,tail=1;
    int cnt=1;

    vis[x0][y0]=1;
    q[tail].x=x0;
    q[tail].y=y0;
    tail++;

    while(head<tail)
    {
        int x=q[head].x;
        int y=q[head].y;
        for(int i=0;i<4;i++)
        {
            int nx=x+dir[i][0];
            int ny=y+dir[i][1];
            if(nx>=1&&nx<=n&&ny>=1&&ny<=m&&vis[nx][ny]==0&&(a[x][y]&b[i])==0)
            {
                cnt++;
                vis[nx][ny]=1;
                q[tail].x=nx;
                q[tail].y=ny;
                tail++;
            }
        }
        head++;
    }
    if(cnt>maxx)
        maxx=cnt;
    sum++;
}
int main()
{
    memset(vis,0,sizeof(vis));
    scanf("%d%d",&n,&m);
    for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)
            scanf("%d",&a[i][j]);

     for(int i=1;i<=n;i++)
        for(int j=1;j<=m;j++)
            if(vis[i][j]==0)
                bfs(i,j);
    printf("%d\n%d\n",sum,maxx);
    return 0;
}
```





