# 表白记 BFS求最短路径 - 紫芝的博客 - CSDN博客





2018年04月04日 21:48:39[紫芝](https://me.csdn.net/qq_40507857)阅读数：140








单身的1暗恋上了一个女生，于是想给她告白，于是就在房间里用蛋糕堆了一个心的形状。

可是天公不作美，在这个房间的某个角落里藏着一只小老鼠，小老鼠虎视眈眈的看着这些蛋糕，想等1走之后去偷吃蛋糕。

一个房间可以看成n*n的方格。小老鼠可以往上、下、左、右四个方向走。问小老鼠吃到蛋糕最少需要多少步？

## Input

本题有多组测试，每组测试第一行输入三个正整数n，x，y。n代表房间的长宽，(x,y)代表老鼠洞的位置

（老鼠出现在老鼠洞这个点也算一步）。接下来n行是房间的布置， ’#’代表蛋糕，’.’代表空地。其中(x,y)一定满足位于房间的边上。

(7 ≤n ≤ 99,1 ≤ x,y ≤ n）


## Output

对于每组测试数据输出一个整数，占一行。

## Sample Input

11 1 8
...........
...........
...#...#...
..###.###..
.#########.
.#########.
..#######..
...#####...
....###....
.....#.....
...........
9 7 9
.........
.........
..##.##..
.#######.
.#######.
..#####..
...###...
....#....

.........

## Sample Output

3

4

思路：

BFS求最短路模板题：

在一个二维平面内，有路和障碍物，已知起点和终点，求从起点到终点的最短路

```cpp
#include<bits/stdc++.h>
using namespace std;
struct node {
    int x,y,step;
};
int n,x,y;
int ans;
char mp[100][100];
bool vis[100][100];
int to[4][2]={1,0,-1,0,0,1,0,-1};
bool check(int x,int y)
{
    if(x<0||y<0||x>=n||y>=n||vis[x][y])
        return 1;
    return 0;
}
void bfs()
{
    node start,next;
    start.x=x;
    start.y=y;
    start.step=0;
    vis[x][y]=1;
    queue<node> q;
    q.push(start);
    while(!q.empty()){
        node temp=q.front();
        q.pop();
        vis[temp.x][temp.y]=1;
        if(mp[temp.x][temp.y]=='#')
        {
            ans=temp.step;
            return;
        }
        for(int i=0;i<4;i++)
        {
        	next=temp;
        	next.x+=to[i][0];
        	next.y+=to[i][1];
        	if(check(next.x,next.y))
        	continue;
        	next.step++;
        	vis[next.x][next.y]=1;
        	q.push(next);
		}
    }
    ans=-1;
}
int main()
{
    while(cin>>n>>x>>y){
    	x--;y--;
       for(int i=0;i<n;i++)
            scanf("%s",mp[i]);
    fill(vis[0],vis[0]+100*100,0);
             bfs();
    cout<<ans+1<<endl;
    }
   return 0;
}
```




