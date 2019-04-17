# Robberies（HDU-2955） - Alex_McAvoy的博客 - CSDN博客





2018年03月31日 21:36:50[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：54
个人分类：[动态规划——背包问题																[HDU](https://blog.csdn.net/u011815404/article/category/7923257)](https://blog.csdn.net/u011815404/article/category/7813163)








# Problem Description

    The aspiring Roy the Robber has seen a lot of American movies, and knows that the bad guys usually gets caught in the end, often because they become too greedy. He has decided to work in the lucrative business of bank robbery only for a short while, before retiring to a comfortable job at a university.

    For a few months now, Roy has been assessing the security of various banks and the amount of cash they hold. He wants to make a calculated risk, and grab as much money as possible.

    His mother, Ola, has decided upon a tolerable probability of getting caught. She feels that he is safe enough if the banks he robs together give a probability less than this.

# Input

    The first line of input gives T, the number of cases. For each scenario, the first line of input gives a floating point number P, the probability Roy needs to be below, and an integer N, the number of banks he has plans for. 

    Then follow N lines, where line j gives an integer Mj and a floating point number Pj . 

    Bank j contains Mj millions, and the probability of getting caught from robbing it is Pj .

# Output

    For each test case, output a line with the maximum number of millions he can expect to get while the probability of getting caught is less than the limit set.

    Notes and Constraints：

        0 < T <= 100

        0.0 <= P <= 1.0

        0 < N <= 100

        0 < Mj <= 100

        0.0 <= Pj <= 1.0

    A bank goes bankrupt if it is robbed, and you may assume that all probabilities are independent as the police have very low funds.

# Sample Input

**30.04 31 0.022 0.033 0.050.06 32 0.032 0.033 0.050.10 31 0.032 0.023 0.05**

# Sample Output

**246**

————————————————————————————————————————————

思路：01背包变种
- 求的是：1-被抓概率=逃跑概率，限制条件和最大背包容量不能同为一属性。
- 概率为浮点数，精确度不高无法遍历。因此背包容量是钱数，因为能抢的银行有限，钱数也是有限的。
- 求最大逃跑概率，而题中每项给出的是被抓概率，所以要先用1减一下。
- 求得的逃跑概率随着抢银行的数量增加而减少，多抢一个银行，其钱数必将转化为概率的乘积，所以动态方程也要做出改变。
- 最后进行遍历，剩余的钱数越多，说明所抢的钱数越少，逃跑几率越大。所以从大到小遍历背包容量，一旦大于p，即为最大概率，跳出。

# Source Program

```cpp
#include<cstdio>
#include<iostream>
#include<cstring>
#define N 50001
using namespace std;
double c[N],dp[N];
int v[N];
int main()
{
    int t;
    double p,n;
    int sum;
    int i,j;

    cin>>t;
    while(t--)
    {
        cin>>p>>n;
        
        /*初始化*/
        sum=0;
        memset(dp,0,sizeof(dp));
        dp[0]=1;
        
        for(i=1;i<=n;i++)
        {
            cin>>v[i]>>c[i];
            sum+=v[i];//记录钱数
        }

        for(i=1;i<=n;i ++)
            for(j=sum;j>=v[i];j--)
                dp[j]=max(dp[j],dp[j-v[i]]*(1-c[i]));

        for(i=sum;i>=0;i--)
        {
            if(dp[i]>(1-p))//一旦大于p，即为最大概率，跳出
            {
                cout<<i<<endl;
                break;
            }
        }
    }
    return 0;
}
```




