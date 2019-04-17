# Dividing（HDU-1059） - Alex_McAvoy的博客 - CSDN博客





2018年03月30日 23:18:54[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：70
个人分类：[动态规划——背包问题																[HDU](https://blog.csdn.net/u011815404/article/category/7923257)](https://blog.csdn.net/u011815404/article/category/7813163)








# Problem Description

    Marsha and Bill own a collection of marbles. They want to split the collection among themselves so that both receive an equal share of the marbles. This would be easy if all the marbles had the same value, because then they could just split the collection in half. But unfortunately, some of the marbles are larger, or more beautiful than others. So, Marsha and Bill start by assigning a value, a natural number between one and six, to each marble. Now they want to divide the marbles so that each of them gets the same total value. 

    Unfortunately, they realize that it might be impossible to divide the marbles in this way (even if the total value of all marbles is even). For example, if there are one marble of value 1, one of value 3 and two of value 4, then they cannot be split into sets of equal value. So, they ask you to write a program that checks whether there is a fair partition of the marbles.

# Input

    Each line in the input describes one collection of marbles to be divided. The lines consist of six non-negative integers n1, n2, ..., n6, where ni is the number of marbles of value i. So, the example from above would be described by the input-line ``1 0 1 2 0 0''. The maximum total number of marbles will be 20000. 

    The last line of the input file will be ``0 0 0 0 0 0''; do not process this line.

# Output

    For each colletcion, output ``Collection #k:'', where k is the number of the test case, and then either ``Can be divided.'' or ``Can't be divided.''. 

    Output a blank line after each test case.

# Sample Input

**1 0 1 2 0 01 0 0 0 1 10 0 0 0 0 0**

# Sample Output

**Collection #1:Can't be divided.Collection #2:Can be divided.**

————————————————————————————————————————————

思路：经典多重背包问题，套用二进制优化模版即可

# Source Program

```cpp
#include<cstdio>
#include<cstring>
#include<iostream>
using namespace std;

int dp[200005];
int main()
{
    int a[7];
    int sum,ave;
    int i,j,k;
    int t=0;

    while(scanf("%d%d%d%d%d%d",&a[1],&a[2],&a[3],&a[4],&a[5],&a[6])!=EOF)
    {
        t++;//记录数据次数

        if(a[1]+a[2]+a[3]+a[4]+a[5]+a[6]==0)
            break;

        sum=0;
        memset(dp,0,sizeof(dp));

        for(i=1;i<=6;i++)//统计总价值
            sum+=i*a[i];

        printf("Collection #%d:\n",t);
        if(sum%2)//总价值无法均分，跳出
        {
            printf("Can't be divided.\n\n");
            continue;
        }

        ave=sum/2;//均值
        for(i=1;i<=6;i++)
        {
            if(i*a[i]>=ave)
            {
                for(j=i;j<=ave;j++)
                    dp[j]=max(dp[j],dp[j-i]+i);
                continue;
            }

            k=1;
            while(k<a[i])
            {
                for(j=ave;j>=k*i;j--)
                    dp[j]=max(dp[j],dp[j-k*i]+k*i);
                a[i]-=k;
                k*=2;
            }
            for(j=ave;j>=a[i]*i;j--)
                dp[j]=max(dp[j],dp[j-a[i]*i]+a[i]*i);
        }

        if(dp[ave]==ave)
            printf("Can be divided.\n");
        else
            printf("Can't be divided.\n");
        printf("\n");
    }
    return 0;
}
```






