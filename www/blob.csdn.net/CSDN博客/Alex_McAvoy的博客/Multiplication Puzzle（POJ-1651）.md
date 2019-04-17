# Multiplication Puzzle（POJ-1651） - Alex_McAvoy的博客 - CSDN博客





2018年08月17日 16:31:57[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：60








# Problem Description

The multiplication puzzle is played with a row of cards, each containing a single positive integer. During the move player takes one card out of the row and scores the number of points equal to the product of the number on the card taken and the numbers on the cards on the left and on the right of it. It is not allowed to take out the first and the last card in the row. After the final move, only two cards are left in the row. 

The goal is to take cards in such order as to minimize the total number of scored points. 

For example, if cards in the row contain numbers 10 1 50 20 5, player might take a card with 1, then 20 and 50, scoring 

10*1*50 + 50*20*5 + 10*50*5 = 500+5000+2500 = 8000

If he would take the cards in the opposite order, i.e. 50, then 20, then 1, the score would be 

1*50*20 + 1*20*5 + 10*1*5 = 1000+100+50 = 1150.

# **Input**

The first line of the input contains the number of cards N (3 <= N <= 100). The second line contains N integers in the range from 1 to 100, separated by spaces.

# Output

Output must contain a single integer - the minimal score.

# Sample Input

**610 1 50 50 20 5**

# Sample Output

**3650**

————————————————————————————————————————————————————

题意：给出 n 个数字，每次删除除第一个与最后一个数字外的一个数字，如要删除一个数字 ai，则花费为 ai*a(i-1)*a(i+1)，删除该数字以后剩下的数字又合并成一个新的数列，求最后删到只剩两个数字的最小花费

思路： 应该算是区间DP的模板题，枚举区间即可

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<set>
#include<map>
#include<stack>
#include<ctime>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 10001
#define MOD 10007
#define E 1e-6
#define LL long long
using namespace std;
int a[N];
int dp[N][N];
int main()
{
    int n;
    while(scanf("%d",&n)!=EOF&&n)
    {
        memset(dp,0,sizeof(dp));

        for(int i=1;i<=n;i++)
            scanf("%d",&a[i]);

        for(int len=2;len<n;len++)
        {
            for(int i=1;i+len<=n;i++)
            {
                int j=i+len;
                dp[i][j]=INF;
                for(int k=i+1;k<j;k++)
                    dp[i][j]=min(dp[i][j],dp[i][k]+dp[k][j]+a[i]*a[k]*a[j]);
            }
        }
        printf("%d\n",dp[1][n]);
    }

    return 0;
}
```





