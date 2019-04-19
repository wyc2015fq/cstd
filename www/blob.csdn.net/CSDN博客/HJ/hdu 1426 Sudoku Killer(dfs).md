# hdu 1426 Sudoku Killer(dfs) - HJ - CSDN博客
2018年04月24日 21:15:43[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：145
[http://acm.hdu.edu.cn/showproblem.php?pid=1426](http://acm.hdu.edu.cn/showproblem.php?pid=1426)
首先要注意输入格式，利用scanf自动忽略换行。
解题思路：依次枚举每个‘？’,首先要用数组存起来，不然会超时。另外必须要用数组存，用vector会超时。
```cpp
#include<iostream>
#include<vector>
#include<cstdio>
using namespace std;
struct node{
    int x,y;
}p[105];
int cnt,blag,g[10][10];
bool Judge(int num,int k)
{
    for(int i=0;i<9;i++)
    {
        if(i!=p[num].y&&g[p[num].x][i]==k)
          return false;
        if(i!=p[num].x&&g[i][p[num].y]==k)
          return false;
    }
    int x=p[num].x/3*3;
    int y=p[num].y/3*3;
    for(int i=0;i<3;i++)
      for(int j=0;j<3;j++)
        if(g[x+i][y+j]==k&&(i!=p[num].x||j!=p[num].y))
          return false;
    return true;
}
void dfs(int num)
{
    if(num==cnt)
    {
        blag=1;
        return ;
    }
    for(int i=1;i<=9;i++)
    {
        if(Judge(num,i))
        {
            g[p[num].x][p[num].y]=i;
            dfs(num+1);
            if(blag)
              return ;
            g[p[num].x][p[num].y]=0;
        }
    }
}
int main()
{
    int t=0;
    char s[2];
    while(~scanf("%s",s))
    {
        cnt=blag=0;
        if(s[0]=='?')
          p[cnt].x=0,p[cnt++].y=0;
        g[0][0]=(s[0]=='?')?0:s[0]-'0';
        for(int i=0;i<9;i++)
          for(int j=0;j<9;j++)
            if(i||j)
            {
                scanf("%s",s);
                if(s[0]=='?')
                  p[cnt].x=i,p[cnt++].y=j;
                g[i][j]=(s[0]=='?')?0:s[0]-'0';
            }
        dfs(0);
        if(t++)
          printf("\n");
        for(int i=0;i<9;i++)
        {
            for(int j=0;j<8;j++)
              printf("%d ",g[i][j]);
            printf("%d\n",g[i][8]);
        }
    }
    return 0;
}
```
