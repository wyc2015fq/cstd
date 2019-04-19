# POJ 1321  棋盘问题（dfs） - HJ - CSDN博客
2017年01月14日 20:39:11[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：239
Description 
在一个给定形状的棋盘（形状可能是不规则的）上面摆放棋子，棋子没有区别。要求摆放时任意的两个棋子不能放在棋盘中的同一行或者同一列，请编程求解对于给定形状和大小的棋盘，摆放k个棋子的所有可行的摆放方案C。
Input 
输入含有多组测试数据。 
每组数据的第一行是两个正整数，n k，用一个空格隔开，表示了将在一个n*n的矩阵内描述棋盘，以及摆放棋子的数目。 n <= 8 , k <= n 
当为-1 -1时表示输入结束。 
随后的n行描述了棋盘的形状：每行有n个字符，其中 # 表示棋盘区域， . 表示空白区域（数据保证不出现多余的空白行或者空白列）。
Output 
对于每一组数据，给出一行输出，输出摆放的方案数目C （数据保证C<2^31）。
Sample Input
2 1 
 #. 
.#
4 4 
…# 
..#. 
.#.. 
 #…
-1 -1
Sample Output
2 
1
解法： 
    基础DFS，用col[]记录每一列的访问状态
```cpp
#include<iostream>
using namespace std;
char s[8][8];
int col[8];
int sum,n,k;
void dfs(int begin,int num)
{
    for(int j=0;j<n;j++)
      if(s[begin][j]=='#'&&col[j]==0)
      {
          if(num==1)
            sum++;
          else
          {
              col[j]=1;
              for(int t=begin+1;t<n-num+2;t++)
                dfs(t,num-1);
              col[j]=0;
          }
      }
}
int main()
{
    while(cin>>n>>k)
    {
        if(n==-1&&k==-1)
          break;
        sum=0;
        for(int i=0;i<n;i++)
          for(int j=0;j<n;j++)
            cin>>s[i][j];
        for(int i=0;i<n;i++)
          col[i]=0;                               //用来保存每一列的访问状态 
        for(int i=0;i<n-k+1;i++)                    //一共是放k个棋子，每个棋子不同行，所以要用k行 
          dfs(i,k);                               //从第i行开始，放k个棋子，按照行递增的顺序访问，不会出现同行的情况 
        cout<<sum<<endl;
    }
    return 0;
}
```
