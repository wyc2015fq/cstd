# Most Powerful（ZOJ-3471） - Alex_McAvoy的博客 - CSDN博客





2018年08月22日 11:33:47[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：51
个人分类：[POJ																[动态规划——状压 DP](https://blog.csdn.net/u011815404/article/category/7955244)](https://blog.csdn.net/u011815404/article/category/7504500)








# Problem Description

Recently, researchers on Mars have discovered N powerful atoms. All of them are different. These atoms have some properties. When two of these atoms collide, one of them disappears and a lot of power is produced. Researchers know the way every two atoms perform when collided and the power every two atoms can produce.

You are to write a program to make it most powerful, which means that the sum of power produced during all the collides is maximal.

# **Input**

There are multiple cases. The first line of each case has an integer N (2 <= N <= 10), which means there are N atoms: A1, A2, ... , AN. Then N lines follow. There are N integers in each line. The j-th integer on the i-th line is the power produced when Ai and Aj collide with Aj gone. All integers are positive and not larger than 10000.

The last case is followed by a 0 in one line.

There will be no more than 500 cases including no more than 50 large cases that N is 10.

# Output

Output the maximal power these N atoms can produce in a line for each case.

# Sample Input

**20 41 030 20 112 0 11 10 00**

# Sample Output

**422**

————————————————————————————————————————————————————

题意：不超过10种气体，两两之间相互碰撞可以产生一定的能量，a 碰 b，b 会就消失，同一种类不能碰撞，求能得到的最大能量

思路：

用 10 位二进制表示气体是否存在，0 表示存在，1表示不存在，上一个状态中的两种气体碰撞并且有一种消失，可以得到新的状态，用 dp[state] 表示状态为 state 时的最大能量，则有转移方程：dp[state] = max(dp[state],dp[pre_state]+a[i][j])

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
#define N 1001
#define MOD 100000000
#define E 1e-6
#define LL long long
using namespace std;
int a[10][10];
int dp[1<<10];
int main()
{
    int n;
    while(scanf("%d",&n)!=EOF&&n)
    {
        memset(dp,0,sizeof(dp));
        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
                cin>>a[i][j];

        for(int s=0;s<(1<<n);s++)
        {
            for(int i=0;i<n;i++)
            {
                if(s&(1<<i))
                    continue;

                for(int j=0;j<n;j++)
                {
                    if(i==j)
                        continue;
                    if(s&(1<<j))
                        continue;

                    int new_state=s+(1<<j);//下一状态
                    dp[new_state]=max(dp[new_state],dp[s]+a[i][j]);
                }
            }
        }

        int maxx=-INF;
        for(int i=0;i<(1<<n);i++)
            maxx=max(maxx,dp[i]);
        printf("%d\n",maxx);
    }
    return 0;
}
```






