# POJ  1651 Multiplication Puzzle（区间dp） - HJ - CSDN博客
2017年03月14日 23:13:34[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：221
Description 
The multiplication puzzle is played with a row of cards, each containing a single positive integer. During the move player takes one card out of the row and scores the number of points equal to the product of the number on the card taken and the numbers on the cards on the left and on the right of it. It is not allowed to take out the first and the last card in the row. After the final move, only two cards are left in the row.
The goal is to take cards in such order as to minimize the total number of scored points.
For example, if cards in the row contain numbers 10 1 50 20 5, player might take a card with 1, then 20 and 50, scoring 
10*1*50 + 50*20*5 + 10*50*5 = 500+5000+2500 = 8000
If he would take the cards in the opposite order, i.e. 50, then 20, then 1, the score would be 
1*50*20 + 1*20*5 + 10*1*5 = 1000+100+50 = 1150.
Input 
The first line of the input contains the number of cards N (3 <= N <= 100). The second line contains N integers in the range from 1 to 100, separated by spaces.
Output 
Output must contain a single integer - the minimal score.
Sample Input
6 
10 1 50 50 20 5
Sample Output
3650
Source
题目大意：
给出n个数，除了第一个和第n个不能取出之外，你可以自由选择先后全部取出中间的n-1个数，但是每去一个数，需要a[i-1]*a[i]*a[i+1]的代价，如果旁边的数字已经被先取出，则再往旁边挪一位，求最少的代价。
解题思路：
设d[i][j]表示取出[i,j]之间的数需要的最少代价，则有
```
d[i][j]=min(d[i][k],d[k+1][j]+a[i]*a[k]*a[j] (k为最后一次取出的数）
```
```cpp
#include<iostream>
#include<cstring> 
using namespace std;
#define INF 100000005
int d[105][105],a[105];
int main()
{
    int n;
    while(cin>>n)
    {
        for(int i=1;i<=n;i++)
          cin>>a[i];
        memset(d,0,sizeof(d));
        for(int len=2;len<n;len++)      //区间（i,j）之间的长度 
          for(int i=2;i+len-1<=n;i++)            
          {
              int j=i+len-1;
              d[i][j]=INF;         //赋初值要足够大 
              for(int k=i;k<j;k++)              //类似于矩阵链乘 k表示从(i,j)中最后抽取k 
                d[i][j]=min(d[i][j],d[i][k]+d[k+1][j]+a[i-1]*a[k]*a[j]);
          }
        cout<<d[2][n]<<endl;
    }
    return 0;
}
```
