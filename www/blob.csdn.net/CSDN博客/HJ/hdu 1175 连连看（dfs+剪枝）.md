# hdu  1175  连连看（dfs+剪枝） - HJ - CSDN博客
2017年06月29日 18:23:11[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：235
Problem Description 
“连连看”相信很多人都玩过。没玩过也没关系，下面我给大家介绍一下游戏规则：在一个棋盘中，放了很多的棋子。如果某两个相同的棋子，可以通过一条线连起来（这条线不能经过其它棋子），而且线的转折次数不超过两次，那么这两个棋子就可以在棋盘上消去。不好意思，由于我以前没有玩过连连看，咨询了同学的意见，连线不能从外面绕过去的，但事实上这是错的。现在已经酿成大祸，就只能将错就错了，连线不能从外围绕过。 
玩家鼠标先后点击两块棋子，试图将他们消去，然后游戏的后台判断这两个方格能不能消去。现在你的任务就是写这个后台程序。
Input 
输入数据有多组。每组数据的第一行有两个正整数n,m(0< n<=1000,0< m< 1000)，分别表示棋盘的行数与列数。在接下来的n行中，每行有m个非负整数描述棋盘的方格分布。0表示这个位置没有棋子，正整数表示棋子的类型。接下来的一行是一个正整数q(0< q< 50)，表示下面有q次询问。在接下来的q行里，每行有四个正整数x1,y1,x2,y2,表示询问第x1行y1列的棋子与第x2行y2列的棋子能不能消去。n=0,m=0时，输入结束。 
注意：询问之间无先后关系，都是针对当前状态的！
Output 
每一组输入数据对应一行输出。如果能消去则输出”YES”,不能则输出”NO”。
Sample Input
3 4 
1 2 3 4 
0 0 0 0 
4 3 2 1 
4 
1 1 3 4 
1 1 2 4 
1 1 3 3 
2 1 2 4 
3 4 
0 1 4 3 
0 2 4 1 
0 0 0 0 
2 
1 1 2 4 
1 3 2 3 
0 0
Sample Output
YES 
NO 
NO 
NO 
NO 
YES
解题方法：很明显用深搜，但是需要注意的是有一些边界条件需要考虑，一些不必要的情况可以省略。否则， 很可能会超时。
```cpp
#include<iostream>
#include<cstring>
using namespace std;
int n,m,x1,y1,x2,y2;
int a[1005][1005],vis[1005][1005],blag;
int xx[]={0,0,1,-1},yy[]={1,-1,0,0};
void dfs(int x,int y,int k,int direction)
{
    if(blag)        //已经到达 
      return ;
    if(x<1||x>n||y<1||y>m)       //边界问题 
      return ;
    if(k==2&&x!=x2&&y!=y2)       //剪枝操作：当拐弯次数等于2并且当前点跟目的点不是在同一方向的时候，不必再搜，直接跳出 
      return ;
    if(k>2)          //拐弯次数大于2之后，已经没有必要继续搜下去了 
      return ;
    if(x==x2&&y==y2&&k<=2)
    {
        cout<<"YES"<<endl;
        blag=1;
        return ;
    }
    if(a[x][y])        //如果该点被访问，并且不是原点，那么说明不可能到达目的点，直接跳出 
    {
        if(x!=x1||y!=y1)
          return ;
    }
    vis[x][y]=1;
    for(int i=0;i<4;i++)
    {
        int dx=x+xx[i];
        int dy=y+yy[i];
        if(i==direction)         //不需要改变方向 
          dfs(dx,dy,k,direction);
        else                      //需要改变方向 
          dfs(dx,dy,k+1,i);
    }
    vis[x][y]=0;
}
int main()
{
    while(cin>>n>>m)
    {
        if(n==0&&m==0)
          break;
        for(int i=1;i<=n;i++)
          for(int j=1;j<=m;j++)
            cin>>a[i][j];
        int k;
        cin>>k;
        while(k--)
        {
            cin>>x1>>y1>>x2>>y2;
            if(x1==x2&&y1==y2)      //为同一个点的情况 
            {
                cout<<"NO"<<endl;
                continue;
            }
            if(x1<1||x2<1||x1>n||x2>n||y1<1||y2<1||y1>m||y2>m)    // 坐标越界的情况 
            {
                cout<<"NO"<<endl;
                continue;
            }
            memset(vis,0,sizeof(vis));
            if(a[x1][y1]==a[x2][y2]&&a[x1][y1])  //两个点位置上的数字相同且不为0
            {
                blag=0;
                for(int i=0;i<4;i++)
                  dfs(x1,y1,0,i);
                if(blag==0)
                  cout<<"NO"<<endl;
            } 
            else
              cout<<"NO"<<endl;
        }
    }
    return 0;
}
```
