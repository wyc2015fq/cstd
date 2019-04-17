# Yogurt factory（POJ-2393） - Alex_McAvoy的博客 - CSDN博客





2018年07月26日 19:33:45[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：65
个人分类：[POJ																[基础算法——贪心](https://blog.csdn.net/u011815404/article/category/7628972)](https://blog.csdn.net/u011815404/article/category/7504500)








# Problem Description

The cows have purchased a yogurt factory that makes world-famous Yucky Yogurt. Over the next N (1 <= N <= 10,000) weeks, the price of milk and labor will fluctuate weekly such that it will cost the company C_i (1 <= C_i <= 5,000) cents to produce one unit of yogurt in week i. Yucky's factory, being well-designed, can produce arbitrarily many units of yogurt each week. 

Yucky Yogurt owns a warehouse that can store unused yogurt at a constant fee of S (1 <= S <= 100) cents per unit of yogurt per week. Fortuitously, yogurt does not spoil. Yucky Yogurt's warehouse is enormous, so it can hold arbitrarily many units of yogurt. 

Yucky wants to find a way to make weekly deliveries of Y_i (0 <= Y_i <= 10,000) units of yogurt to its clientele (Y_i is the delivery quantity in week i). Help Yucky minimize its costs over the entire N-week period. Yogurt produced in week i, as well as any yogurt already in storage, can be used to meet Yucky's demand for that week.

# **Input**

* Line 1: Two space-separated integers, N and S. 

* Lines 2..N+1: Line i+1 contains two space-separated integers: C_i and Y_i.

# Output

* Line 1: Line 1 contains a single integer: the minimum total cost to satisfy the yogurt schedule. Note that the total might be too large for a 32-bit integer.

# Sample Input

**4 588 20089 40097 30091 500 **

# Sample Output

**126900**

————————————————————————————————————————————————————

题意：在 n 周内，每周花费 ci 元生产一单位的酸奶，每单位酸奶在仓库存储每周花费 s 元，现要求每周交付 y 单位的酸奶，仓库无限大，且酸奶不会变质，求上交的最小花费。

思路：贪心题，先比较当前周的与上周的总花费大小，如果当前周花费少低，则选择当前周的单价c[i]*y[i]，如果上周花费低，则选择 (c[i-1]+s)*y[i]，要注意的是，第一周时无法与上一周比较，只能选择交付本周生产的酸奶。注意数据范围。

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
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 100001
#define MOD 123
#define E 1e-6
using namespace std;
int c[N],y[N];
int main()
{
    int n,s;
    scanf("%d%d",&n,&s);

    for(int i=1;i<=n;i++)
        scanf("%d%d",&c[i],&y[i]);

    long long sum=0;
    sum+=c[1]*y[1];//第一周
    for(int i=2;i<=n;i++)
    {
        if(c[i-1]+s>c[i])
            sum+=c[i]*y[i];
        else
            sum+=(c[i-1]+s)*y[i];
    }

    printf("%lld\n",sum);

    return 0;
}
```






