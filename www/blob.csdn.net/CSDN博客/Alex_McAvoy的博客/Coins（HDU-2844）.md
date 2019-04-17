# Coins（HDU-2844） - Alex_McAvoy的博客 - CSDN博客





2018年03月30日 23:19:00[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：79
个人分类：[动态规划——背包问题																[HDU](https://blog.csdn.net/u011815404/article/category/7923257)](https://blog.csdn.net/u011815404/article/category/7813163)








# Problem Description

    Whuacmers use coins.They have coins of value A1,A2,A3...An Silverland dollar. One day Hibix opened purse and found there were some coins. He decided to buy a very nice watch in a nearby shop. He wanted to pay the exact price(without change) and he known the price would not more than m.But he didn't know the exact price of the watch.

    You are to write a program which reads n,m,A1,A2,A3...An and C1,C2,C3...Cn corresponding to the number of Tony's coins of value A1,A2,A3...An then calculate how many prices(form 1 to m) Tony can pay use these coins. 

# Input

    The input contains several test cases. The first line of each test case contains two integers n(1 ≤ n ≤ 100),m(m ≤ 100000).The second line contains 2n integers, denoting A1,A2,A3...An,C1,C2,C3...Cn (1 ≤ Ai ≤ 100000,1 ≤ Ci ≤ 1000). The last test case is followed by two zeros.

# Output

    For each test case output the answer on a single line.

# Sample Input

**3 101 2 4 2 1 12 51 4 2 10 0**

# Sample Output

**84**

————————————————————————————————————————————

思路：多重背包问题，需进行二进制优化

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
int main()
{
    int n,m;
    int a[101],c[101];
    int dp[100001];
    int sum;
    int amount;
    int i,j,k;

    while(scanf("%d%d",&n,&m)!=EOF)
    {
        if(n==0&&m==0)
            break;
        for(i=1;i<=n;i++)
            scanf("%d",&a[i]);
        for(i=1;i<=n;i++)
            scanf("%d",&c[i]);

        sum=0;
        memset(dp,0,sizeof(dp));
        dp[0]=1;

        for(i=1;i<=n;i++)//按硬币种类枚举
        {
            /*二进制优化*/
            k=1;
            while(k<=c[i])
            {
                amount=k*a[i];//统计总价值
                for(j=m;j>=amount;j--)//完全背包
                    if(dp[j-amount]==1)
                        dp[j]=1;
                c[i]-=k;//记录当前硬币所使用的数量
                k*=2;//每次乘2
            }
            if(c[i])//如果当前硬币有剩余
            {
                amount=a[i]*c[i];//记录当前情况的硬币总价值
                for(j=m;j>=amount;j--)//完全背包
                    if(dp[j-amount]==1)
                        dp[j]=1;
            }
        }
        
        for(i=1;i<=m;i++)
            if(dp[i]==1)
                sum++;

        printf("%d\n",sum);

    }
    return 0;
}
```






