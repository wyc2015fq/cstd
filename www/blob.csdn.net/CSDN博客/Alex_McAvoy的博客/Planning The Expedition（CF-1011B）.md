# Planning The Expedition（CF-1011B） - Alex_McAvoy的博客 - CSDN博客





2018年08月15日 15:21:41[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：35
个人分类：[CodeForces																[基础算法——排序](https://blog.csdn.net/u011815404/article/category/7439201)](https://blog.csdn.net/u011815404/article/category/7923180)








# Problem Description

Natasha is planning an expedition to Mars for n people. One of the important tasks is to provide food for each participant.

The warehouse has mm daily food packages. Each package has some food type aiai.

Each participant must eat exactly one food package each day. Due to extreme loads, each participant must eat the same food type throughout the expedition. Different participants may eat different (or the same) types of food.

Formally, for each participant j Natasha should select his food type bjbj and each day j-th participant will eat one food package of type bj. The values bjbj for different participants may be different.

What is the maximum possible number of days the expedition can last, following the requirements above?

# Input

The first line contains two integers n and m (1≤n≤100, 1≤m≤100) — the number of the expedition participants and the number of the daily food packages available.

The second line contains sequence of integers a1,a2,…,am (1≤ai≤100), where aiai is the type of i-th food package.

# Output

Print the single integer — the number of days the expedition can last. If it is not possible to plan the expedition for even one day, print 0.

# Examples

**Input**

4 10

1 5 2 1 1 1 2 5 7 2

**Output**

2

**Input**

100 1

1

**Output**

0

**Input**

2 5

5 4 3 2 1

**Output**

1

**Input**

3 9

42 42 42 42 42 42 42 42 42

**Output**

3

————————————————————————————————————————————

题意：总共有 n 个人以及 m 个食物，且每个食物都有一个序号，每个人只能选择吃一种序号的食物，如果有一个人没有食物吃就算饿死，求所有人都没有饿死的情况下能存活几天

思路：先按天数从1枚举到100，依次判断每一天的食物是否足够所有人存活，若够就更新最大存活天数，否则输出之前的天数

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
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
int a[N];
int bucket[N];
int main()
{
    int n,m;
    cin>>n>>m;
    for(int i=1;i<=m;i++)
    {
        cin>>a[i];
        bucket[a[i]]++;
    }
    
    if(n>m)
        cout<<0<<endl;
    else
    {
        int day=0;
        for(int i=1;i<=100;i++)
        {
            int cnt=0;
            for(int j=1;j<=100;j++)
                cnt+=(bucket[j]/i);
                
            if(cnt>=n)
                day=i;
            else
                break;
        }
        cout<<day<<endl;
    }
    return 0;
}
```






