# Hie with the Pie（POJ-3311） - Alex_McAvoy的博客 - CSDN博客





2018年08月22日 10:34:23[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：58
个人分类：[POJ																[动态规划——状压 DP](https://blog.csdn.net/u011815404/article/category/7955244)](https://blog.csdn.net/u011815404/article/category/7504500)








# Problem Description

The Pizazz Pizzeria prides itself in delivering pizzas to its customers as fast as possible. Unfortunately, due to cutbacks, they can afford to hire only one driver to do the deliveries. He will wait for 1 or more (up to 10) orders to be processed before he starts any deliveries. Needless to say, he would like to take the shortest route in delivering these goodies and returning to the pizzeria, even if it means passing the same location(s) or the pizzeria more than once on the way. He has commissioned you to write a program to help him.

# **Input**

Input will consist of multiple test cases. The first line will contain a single integer n indicating the number of orders to deliver, where 1 ≤ n ≤ 10. After this will be n + 1 lines each containing n + 1 integers indicating the times to travel between the pizzeria (numbered 0) and the n locations (numbers 1 to n). The jth value on the ith line indicates the time to go directly from location i to location j without visiting any other locations along the way. Note that there may be quicker ways to go from i to j via other locations, due to different speed limits, traffic lights, etc. Also, the time values may not be symmetric, i.e., the time to go directly from location i to j may not be the same as the time to go directly from location j to i. An input value of n = 0 will terminate input.

# Output

For each test case, you should output a single number indicating the minimum time to deliver all of the pizzas and return to the pizzeria.

# Sample Input

**3 0 1 10 10 1 0 1 2 10 1 0 10 10 2 10 0 0 **

# Sample Output

**8**

————————————————————————————————————————————————————

题意：从起点 0 开始，去其他 n 个点，再回到起点 0，每个点可经过多次，求最少花费时间

思路：类似 TSP 问题，但每个点可以走多次，先用Floyd算法的预处理一下两点之间的距离，然后使用状压 DP 解即可

用一个二进制数表示点是否走过，用 dp[state][i] 表示到达 i 点状态为 state 的最短距离

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
#define N 1000001
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
int dis[12][12];
int dp[1<<11][12];
int main()
{
    int n;
    while(scanf("%d",&n)!=EOF&&n)
    {
        for(int i=0;i<=n;i++)
           for(int j=0;j<=n;j++)
                cin>>dis[i][j];

        /*Floyd算法预处理*/
        for(int k=0;k<=n;k++)
           for(int i=0;i<=n;i++)
                for(int j=0;j<=n;j++)
                    if(dis[i][k]+dis[k][j]<dis[i][j])
                        dis[i][j]=dis[i][k]+dis[k][j];


        for(int s=0;s<=(1<<n)-1;s++)//枚举所有状态
        {
            for(int i=1;i<=n;i++)//枚举所有城市
            {
                if(s&(1<<(i-1)))//已经过城市i
                {
                    if(s==(1<<(i-1)))//只经过城市i
                        dp[s][i]=dis[0][i];
                    else//经过多个城市
                    {
                        dp[s][i]=INF;
                        for(int j=1;j<=n;j++)
                        {
                            if(s&(1<<(j-1))&&j!=i)//不是城市i的其他城市
                                dp[s][i]=min(dp[s^(1<<(i-1))][j]+dis[j][i],dp[s][i]);
                        }
                    }
                }
            }
        }

        int minn=dp[(1<<n)-1][1]+dis[1][0];
        for(int i=2;i<=n;i++)
            if(dp[(1<<n)-1][i]+dis[i][0]<minn)
                minn=dp[(1<<n)-1][i]+dis[i][0];

        cout<<minn<<endl;
    }
    return 0;
}
```






