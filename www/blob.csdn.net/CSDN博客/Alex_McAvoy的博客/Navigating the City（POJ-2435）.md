# Navigating the City（POJ-2435） - Alex_McAvoy的博客 - CSDN博客





2018年07月24日 23:43:47[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：62
个人分类：[POJ																[图论——图的搜索](https://blog.csdn.net/u011815404/article/category/8237087)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

A dip in the milk market has forced the cows to move to the city. The only employment available is in the venerable field of taxi-driving. Help the cows learn their way around the city. 

Given a city map with E (1 <= E <= 40) east/west street locations and N (1 <= N <= 30) north/south street locations, create instructions for a taxi driver to navigate from the start of his route (marked 'S') to the end (marked 'E'). Each instruction is a direction (one of 'N', 'E', 'S', or 'W') followed by a space followed by an integer that tells how many blocks to drive in that direction. If multiple routes are available, your program should output the shortest route. The shortest route is guaranteed to exist and be unique. 

The map is depicted as a grid of '+'s that represent intersections and a set of roads depicted as '-' and '|'. Buildings and other obstacles are shown as '.'s. Here is a typical map: 

+-+-+.+-+-+

|...|.....|

+-+.+-+-+-+

..|.......|

S-+-+-+.E-+

The taxi should go east, north, west, north, east two blocks, and so on. See the output format and sample solution below for its complete route.

# **Input**

* Line 1: Two space-separated integers, N and E. 

* Lines 2..2*N: These lines each contain 2*E-1 characters and encode the map of the street. Every other input line gives the data for the east/west streets; the remaining lines show the north/south streets. The format should be clear from the example.

# Output

* Lines 1..?: Each line contains a direction letter and a number of blocks to travel in that direction.

# Sample Input

**3 6+-+-+.+-+-+|...|.....|+-+.+-+-+-+..|.......|S-+-+-+.E-+**

# Sample Output

**E 1N 1W 1N 1E 2S 1E 3S 1W 1**


题意：给出一张东西向街道数为 N ，南北向街道数为 E 的地图，" . " 是障碍物，" - " 与 " | " 是可行的路，" + " 是十字路口，从始点 S 出发到达终点 E，以 街道+方向 的形式输出最短路线。

思路：bfs 搜索即可，唯一的难点在于输出，用一个队列来记录路径，然后根据队列存储的路径进行匹配输出。

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
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 1001
#define MOD 123
#define E 1e-6
using namespace std;
int n,e;
char g[N][N];
char path[N*N];
int vis[N][N];
int dir[4][2]={{1,0},{-1,0},{0,1},{0,-1}};
struct Node{
    int x;
    int y;
}pre[N][N];
queue<Node> q;
char get_dir(int x,int y,int turn_x,int turn_y)//获取转向方向
{
    for(int i=0;i<4;i++)
    {
        int nx=x+dir[i][0];
        int ny=y+dir[i][1];

        if(nx==turn_x&&ny==turn_y)
        {
            if(i==0)
                return 'E';
            else if(i==1)
                return 'W';
            else if(i==2)
                return 'S';
            else
                return 'N';
        }
    }
    return 0;
}
void bfs(int x,int y)
{
    q.push({y,x});//压入始点
    pre[x][y].x=-1;
    pre[x][y].y=-1;
    vis[x][y]=1;

    while(!q.empty())
    {
        Node pos=q.front();
        q.pop();

        if(g[pos.y][pos.x]=='E')
            break;

        for(int i=0;i<4;i++)
        {
            int nx=pos.x+dir[i][0];
            int ny=pos.y+dir[i][1];

            if(nx<0||nx>=e||ny<0||ny>=n||vis[ny][nx]||g[ny][nx]=='.')//越界判断
                continue;

            pre[ny][nx]=pos;
            vis[ny][nx]=1;
            q.push({nx,ny});//将路径压入队列
        }
    }
}
void print(int x,int y)
{
    int cnt=0;
    Node pos=pre[x][y],last;

    last.x=y;
    last.y=x;

    while(pos.x!=-1&&pos.y!=-1)
    {
        path[cnt++]=get_dir(pos.x,pos.y,last.x,last.y);//获取行进方向
        while(g[pos.y][pos.x]=='-' || g[pos.y][pos.x]=='|')
        {
            last=pos;
            pos=pre[last.y][last.x];
        }

        last=pos;
        pos=pre[last.y][last.x];
    }

    for(int i=cnt-1;i>=0;i--)
    {
        int sum=0;
        char last_dir=path[i];
        while(i>=0&&last_dir==path[i])//如果转向方向与路径方向相同
        {
            i--;
            sum++;
        }

        printf("%c %d\n",last_dir,sum);

        if(last_dir!=path[i])//如果转向方向与路径方向不同
            i++;
    }

}
int main()
{
    scanf("%d%d",&n,&e);

    n=n*2-1;//图的真实横长
    e=e*2-1;//图的真实纵长

    for(int i=0;i<n;i++)
        scanf("%s",g[i]);

    int start_x,start_y,end_x,end_y;
    for(int i=0;i<n;i++)//记录始点与终点
    {
        for(int j=0;j<e;j++)
        {
            if(g[i][j]=='S')
            {
                start_x=i;
                start_y=j;
            }
            if(g[i][j]=='E')
            {
                end_x=i;
                end_y=j;
            }
        }
    }

    bfs(start_x,start_y);//bfs搜索

    print(end_x,end_y);//输出

    return 0;
}
```






