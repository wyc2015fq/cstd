# hdu  3853 LOOPS（概率dp） - HJ - CSDN博客
2017年03月16日 21:03:00[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：312
Problem Description 
Akemi Homura is a Mahou Shoujo (Puella Magi/Magical Girl).
Homura wants to help her friend Madoka save the world. But because of the plot of the Boss Incubator, she is trapped in a labyrinth called LOOPS.
The planform of the LOOPS is a rectangle of R*C grids. There is a portal in each grid except the exit grid. It costs Homura 2 magic power to use a portal once. The portal in a grid G(r, c) will send Homura to the grid below G (grid(r+1, c)), the grid on the right of G (grid(r, c+1)), or even G itself at respective probability (How evil the Boss Incubator is)! 
At the beginning Homura is in the top left corner of the LOOPS ((1, 1)), and the exit of the labyrinth is in the bottom right corner ((R, C)). Given the probability of transmissions of each portal, your task is help poor Homura calculate the EXPECT magic power she need to escape from the LOOPS.
![这里写图片描述](http://acm.hdu.edu.cn/data/images/3853-1.jpg)
Input 
The first line contains two integers R and C (2 <= R, C <= 1000).
The following R lines, each contains C*3 real numbers, at 2 decimal places. Every three numbers make a group. The first, second and third number of the cth group of line r represent the probability of transportation to grid (r, c), grid (r, c+1), grid (r+1, c) of the portal in grid (r, c) respectively. Two groups of numbers are separated by 4 spaces.
It is ensured that the sum of three numbers in each group is 1, and the second numbers of the rightmost groups are 0 (as there are no grids on the right of them) while the third numbers of the downmost groups are 0 (as there are no grids below them).
You may ignore the last three numbers of the input data. They are printed just for looking neat.
The answer is ensured no greater than 1000000.
Terminal at EOF
Output 
A real number at 3 decimal places (round to), representing the expect magic power Homura need to escape from the LOOPS.
Sample Input
2 2 
0.00 0.50 0.50    0.50 0.00 0.50 
0.50 0.50 0.00    1.00 0.00 0.00
Sample Output
6.000
题目大意：
从点（1，1）到（R，C），每次有三种选择，待在原地，往左走一步，往下走一步，现在分别给出每一个位置三种选择的概率，并且要求，每次移动都需要消耗2点能量，求最后到达终点的平均期望值。
解题思路：
设d[i][j]为从（i，j）到（R，C）所需要的平均期望（能量），则可以得到：
```
d[i][j] = d[i][j] * p[i][j][0] + d[i][j+1] * p[i][j][1] + d[i+1][j] * p[i][j][2] + 2
```
化简之后得到：
```
dp[i][j] = ( p[i][j][1] * dp[i][j+1] + p[i][j][2] * dp[i+1][j] + 2) / ( 1 - p[i][j][0] )
```
然后还需要注意两点：
**如果已经在目的地就不需要一定，平均期望值就为0**
**如果当前位置待在原地的概率为1，那么就走不到终点，平均期望也为0**
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
double dp[1005][1005];
double p[1005][1005][4]; 
int main()
{
    int n,m;
    while(~scanf("%d%d",&n,&m))
    {
        for(int i=1;i<=n;i++)
          for(int j=1;j<=m;j++)
            for(int k=0;k<3;k++)
              scanf("%lf",&p[i][j][k]);
        memset(dp,0,sizeof(dp));
        for(int i=n;i>0;i--)
          for(int j=m;j>0;j--)
          {
              if(i==n&&j==m)   //如果不需要移动，期望也为0 
                continue;
              if(p[i][j][0]==1.0)     //留在原地的概率为1，无路可走，因此期望为0 
                continue;
              // 根据d[i][j]=d[i][j]*p[i][j][0]+d[i][j+1]*p[i][j][1]+d[i+1][j]*p[i][j][2]+2化简而来 
              dp[i][j]+=(p[i][j][1]*dp[i][j+1]+p[i][j][2]*dp[i+1][j]+2)/(1-p[i][j][0]);
          }
        printf("%.3lf\n",dp[1][1]);
    }
    return 0;
}
```
