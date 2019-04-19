# POJ 3984  迷宫问题（BFS） - HJ - CSDN博客
2017年01月14日 20:34:53[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：245
Description 
定义一个二维数组：
int maze[5][5] = {
```
0, 1, 0, 0, 0,
0, 1, 0, 1, 0,
0, 0, 0, 0, 0,
0, 1, 1, 1, 0,
0, 0, 0, 1, 0,
```
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
Source
解法： 
    用BFS求最短路径，记录每一个状态之前的状态，输出路径即可
```
#include<iostream>
#include<cstring> 
using namespace std;
int vis[15][15];
int dx[4]={-1,0,1,0},dy[4]={0,-1,0,1};              //记录四个方向 
struct node{
    int x,y,pre;
}q[15];
void print(int i)
{
    if(q[i].pre!=-1)
    {
        print(q[i].pre);                               //递归调用，逆序打印路径 
        cout<<"("<<q[i].x<<", "<<q[i].y<<")"<<endl; 
    }
}
void bfs(int x1,int y1)
{
    int front=0,rear=1,m,n;
    q[front].x=x1;q[front].y=y1;q[front].pre=-1;
    while(front<rear)
    {
        for(int i=0;i<4;i++)
        {
            m=q[front].x+dx[i];
            n=q[front].y+dy[i];
            if(m<0||n<0||m>4||n>4||vis[m][n])
              continue;
            else
            {
                vis[m][n]=1;
                q[rear].x=m;
                q[rear].y=n;
                q[rear++].pre=front;               //每次记录前面一个可行节点的步数 
            }
            if(m==4&&n==4)                         //bfs搜索边界 
              print(front);
        }
        front++;                                   //层次数，即步数 
    }
}
int main()
{
    memset(vis,0,sizeof(vis));
    for(int i=0;i<5;i++)
      for(int j=0;j<5;j++)
        cin>>vis[i][j];
    cout<<"(0, 0)"<<endl;
    bfs(0,0);
    cout<<"(4, 4)"<<endl;
    return 0;
}
```
