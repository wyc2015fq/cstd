# POJ 3279   Fliptile（bfs） - HJ - CSDN博客
2017年01月14日 20:46:02[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：248
Description
Farmer John knows that an intellectually satisfied cow is a happy cow who will give more milk. He has arranged a brainy activity for cows in which they manipulate an M × N grid (1 ≤ M ≤ 15; 1 ≤ N ≤ 15) of square tiles, 
each of which is colored black on one side and white on the other side.
As one would guess, when a single white tile is flipped, it changes to black; when a single black tile is flipped, it changes to white. The cows are rewarded when they flip the tiles so that each tile has the white side face up. However, the cows have rather 
large hooves and when they try to flip a certain tile, they also flip all the adjacent tiles (tiles that share a full edge with the flipped tile). Since the flips are tiring, the cows want to minimize the number of flips they have to make.
Help the cows determine the minimum number of flips required, and the locations to flip to achieve that minimum. If there are multiple ways to achieve the task with the minimum amount of flips, return the one with the least lexicographical ordering in the 
output when considered as a string. If the task is impossible, print one line with the word “IMPOSSIBLE”.
Input 
Line 1: Two space-separated integers: M and N  
Lines 2..M+1: Line i+1 describes the colors (left to right) of row i of the grid with N space-separated integers which are 1 for black and 0 for white
Output 
Lines 1..M: Each line contains N space-separated integers, each specifying how many times to flip that particular location.
Sample Input
4 4 
1 0 0 1 
0 1 1 0 
0 1 1 0 
1 0 0 1
Sample Output
0 0 0 0 
1 0 0 1 
1 0 0 1 
0 0 0 0
Source 
USACO 2007 Open Silver
题目大意： 
    有一个M*N的棋盘，每一个格子只有两种状态0或1，每次可以选择一个格子执行翻转操作，并且与该格子相邻的4个格子都会被翻转，求将所有格子都翻转成0所需要的最小操作数，若有多种方案，输出字典序最小的方案数。
解法： 
    枚举第一行的所有翻转状态，在此基础上bfs剩余行的翻转状态。
```cpp
#include<iostream>
#include<cstring>
using namespace std;
#define INF 500
int n,m,map[18][18],vis[18][18],ans[18][18];
int dx[5]={-1,0,0,0,1},dy[5]={0,1,0,-1,0};
int get(int x,int y)                         //(x,y)的颜色 
{
    int res=map[x][y];
    for(int i=0;i<5;i++)
    {
        int a=dx[i]+x,b=dy[i]+y;
        if(a>=0&&a<m&&b>=0&&b<n)
          res+=vis[a][b];
    }
    return res&1;
}
int calc()                                  //求出第一行确定的情况下最小操作次数 
{
    for(int i=1;i<m;i++)
      for(int j=0;j<n;j++)
        if(get(i-1,j))                     //如果（i-1,j)是黑色，就要翻转 
          vis[i][j]=1;
    for(int i=0;i<n;i++)                   //无解 
      if(get(m-1,i))
        return INF;
    int res=0;                            //res用来记录翻转次数 
    for(int i=0;i<m;i++)
      for(int j=0;j<n;j++)
        res+=vis[i][j];
    return res;
}
int main()
{
    while(cin>>m>>n)
    {
        for(int i=0;i<m;i++)
          for(int j=0;j<n;j++)
            cin>>map[i][j];
        int res=INF;
        memset(ans,0,sizeof(ans));
        for(int i=0;i< 1<<n;i++)                //枚举第一行的所有翻转情况 
        {
            memset(vis,0,sizeof(vis));
            for(int j=0;j<n;j++)
              vis[0][j]= i>>j&1;                //在i的状态下，第一行的j是否翻转 
            int temp=calc();
            if(temp<res) 
            {
                res=temp;
                for(int i=0;i<m;i++)
                  for(int j=0;j<n;j++)
                    ans[i][j]=vis[i][j];
            } 
        }
        if(res==INF)
        {
            cout<<"IMPOSSIBLE"<<endl;
            continue;
        }
        for(int i=0;i<m;i++)
          for(int j=0;j<n;j++)  
          {  
              cout<<ans[i][j];  
              if(j!=n-1)
                cout<<" ";
              else
                cout<<endl;  
          }  
    }
    return 0;
}
```
