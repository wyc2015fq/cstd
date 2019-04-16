# POJ 3984 迷宫问题 BFS求最短路线+路径记录 - 紫芝的博客 - CSDN博客





2018年05月30日 10:59:43[紫芝](https://me.csdn.net/qq_40507857)阅读数：94








迷宫问题

|**Time Limit:** 1000MS||**Memory Limit:** 65536K|
|----|----|----|
|**Total Submissions:** 31050||**Accepted:** 17826|
Description

定义一个二维数组：
int maze[5][5] = {

	0, 1, 0, 0, 0,

	0, 1, 0, 1, 0,

	0, 0, 0, 0, 0,

	0, 1, 1, 1, 0,

	0, 0, 0, 1, 0,

};
它表示一个迷宫，其中的1表示墙壁，0表示可以走的路，只能横着走或竖着走，不能斜着走，要求编程序找出从左上角到右下角的最短路线。

Input

一个5 × 5的二维数组，表示一个迷宫。数据保证有唯一解。

Output

左上角到右下角的最短路径，格式如样例所示。

Sample Input
0 1 0 0 0
0 1 0 1 0
0 0 0 0 0
0 1 1 1 0
0 0 0 1 0
Sample Output
(0, 0)
(1, 0)
(2, 0)
(2, 1)
(2, 2)
(2, 3)
(2, 4)
(3, 4)
(4, 4)
```cpp
#include<cstdio>
#include<cstring>
using namespace std;
int mp[8][8],vis[8][8];
int ans;//输出从起点到终点的步数
int pre[50];//记录路径
struct node
{
    int x,y,step;
}q[110];//队列
int dic[4][2]={1,0,0,1,-1,0,0,-1};

bool check(int x,int y)
{
    if(x<0||x>=5||y<0||y>=5||vis[x][y]||mp[x][y])
        return 1;
    return 0;
}
//输出路径
void print(int head)
{
    int next=pre[head];
    if(next==0){
        printf("(0, 0)\n");
        printf("(%d, %d)\n",q[head].x,q[head].y);
        return;
    }else   print(next);
    printf("(%d, %d)\n",q[head].x,q[head].y);
}
void bfs()
{
    q[0].x=0;q[0].y=0;q[0].step=0;
    pre[0]=-1;
    //vis[0][0]=1;
    int head=0,tail=1;
    while(head<tail)
    {
        int x=q[head].x;
        int y=q[head].y;

        if(x==4&&y==4){
            ans=q[head].step;
            print(head);
            return;
        }
        for(int i=0;i<4;i++)
        {
            int xx=x+dic[i][0];
            int yy=y+dic[i][1];
            if(check(xx,yy))
                continue;
                vis[xx][yy]=1;
            q[tail].x=xx;
            q[tail].y=yy;
            q[tail].step=q[head].step+1;
            pre[tail]=head;//记录路径

            tail++;//入队
        }
       head++;//出队
    }
    return;
}
int main()
{
    for(int i=0;i<5;i++)
        for(int j=0;j<5;j++)
        scanf("%d",&mp[i][j]);
        ans=0;
        memset(vis,0,sizeof(vis));
    bfs();
    //printf("%d\n",ans);
    return 0;
}
```




