# Mud Puddles（POJ-3626） - Alex_McAvoy的博客 - CSDN博客





2018年06月29日 16:50:28[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：70








# Problem Description

Farmer John is leaving his house promptly at 6 AM for his daily milking of Bessie. However, the previous evening saw a heavy rain, and the fields are quite muddy. FJ starts at the point (0, 0) in the coordinate plane and heads toward Bessie who is located at (X, Y) (-500 ≤ X ≤ 500; -500 ≤ Y ≤ 500). He can see all N (1 ≤ N ≤ 10,000) puddles of mud, located at points (Ai, Bi) (-500 ≤ Ai ≤ 500; -500 ≤ Bi ≤ 500) on the field. Each puddle occupies only the point it is on.

Having just bought new boots, Farmer John absolutely does not want to dirty them by stepping in a puddle, but he also wants to get to Bessie as quickly as possible. He's already late because he had to count all the puddles. If Farmer John can only travel parallel to the axes and turn at points with integer coordinates, what is the shortest distance he must travel to reach Bessie and keep his boots clean? There will always be a path without mud that Farmer John can take to reach Bessie.


# **Input**

Line 1: Three space-separate integers: X, Y, and N.

Lines 2..N+1: Line i+1 contains two space-separated integers: Ai and Bi

# Output

Line 1: The minimum distance that Farmer John has to travel to reach Bessie without stepping in mud.

# Sample Input

**1 2 7**

**0 2-1 33 11 14 2-1 12 2**

# Sample Output

**11**

——————————————————————————————————————————————————————————————————————————————————————————

题意：起点（0，0）终点（x，y），给出n个不能走的点的坐标，求从起点到终点的最短路。

思路：bfs水题。。需要注意的是坐标范围 (Ai, Bi) (-500 ≤ Ai ≤ 500; -500 ≤ Bi ≤ 500)，存在负数，所有坐标加500再套 bfs 模版就行了。。。

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
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 1001
#define MOD 123
#define E 1e-6
using namespace std;
int g[N][N];
int dir[4][2]={{1,0},{-1,0},{0,1},{0,-1}};
struct Node
{
	int x;
	int y;
	int step;
};
void bfs(int x0,int y0)
{
    queue <Node> q;
    Node start,endd;
    
    start.x=500;
    start.y=500;
    start.step=0;
    q.push(start);
    
    while(!q.empty())
    {
        start=q.front();
        q.pop();
        
        if(start.x==x0&&start.y==y0)
        {
            printf("%d\n",start.step);
            return;
        }
        
        for(int i=0;i<4;i++)
        {
            int xx=start.x+dir[i][0];
            int yy=start.y+dir[i][1];
            if(xx>=0&&xx<=1000&&yy>=0&&yy<=1000&&g[xx][yy]==0)
            {
                endd.x=xx;
                endd.y=yy;
                endd.step=start.step+1;
                g[xx][yy]=1;
                q.push(endd);
            }
        }
    }
}
int main()
{
    int x,y,n;
    while(scanf("%d%d%d",&x,&y,&n)!=EOF)
    {
        x+=500;//横坐标x+500
        y+=500;//纵坐标y+500
        memset(g,0,sizeof(g));

        while(n--)
        {
            int a,b;
            scanf("%d%d",&a,&b);
            g[a+500][b+500]=1;
        }
        bfs(x,y);
    }
    return 0;
}
```





