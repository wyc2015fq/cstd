# POJ 2251  Dungeon Master（BFS三维解） - HJ - CSDN博客
2017年01月14日 20:43:44[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：309
Description 
You are trapped in a 3D dungeon and need to find the quickest way out! The dungeon is composed of unit cubes which may or may not be filled with rock. It takes one minute to move one unit north, south, east, west, up or down. You cannot move diagonally and the maze is surrounded by solid rock on all sides.
Is an escape possible? If yes, how long will it take?
Input 
The input consists of a number of dungeons. Each dungeon description starts with a line containing three integers L, R and C (all limited to 30 in size). 
L is the number of levels making up the dungeon. 
R and C are the number of rows and columns making up the plan of each level. 
Then there will follow L blocks of R lines each containing C characters. Each character describes one cell of the dungeon. A cell full of rock is indicated by a ‘#’ and empty cells are represented by a ‘.’. Your starting position is indicated by ‘S’ and the exit by the letter ‘E’. There’s a single blank line after each level. Input is terminated by three zeroes for L, R and C.
Output 
Each maze generates one line of output. If it is possible to reach the exit, print a line of the form
Escaped in x minute(s). 
where x is replaced by the shortest time it takes to escape. 
If it is not possible to escape, print the line
Trapped! 
Sample Input
3 4 5 
S…. 
.###. 
.##.. 
 ###.#
##### 
 ##### 
 ##.## 
 ##…
##### 
 ##### 
 #.### 
 ####E
1 3 3 
 S## 
 #E# 
 ###
0 0 0
Sample Output
Escaped in 11 minute(s). 
Trapped!
题目大意： 
    一个立体空间， 输入三个数，L，R，C，代表有L个平面，R行，C列，.代表可以走，#代表不能走，S代表开始点，E代表结束点，问从S开始走，对每个位置，有六个走法，即空间的六个方向的走法（上下东南西北)，一分钟可以走一个点，问从S走到E点，最少可以经过多少分钟，若不能到达，则输出Trapped!
解题思路： 
    用三维数组模拟空间立体，用作于BFS来搜索
所以最后只需要用BFS来做即可，四个方向变成六个方向！
```
#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
char s[35][35][35];
int l,r,c,sx,sy,sz;
int len[30000],vis[35][35][35];
int xx[]={1,-1,0,0,0,0},yy[]={0,0,1,-1,0,0},zz[]={0,0,0,0,1,-1};     //三维空间总共六个方向 
struct node{
    int x,y,z;
}q[30000];
int bfs()
{
    int rear,front,dx,dy,dz;
    memset(vis,0,sizeof(vis));            //访问标志变量 
    memset(len,0,sizeof(len));            //统计走的步数 
    q[0].x=sx;q[0].y=sy;q[0].z=sz;
    front=rear=0;
    while(front<=rear)
    {
        for(int i=0;i<6;i++)
        {
            dx=q[front].x+xx[i];
            dy=q[front].y+yy[i];
            dz=q[front].z+zz[i];
            if(!vis[dx][dy][dz]&&s[dx][dy][dz]!='#'&&dx>=0&&dx<l&&dy>=0&&dy<r&&dz>=0&&dz<c)
            {
                vis[dx][dy][dz]=1;
                rear++;
                q[rear].x=dx;
                q[rear].y=dy;
                q[rear].z=dz;
                len[rear]=len[front]+1;
                if(s[dx][dy][dz]=='E')         //终止条件 
                  return len[rear];
            }
        }
        front++;                      //bfs层次遍历 
    }
    return 0;                         //如果不符合条件即步数返回为零 
} 
int main()
{
    while(cin>>l>>r>>c)
    {
        if(l==0&&r==0&&c==0)
          break;
        for(int i=0;i<l;i++)
          for(int j=0;j<r;j++)
            cin>>s[i][j];
        for(int i=0;i<l;i++)
          for(int j=0;j<r;j++)
            for(int k=0;k<c;k++)
              if(s[i][j][k]=='S')
              {
                  sx=i;sy=j;sz=k;              //寻找开始的搜索节点 进行bfs搜索 
              }
        if(bfs())
          cout<<"Escaped in "<<bfs()<<" minute(s)."<<endl;
        else
          cout<<"Trapped!"<<endl;
    }
    return 0;
}
```
