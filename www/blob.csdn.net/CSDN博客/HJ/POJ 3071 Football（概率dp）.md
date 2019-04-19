# POJ  3071 Football（概率dp） - HJ - CSDN博客
2017年03月16日 20:54:20[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：241
个人分类：[====ACM====																[---------OJ---------																[POJ																[--------动态规划---------																[概率DP](https://blog.csdn.net/feizaoSYUACM/article/category/7238030)](https://blog.csdn.net/feizaoSYUACM/article/category/7238093)](https://blog.csdn.net/feizaoSYUACM/article/category/6308043)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Description
Consider a single-elimination football tournament involving 2n teams, denoted 1, 2, …, 2n. In each round of the tournament, all teams still in the tournament are placed in a list in order of increasing index. Then, the first team in the list plays the second team, the third team plays the fourth team, etc. The winners of these matches advance to the next round, and the losers are eliminated. After n rounds, only one team remains undefeated; this team is declared the winner.
Given a matrix P = [pij] such that pij is the probability that team i will beat team j in a match determine which team is most likely to win the tournament.
Input
The input test file will contain multiple test cases. Each test case will begin with a single line containing n (1 ≤ n ≤ 7). The next 2n lines each contain 2n values; here, the jth value on the ith line represents pij. The matrix P will satisfy the constraints that pij = 1.0 − pji for all i ≠ j, and pii = 0.0 for all i. The end-of-file is denoted by a single line containing the number −1. Note that each of the matrix entries in this problem is given as a floating-point value. To avoid precision problems, make sure that you use either the double data type instead of float.
Output
The output file should contain a single line for each test case indicating the number of the team most likely to win. To prevent floating-point precision issues, it is guaranteed that the difference in win probability for the top two teams will be at least 0.01.
Sample Input
2 
0.0 0.1 0.2 0.3 
0.9 0.0 0.4 0.5 
0.8 0.6 0.0 0.6 
0.7 0.5 0.4 0.0 
-1
Sample Output
2
Hint
In the test case above, teams 1 and 2 and teams 3 and 4 play against each other in the first round; the winners of each match then play to determine the winner of the tournament. The probability that team 2 wins the tournament in this case is:
P(2 wins）
= P(2 beats 1)P(3 beats 4)P(2 beats 3) + P(2 beats 1)P(4 beats 3)P(2 beats 4)）
=p21p34p23 + p21p43p24
= 0.9 · 0.6 · 0.4 + 0.9 · 0.4 · 0.5 = 0.396.
The next most likely team to win is team 3, with a 0.372 probability of winning the tournament.
题目大意：
有2^n支球队，每两支队伍比一次，求最后胜出概率最大的球队。
解题思路：
设d[i][j]为第 i 场 第 j 支球队胜出，那么前提条件是，第 j 支球队和对手第 k 支球队都获胜了前 i-1 场并且在这一场 j 胜出了
```
d[i][j]+=d[i-1][j]*d[i-1][k]*p[j][k];
```
那么问题就在于，因为每次两支队伍是相邻的，如何找第 j 支球队的对手？
这里用到一个异或运算方法：
当j为奇数的时候，j=(2n+1)   那么j的对手必定是（2n） 即 (2n+1)^1==2n
同理当j为偶数的时候，j=2n 此时j的对手为(2n+1)  即(2n)^1=2n+1
```cpp
#include<iostream>
#include<cstring> 
#include<cstdio>
using namespace std;
#define M (1<<7)+10
double p[M][M],dp[M][M];
int bit(int x)                         //位运算求出2的x次方 
{ 
    return 1<<x;
}
int main()
{
    int n;
    while(scanf("%d",&n))
    {
        if(n==-1)
          break;
        int m=bit(n);
        for(int i=0;i<m;i++)
          for(int j=0;j<m;j++)
            scanf("%lf",&p[i][j]);
        memset(dp,0,sizeof(dp));                         //d[i][j]表示第i轮的时候，第j支队伍赢的概率 
        for(int i=0;i<m;i++)
          dp[0][i]=1; 
        for(int i=1;i<=n;i++)                           //2的n次方支队伍两两淘汰需要打n场 
          for(int j=0;j<m;j++)
            for(int k=0;k<m;k++)
              if(((j>>(i-1))^1)==(k>>(i-1)))                 //(2n)^1=2n+1   (2n+1)^1=2n 
                dp[i][j]+=dp[i-1][j]*dp[i-1][k]*p[j][k];
        int p=0;
        for(int i=0;i<m;i++)
          if(dp[n][i]>dp[n][p])
            p=i;
        printf("%d\n",p+1);
    }
    return 0;
}
```
