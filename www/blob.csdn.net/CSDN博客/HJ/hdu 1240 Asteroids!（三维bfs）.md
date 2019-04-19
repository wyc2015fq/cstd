# hdu 1240 Asteroids!（三维bfs） - HJ - CSDN博客
2017年07月02日 12:53:57[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：218
Problem Description 
You’re in space. 
You want to get home. 
There are asteroids. 
You don’t want to hit them.
Input 
Input to this problem will consist of a (non-empty) series of up to 100 data sets. Each data set will be formatted according to the following description, and there will be no blank lines separating data sets.
A single data set has 5 components:
Start line - A single line, “START N”, where 1 <= N <= 10.
Slice list - A series of N slices. Each slice is an N x N matrix representing a horizontal slice through the asteroid field. Each position in the matrix will be one of two values:
‘O’ - (the letter “oh”) Empty space
‘X’ - (upper-case) Asteroid present
Starting Position - A single line, “A B C”, denoting the < A,B,C > coordinates of your craft’s starting position. The coordinate values will be integers separated by individual spaces.
Target Position - A single line, “D E F”, denoting the < D,E,F > coordinates of your target’s position. The coordinate values will be integers separated by individual spaces.
End line - A single line, “END”
The origin of the coordinate system is <0,0,0>. Therefore, each component of each coordinate vector will be an integer between 0 and N-1, inclusive.
The first coordinate in a set indicates the column. Left column = 0.
The second coordinate in a set indicates the row. Top row = 0.
The third coordinate in a set indicates the slice. First slice = 0.
Both the Starting Position and the Target Position will be in empty space.
Output 
For each data set, there will be exactly one output set, and there will be no blank lines separating output sets.
A single output set consists of a single line. If a route exists, the line will be in the format “X Y”, where X is the same as N from the corresponding input data set and Y is the least number of moves necessary to get your ship from the starting position to the target position. If there is no route from the starting position to the target position, the line will be “NO ROUTE” instead.
A move can only be in one of the six basic directions: up, down, left, right, forward, back. Phrased more precisely, a move will either increment or decrement a single component of your current position vector by 1.
Sample Input
START 1 
O 
0 0 0 
0 0 0 
END 
START 3 
XXX 
XXX 
XXX 
OOO 
OOO 
OOO 
XXX 
XXX 
XXX 
0 0 1 
2 2 1 
END 
START 5 
OOOOO 
OOOOO 
OOOOO 
OOOOO 
OOOOO 
OOOOO 
OOOOO 
OOOOO 
OOOOO 
OOOOO 
XXXXX 
XXXXX 
XXXXX 
XXXXX 
XXXXX 
OOOOO 
OOOOO 
OOOOO 
OOOOO 
OOOOO 
OOOOO 
OOOOO 
OOOOO 
OOOOO 
OOOOO 
0 0 0 
4 4 4 
END
Sample Output
1 0 
3 4 
NO ROUTE
题目意思读了好久才懂！汗！其实就是给你一个三维空间图，要你求从起始地点到终点所需要的最少步数。
三维bfs搜索！
```cpp
#include<iostream>
#include<cstring>
#include<queue>
using namespace std;
#define maxn 25
char a[maxn][maxn][maxn];
int vis[maxn][maxn][maxn];
int dx[]={0,0,0,0,1,-1},dy[]={0,0,1,-1,0,0},dz[]={1,-1,0,0,0,0};    //三维遍历 
int n,sx,sy,sz,qx,qy,qz;       //(sx,sy,sz)为起点 (qx,qy,qz)为终点 
struct node{
    int x,y,z;             //队列当前点的三维空间位置 
    int step;         //从起点走到当前点所需要的最少步骤 
};
int check(int x,int y,int z)          //判断当前点是否可以访问 
{
    if(x<0||x>=n||y<0||y>=n||z<0||z>=n||vis[x][y][z])
      return 0;
    return 1;
}
int bfs(int x,int y,int z)
{
    queue<node> q;
    node a,next;
    a.x=x;a.y=y;a.z=z;
    a.step=0;
    vis[x][y][z]=1;
    q.push(a);
    while(!q.empty())
    {
        a=q.front();
        q.pop();
        if(a.x==qx&&a.y==qy&&a.z==qz)     //如果到达终点，返回步骤 
          return a.step;
        for(int i=0;i<6;i++)
        {
            next=a;
            next.x+=dx[i];next.y+=dy[i];next.z+=dz[i];
            if(check(next.x,next.y,next.z))
            {
                vis[next.x][next.y][next.z]=1;
                next.step++;
                q.push(next);
            }
        }
    }
    return -1;
}
int main()
{
    char s[10];
    while(cin>>s>>n)
    {
        for(int i=0;i<n;i++)
          for(int j=0;j<n;j++)
            cin>>a[i][j];
        cin>>sx>>sy>>sz>>qx>>qy>>qz;
        cin>>s;
        int ans=bfs(sx,sy,sz);
        if(ans>=0)
          cout<<n<<" "<<ans<<endl;
        else
          cout<<"NO ROUTE"<<endl;
    }
    return 0;
}
```
