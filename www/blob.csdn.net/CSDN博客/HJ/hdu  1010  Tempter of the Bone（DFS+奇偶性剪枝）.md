# hdu   1010   Tempter of the Bone（DFS+奇偶性剪枝） - HJ - CSDN博客
2016年08月08日 22:18:58[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：253
个人分类：[====ACM====																[-----------搜索-----------																[---------OJ---------																[HDU																[剪枝](https://blog.csdn.net/feizaoSYUACM/article/category/7238040)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6307975)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Tempter of the Bone
Problem Description 
The doggie found a bone in an ancient maze, which fascinated him a lot. However, when he picked it up, the maze began to shake, and the doggie could feel the ground sinking. He realized that the bone was a trap, and he tried desperately to get out of this maze.
The maze was a rectangle with sizes N by M. There was a door in the maze. At the beginning, the door was closed and it would open at the T-th second for a short period of time (less than 1 second). Therefore the doggie had to arrive at the door on exactly the T-th second. In every second, he could move one block to one of the upper, lower, left and right neighboring blocks. Once he entered a block, the ground of this block would start to sink and disappear in the next second. He could not stay at one block for more than one second, nor could he move into a visited block. Can the poor doggie survive? Please help him.
Input 
The input consists of multiple test cases. The first line of each test case contains three integers N, M, and T (1 < N, M < 7; 0 < T < 50), which denote the sizes of the maze and the time at which the door will open, respectively. The next N lines give the maze layout, with each line containing M characters. A character is one of the following:
‘X’: a block of wall, which the doggie cannot enter;  
‘S’: the start point of the doggie;  
‘D’: the Door; or 
‘.’: an empty block.
The input is terminated with three 0’s. This test case is not to be processed.
Output 
For each test case, print in one line “YES” if the doggie can survive, or “NO” otherwise.
Sample Input 
4 4 5 
S.X. 
..X. 
..XD 
….
3 4 5 
S.X. 
..X. 
…D 
0 0 0
Sample Output 
NO 
YES
解析：很经典的DFS题，需要剪枝技巧。
首先，需要题目要求刚好在T时间点到达目的点，那么根据这一点可以进行第一步剪枝，只要超出时间就跳出，重新开始另一重搜索。另外，根据奇偶性来剪枝，是这一题的亮点所在。容易观察，设起点为偶数0，那么下一节点的值就可以设为奇数1，而再下一层节点就回到偶数0，这样的话就可以不经过搜索首先判断，如果时间给的是偶数，而起点跟终点的横纵坐标差的和为奇数，就直接输出NO，反之，如果时间给的是奇数，而起点跟终点的横纵坐标差的和为偶数，也直接输出NO。
具体请看如下代码：
```cpp
#include<iostream>
#include<cstdio>
#include<cmath>
using namespace std;
int a[4]={0,-1,0,1},b[4]={-1,0,1,0},n,m,cnt,t,blag,rr,ll;
char s[8][8];
void dfs(int x,int y,int cnt)
{
    if(x==rr&&y==ll&&cnt==t)     
    { 
        blag=1;
        return;
    }   
    if(cnt>=t)          //剪枝，如果当前时间已经超过最大值，则提前跳出循环 
      return;
    for(int i=0;i<4;i++)
    {
        int p=x+a[i];
        int q=y+b[i];
        if(p>=0&&p<n&&q>=0&&q<m&&s[p][q]!='X')
        {
            s[p][q]='X';          //标记 
            dfs(p,q,cnt+1);     //递归搜索 
            s[p][q]='.';     //回溯 
            if(blag)         //如果满足条件，提前跳出循环 
              return;
        }
    }
}
int main()
{
    while(cin>>n>>m>>t)
    {
        int r,l;
        if(n==0&&m==0&&t==0)
           break;
        for(int i=0;i<n;i++)
          for(int j=0;j<m;j++)
          {
              cin>>s[i][j];
              if(s[i][j]=='S')     //记录起点坐标 
              {
                  r=i;
                  l=j;
              } 
              else if(s[i][j]=='D')    //记录终点坐标 
              {
                  rr=i;
                  ll=j;
              }
          }
        if(abs(r-rr)+abs(l-ll)>t||(r+rr+l+ll+t)%2==1)   //剪枝，如果最少步数都超过时间或者奇偶性相反则跳过搜索直接判断NO 
        {
            cout<<"NO"<<endl;
            continue;
        }
        blag=0;
        s[r][l]='X';
        dfs(r,l,0);     //递归搜索 
        if(blag)
          cout<<"YES"<<endl;
        else
          cout<<"NO"<<endl;
    } 
    return 0;
}
```
