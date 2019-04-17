# T1257 Knight Moves（#Ⅱ- 8） - Alex_McAvoy的博客 - CSDN博客





2018年05月28日 20:33:39[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：566








### 【题目描述】



输入n代表有个n×n的棋盘，输入开始位置的坐标和结束位置的坐标，问一个骑士朝棋盘的八个方向走马字步，从开始坐标到结束坐标可以经过多少步。



![](http://ybt.ssoier.cn:8088/pic/1257.gif)



### 【输入】



首先输入一个n,表示测试样例的个数。

每个测试样例有三行。

第一行是棋盘的大小L(4≤L≤300)；

第二行和第三行分别表示马的起始位置和目标位置(0..L−1)。

### 【输出】

马移动的最小步数，起始位置和目标位置相同时输出0。

### 【输入样例】

```
3
8
0 0
7 0
100
0 0
30 50
10
1 1
1 1
```

### 【输出样例】

```
5
28
0
```

### 【源程序】

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
#define N 310
#define MOD 2520
#define E 1e-12
using namespace std;
int n;
char a[N][N];
bool vis[N][N];
int dir[8][2]={{-2,1},{-1,2},{1,2},{2,1},{2,-1},{1,-2},{-1,-2},{-2,-1}};
struct node
{
    int x;
    int y;
    int step;
}q[100000];
void bfs(int sx,int sy,int ex,int ey)
{
    int head=1,tail=1;
    memset(vis,0,sizeof(vis));

    vis[sx][sy]=1;
    q[tail].x=sx;
    q[tail].y=sy;
    q[tail].step=0;
    tail++;

    while(head<tail)
    {
        int x=q[head].x;
        int y=q[head].y;
        int step=q[head].step;
        if(x==ex&&y==ey)
        {
            printf("%d\n",step);
            break;
        }
        for(int i=0;i<8;i++)
        {
            int nx=x+dir[i][0];
            int ny=y+dir[i][1];
            if(nx>=0&&nx<n&&ny>=0&&ny<n&&vis[nx][ny]==0)
            {
                vis[nx][ny]=1;
                q[tail].x=nx;
                q[tail].y=ny;
                q[tail].step=step+1;
                tail++;
            }
        }
        head++;
    }
}
int main()
{
    int t;
    int sx,sy,ex,ey;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d",&n);
        scanf("%d%d%d%d",&sx,&sy,&ex,&ey);
        bfs(sx,sy,ex,ey);
    }


    return 0;
}
```





