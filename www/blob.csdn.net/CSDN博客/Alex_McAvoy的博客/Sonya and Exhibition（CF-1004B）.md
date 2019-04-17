# Sonya and Exhibition（CF-1004B） - Alex_McAvoy的博客 - CSDN博客





2018年08月26日 16:12:45[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：75
个人分类：[CodeForces																[数学——其他](https://blog.csdn.net/u011815404/article/category/8279050)](https://blog.csdn.net/u011815404/article/category/7923180)








> 
# Problem Description

Sonya decided to organize an exhibition of flowers. Since the girl likes only roses and lilies, she decided that only these two kinds of flowers should be in this exhibition.

There are nn flowers in a row in the exhibition. Sonya can put either a rose or a lily in the ii-th position. Thus each of nn positions should contain exactly one flower: a rose or a lily.

She knows that exactly mm people will visit this exhibition. The i-th visitor will visit all flowers from lili to riri inclusive. The girl knows that each segment has its own beauty that is equal to the product of the number of roses and the number of lilies.

Sonya wants her exhibition to be liked by a lot of people. That is why she wants to put the flowers in such way that the sum of beauties of all segments would be maximum possible.

# Input

The first line contains two integers n and m (1≤n,m≤103) — the number of flowers and visitors respectively.

Each of the next m lines contains two integers li and ri (1≤li≤ri≤n), meaning that i-th visitor will visit all flowers from li to ri inclusive.

# Output

Print the string of nn characters. The i-th symbol should be «0» if you want to put a rose in the ii-th position, otherwise «1» if you want to put a lily.

If there are multiple answers, print any.

# Examples

**Input**

5 3

1 3

2 4

2 5

**Output**

01100

**Input**

6 3

5 6

1 4

4 6

**Output**

110010


题意：有 n 个位置，m 个顾客，要往 n 个位置中依次放两种以 1、0 代替的花，给出 m 个人观赏花的区间，其满意度为这个区间内两种花个数的乘积，先求给出一种花的排放顺序，使得所有人的满意度的和最大

思路：实质就是让 0、1 的个数在各个区间内都是接近的，因为和相等时，两数值越接近，积越大，即 0、1 分布均匀，因此直接0、1 交替输出即可

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
#define N 100001
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
int main()
{
    int n,m;
    cin>>n>>m;

    int l,r;
    while(m--)
        cin>>l>>r;

    for(int i=0;i<n;i++)
    {
        if(i%2)
            cout<<1;
        else
            cout<<0;
    }
    cout<<endl;

    return 0;
}
```






