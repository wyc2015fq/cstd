# Tanya and Stairways（CF-1005A） - Alex_McAvoy的博客 - CSDN博客





2018年08月05日 20:17:11[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：50
个人分类：[基础算法——模拟																[CodeForces](https://blog.csdn.net/u011815404/article/category/7923180)](https://blog.csdn.net/u011815404/article/category/7812141)








# Problem Description

Little girl Tanya climbs the stairs inside a multi-storey building. Every time Tanya climbs a stairway, she starts counting steps from 1 to the number of steps in this stairway. She speaks every number aloud. For example, if she climbs two stairways, the first of which contains 3 steps, and the second contains 4 steps, she will pronounce the numbers 1,2,3,1,2,3,4.

You are given all the numbers pronounced by Tanya. How many stairways did she climb? Also, output the number of steps in each stairway.

The given sequence will be a valid sequence that Tanya could have pronounced when climbing one or more stairways.

# Input

The first line contains n (1≤n≤1000) — the total number of numbers pronounced by Tanya.

The second line contains integers a1,a2,…,an (1≤ai≤1000) — all the numbers Tanya pronounced while climbing the stairs, in order from the first to the last pronounced number. Passing a stairway with xx steps, she will pronounce the numbers 1,2,…,x1,2,…,x in that order.

The given sequence will be a valid sequence that Tanya could have pronounced when climbing one or more stairways.

# Output

In the first line, output tt — the number of stairways that Tanya climbed. In the second line, output tt numbers — the number of steps in each stairway she climbed. Write the numbers in the correct order of passage of the stairways.

# Examples

**Input**

7

1 2 3 1 2 3 4

**Output**

2

3 4 

**Input**

4

1 1 1 1

**Output**

4

1 1 1 1 

**Input**

5

1 2 3 4 5

**Output**

1

5 

**Input**

5

1 2 1 2 1

**Output**

3

2 2 1

————————————————————————————————————————————

题意：每走一个台阶就会报数，每走新的一层就再从1开始报，给出报的顺序，求一共走了几层，每层有多少个台阶

思路：每当遇到 1 就表示走到了新的一层，1 前边的一个数即为前一层的台阶数

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
#define N 10001
#define MOD 123
#define E 1e-6
using namespace std;
int a[N];
int main()
{
    int n;
    while(scanf("%d",&n)!=EOF)
    {
        int cnt=0;
        for(int i=1;i<=n;i++)
        {
            scanf("%d",&a[i]);
            if(a[i]==1)//有几个1就有几个楼梯
                cnt++;
        }

        printf("%d\n",cnt);
        
        for(int i=2;i<=n;i++)//如果一个台阶是1，则其前面的台阶号即为台阶数量
            if(a[i]==1)
                printf("%d ",a[i-1]);
        printf("%d\n",a[n]);

    }
    return 0;
}
```






