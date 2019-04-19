# hdu  1728 逃离迷宫（dfs） - HJ - CSDN博客
2017年06月30日 13:22:59[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：196
Problem Description 
　　给定一个m × n (m行, n列)的迷宫，迷宫中有两个位置，gloria想从迷宫的一个位置走到另外一个位置，当然迷宫中有些地方是空地，gloria可以穿越，有些地方是障碍，她必须绕行，从迷宫的一个位置，只能走到与它相邻的4个位置中,当然在行走过程中，gloria不能走到迷宫外面去。令人头痛的是，gloria是个没什么方向感的人，因此，她在行走过程中，不能转太多弯了，否则她会晕倒的。我们假定给定的两个位置都是空地，初始时，gloria所面向的方向未定，她可以选择4个方向的任何一个出发，而不算成一次转弯。gloria能从一个位置走到另外一个位置吗？
Input 
　　第1行为一个整数t (1 ≤ t ≤ 100),表示测试数据的个数，接下来为t组测试数据，每组测试数据中， 
　　第1行为两个整数m, n (1 ≤ m, n ≤ 100),分别表示迷宫的行数和列数，接下来m行，每行包括n个字符，其中字符’.’表示该位置为空地，字符’*’表示该位置为障碍，输入数据中只有这两种字符，每组测试数据的最后一行为5个整数k, x1, y1, x2, y2 (1 ≤ k ≤ 10, 1 ≤ x1, x2 ≤ n, 1 ≤ y1, y2 ≤ m),其中k表示gloria最多能转的弯数，(x1, y1), (x2, y2)表示两个位置，其中x1，x2对应列，y1, y2对应行。
Output 
　　每组测试数据对应为一行，若gloria能从一个位置走到另外一个位置，输出“yes”，否则输出“no”。
Sample Input
2 
5 5 
…** 
*.**. 
….. 
….. 
*…. 
1 1 1 1 3 
5 5 
…** 
*.**. 
….. 
….. 
*…. 
2 1 1 1 3
Sample Output
no 
yes
解题方法： 
实际上就是解决从起点到任意一点的最少转弯次数，用dfs解决的话，最重要的是需要提前判断好哪些情况需要提前退出，也就是剪枝！
细节请看代码！
```cpp
#include<iostream>
#include<cstring> 
#include<cstdio>
using namespace std;
#define INF 0x3f3f3f3f
#define maxn 1004 
char s[maxn][maxn];
int turn[maxn][maxn];            //turn[i][j]表示从起点(x1,y1)到点(i,j)所需要的最少转弯次数 
int dx[]={0,0,-1,1},dy[]={1,-1,0,0};
int n,m,blag,x1,y1,x2,y2,k;          //起点(x1,y1) 终点(x2,y2) 
void dfs(int x,int y,int direction)
{
    if(x==x2&&y==y2&&turn[x][y]<=k)    //到达终点并且转弯次数小于k 
    {
        blag=1;
        return ;
    }
    if(turn[x][y]>k)          //如果转弯次数大于k就不再继续 回溯到上一层 
      return ;
    if(x!=x2&&y!=y2&&turn[x][y]==k)         //如果从点(x,y)到点(x2,y2)至少还需要再转弯一次，而转弯次数等于k，就不必继续，直接返回 
      return ;
    for(int i=0;i<4;i++)
    {
        int xx=x+dx[i];
        int yy=y+dy[i];
        if(xx<1||xx>m||yy<1||yy>n||s[xx][yy]=='*')
          continue;
        if(turn[xx][yy]<turn[x][y])       //turn[xx][yy]表示点(xx,yy)已经由别的线路走过了，并记录了turn[xx][yy]   
          continue;                     //现在需要比较这次走到(xx,yy)和由别的路线走到(xx,yy)时，两个的turn值
        /* 注意turn[xx][yy]==turn[x][y]的情况不能剪掉 */ 
        if(direction!=-1&&i!=direction&&turn[xx][yy]<turn[x][y]+1)    //剪枝条件同上 
          continue;
        if(direction!=-1&&i!=direction)       //需要转弯 
          turn[xx][yy]=turn[x][y]+1;
        else                                   //不需要转弯 
          turn[xx][yy]=turn[x][y];
        s[xx][yy]='*';
        dfs(xx,yy,i);
        s[xx][yy]='.';
        if(blag)
          return ;
    }
}
int main()
{
    int t;
    cin>>t;
    while(t--)
    {
        cin>>m>>n;
        for(int i=1;i<=m;i++)
          for(int j=1;j<=n;j++)
             cin>>s[i][j];
        cin>>k>>y1>>x1>>y2>>x2;
        memset(turn,INF,sizeof(turn));        //初始化为INF  
        blag=0;
        turn[x1][y1]=0;
        dfs(x1,y1,-1);
        if(blag)
          cout<<"yes"<<endl;
        else
          cout<<"no"<<endl;
    }
    return 0;
}
```
