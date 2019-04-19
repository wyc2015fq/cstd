# POJ   1088  滑雪（简单DFS+dp） - HJ - CSDN博客
2016年08月11日 13:36:29[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：252
Description 
Michael喜欢滑雪百这并不奇怪， 因为滑雪的确很刺激。可是为了获得速度，滑的区域必须向下倾斜，而且当你滑到坡底，你不得不再次走上坡或者等待升降机来载你。Michael想知道载一个区域中最长底滑坡。区域由一个二维数组给出。数组的每个数字代表点的高度。下面是一个例子  
 1  2  3  4 5
16 17 18 19 6
15 24 25 20 7
14 23 22 21 8
13 12 11 10 9
一个人可以从某个点滑向上下左右相邻四个点之一，当且仅当高度减小。在上面的例子中，一条可滑行的滑坡为24-17-16-1。当然25-24-23-…-3-2-1更长。事实上，这是最长的一条。
Input 
输入的第一行表示区域的行数R和列数C(1 <= R,C <= 100)。下面是R行，每行有C个整数，代表高度h，0<=h<=10000。
Output 
输出最长区域的长度。
Sample Input 
5 5 
1 2 3 4 5 
16 17 18 19 6 
15 24 25 20 7 
14 23 22 21 8 
13 12 11 10 9
Sample Output 
25
解析：看到题目要求从一个方格的n*m个数里面寻找最长降序子列，每个数字有四个方向寻找，首先就应该用到DFS，找到当前点a[i][j]的最长降序子列，然后简单dp一下，求出n*m所有数的最长降序子列最长的那个子列。说起来可能比较拗口，请看下面代码：
```
#include<iostream>
using namespace std;
int s[105][105],ss[105][105],a[4]={0,-1,0,1},b[4]={-1,0,1,0},n,m;
int DFS(int x,int y)
{
    if(ss[x][y]!=0)
      return ss[x][y];
    int max=0,gg;
    for(int i=0;i<4;i++)
    {
        int xx=x+a[i];
        int yy=y+b[i];
        if(xx>=0&&xx<n&&yy>=0&&yy<m)     //四个方向搜索 
        {
            if(s[xx][yy]<s[x][y])
            {
                gg=DFS(xx,yy);    //递归DFS 
                if(max<gg)
                  max=gg;      //寻找最长子串 
            }
        }
    }
    ss[x][y]=max+1;   //注意长度值+1 
    return ss[x][y];    //返回处在当前位置最长子串的长度值 
}
int main()
{
    while(cin>>n>>m)
    {
        int max=0;
        for(int i=0;i<n;i++)
          for(int j=0;j<m;j++)
          {
              ss[i][j]=0;       //赋初值0
              cin>>s[i][j];
          }
        for(int i=0;i<n;i++)
          for(int j=0;j<m;j++)
          {
              ss[i][j]=DFS(i,j);        
              if(max<ss[i][j])      
                max=ss[i][j];    //寻找在所有位置里最长子串，并赋给其长度值 
          }
        cout<<max<<endl;
    }
    return 0;
}
```
