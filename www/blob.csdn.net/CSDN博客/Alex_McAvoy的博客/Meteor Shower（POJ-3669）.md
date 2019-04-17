# Meteor Shower（POJ-3669） - Alex_McAvoy的博客 - CSDN博客





2018年06月07日 23:47:41[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：66
个人分类：[POJ																[图论——图的搜索](https://blog.csdn.net/u011815404/article/category/8237087)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

Bessie hears that an extraordinary meteor shower is coming; reports say that these meteors will crash into earth and destroy anything they hit. Anxious for her safety, she vows to find her way to a safe location (one that is never destroyed by a meteor) . She is currently grazing at the origin in the coordinate plane and wants to move to a new, safer location while avoiding being destroyed by meteors along her way.

The reports say that M meteors (1 ≤ M ≤ 50,000) will strike, with meteor i will striking point (Xi, Yi) (0 ≤ Xi ≤ 300; 0 ≤ Yi ≤ 300) at time Ti (0 ≤ Ti  ≤ 1,000). Each meteor destroys the point that it strikes and also the four rectilinearly adjacent lattice points.

Bessie leaves the origin at time 0 and can travel in the first quadrant and parallel to the axes at the rate of one distance unit per second to any of the (often 4) adjacent rectilinear points that are not yet destroyed by a meteor. She cannot be located on a point at any time greater than or equal to the time it is destroyed).

Determine the minimum time it takes Bessie to get to a safe place.

# **Input**

Line 1: A single integer: M

Lines 2..M+1: Line i+1 contains three space-separated integers: Xi, Yi, and Ti

# Output

Line 1: The minimum time it takes Bessie to get to a safe place or -1 if it is impossible.

# Sample Input

**4**

**0 0 22 1 21 1 20 3 5**

# Sample Output

**5**


题意：一共有n颗流星，给出落下的坐标（xi，yi）和时间ti，每一颗流星落地时上下左右的格子被毁灭，开始时人在（0,0）点，了防止被砸中，需要移动到安全位置。

思路：先建图，储存好每个点遭受流星打击的时间，不受打击的点用-1，因为开始时可能遭受打击，然后bfs遍历整张图，找不受打击的点即可，因为要求最短时间，注意判重。

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
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 340
#define MOD 123
#define E 1e-6
using namespace std;
int g[N][N];
int dir[5][2]={{0,0},{1,0},{-1,0},{0,1},{0,-1}};//注意一开始也可能被砸到，因此要加一个(0,0)
int vis[N][N];
struct Node
{
    int x;
    int y;
    int step;
};

int bfs()
{
    if(g[0][0]==0)
        return -1;
    if(g[0][0]==-1)
        return 0;

    queue<Node>q;
    Node a,temp;
    temp.x=0;
    temp.y=0;
    temp.step=0;
    q.push(temp);

    while(!q.empty())
    {
        a=q.front();
        q.pop();
        for(int i=1;i<5;i++)
        {
            temp.x=a.x+dir[i][0];
            temp.y=a.y+dir[i][1];
            temp.step=a.step+1;

            if(temp.x<0||temp.y<0||temp.x>=N||temp.y>=N)
                continue;
            if(g[temp.x][temp.y]==-1)//到达安全地带退出
                return temp.step;
            if(temp.step>=g[temp.x][temp.y])//到达的时间大于等于被毁坏的时间不行
                continue;
            g[temp.x][temp.y]=temp.step;//更新步数
            q.push(temp);

        }
    }
    return -1;
}

int main()
{
    int n;
    while(scanf("%d",&n)!=EOF)
    {
        memset(g,-1,sizeof(g));
        while(n--)
        {
            int x,y,t;
            scanf("%d%d%d",&x,&y,&t);
            if(g[x][y]==-1)//受到撞击后更新时间
                g[x][y]=t;
            else
                g[x][y]=min(g[x][y],t);

            for(int i=0;i<5;i++)//地图构建
            {
                int nx=x+dir[i][0];
                int ny=y+dir[i][1];
                if(nx<0||nx>=N||ny<0||ny>=N)
                    continue;
                if(g[nx][ny]==-1)
                    g[nx][ny]=t;
                else
                    g[nx][ny]=min(t,g[nx][ny]);

            }
        }
        int res=bfs();
        printf("%d\n",res);
    }
    return 0;
}
```






