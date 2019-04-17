# Polycarp's Pockets（CF-1003A） - Alex_McAvoy的博客 - CSDN博客





2018年08月05日 19:46:46[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：63








> 
# Problem Description

Polycarp has nn coins, the value of the ii-th coin is aiai. Polycarp wants to distribute all the coins between his pockets, but he cannot put two coins with the same value into the same pocket.

For example, if Polycarp has got six coins represented as an array a=[1,2,4,3,3,2]a=[1,2,4,3,3,2], he can distribute the coins into two pockets as follows: [1,2,3],[2,3,4][1,2,3],[2,3,4].

Polycarp wants to distribute all the coins with the minimum number of used pockets. Help him to do that.

# Input

The first line of the input contains one integer nn (1≤n≤1001≤n≤100) — the number of coins.

The second line of the input contains nn integers a1,a2,…,ana1,a2,…,an (1≤ai≤1001≤ai≤100) — values of coins.

# Output

Print only one integer — the minimum number of pockets Polycarp needs to distribute all the coins so no two coins with the same value are put into the same pocket.

# Examples

**Input**

6

1 2 4 3 3 2

**Output**

2

**Input**

1

100

**Output**

1


题意：给出 n 个硬币及价值，要把放进一些口袋，要保证每个口袋的价值不同，输出所需的最少的口袋数

思路：抽屉原理，找出的金币最多重复个数即可。

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
int bucket[N];
int main()
{
    int n;
    while(scanf("%d",&n)!=EOF)
    {
        int maxx=-INF;
        memset(bucket,0,sizeof(bucket));
        for(int i=1;i<=n;i++)
        {
            int temp;
            scanf("%d",&temp);

            bucket[temp]++;//桶排思想存入硬币

            maxx=max(maxx,bucket[temp]);//寻找硬币最多的桶
        }

        printf("%d\n",maxx);
    }
	return 0;
}
```





