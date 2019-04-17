# Muddy roads（POJ-2437） - Alex_McAvoy的博客 - CSDN博客





2018年07月25日 17:41:07[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：67








# Problem Description

Farmer John has a problem: the dirt road from his farm to town has suffered in the recent rainstorms and now contains (1 <= N <= 10,000) mud pools. 

Farmer John has a collection of wooden planks of length L that he can use to bridge these mud pools. He can overlap planks and the ends do not need to be anchored on the ground. However, he must cover each pool completely. 

Given the mud pools, help FJ figure out the minimum number of planks he needs in order to completely cover all the mud pools.

# **Input**

* Line 1: Two space-separated integers: N and L 

* Lines 2..N+1: Line i+1 contains two space-separated integers: s_i and e_i (0 <= s_i < e_i <= 1,000,000,000) that specify the start and end points of a mud pool along the road. The mud pools will not overlap. These numbers specify points, so a mud pool from 35 to 39 can be covered by a single board of length 4. Mud pools at (3,6) and (6,9) are not considered to overlap. 

# Output

* Line 1: The miminum number of planks FJ needs to use.

# Sample Input

**3 31 613 178 12**

# Sample Output

**5**

————————————————————————————————————————————————————

题意：给出每一块木板长度 L 与水坑个数 N，以及每个水坑的区间，求完全铺满水坑所需的最小木板数。

思路：先将水坑区间进行排序，再从最小的端点开始铺，每铺完一个区间，去判断木板右端点是否超过下一个区间的左端点，如果是，下一块木板就从这个点开始铺，如果不是，下一块木板从新的水坑起点开始铺。

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
int a[N];
int main()
{
    int n,l;
    scanf("%d%d",&n,&l);

    n*=2;//转换为水坑起点终点总数
    for(int i=1;i<=n;i+=2)
        scanf("%d%d",&a[i],&a[i+1]);

    sort(a+1,a+n+1);

    int k=0;
    int cnt=0;
    for(int i=1;i<=n;i+=2)
    {
        if(k>=a[i+1])
            continue;

        if(k>a[i])
            a[i]=k;

        int num=(a[i+1]-a[i])/l;//记录需要木板个数
        if( (a[i+1]-a[i])%l )//如果水坑有余
            num++;

        cnt+=num;
        k=num*l+a[i];
    }

    printf("%d\n",cnt);

    return 0;
}
```





