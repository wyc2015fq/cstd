# Balloons（CF-998A） - Alex_McAvoy的博客 - CSDN博客





2018年08月26日 16:18:00[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：58
个人分类：[基础算法——贪心																[CodeForces](https://blog.csdn.net/u011815404/article/category/7923180)](https://blog.csdn.net/u011815404/article/category/7628972)








# Problem Description

There are quite a lot of ways to have fun with inflatable balloons. For example, you can fill them with water and see what happens.

Grigory and Andrew have the same opinion. So, once upon a time, they went to the shop and bought nn packets with inflatable balloons, where i-th of them has exactly aiai balloons inside.

They want to divide the balloons among themselves. In addition, there are several conditions to hold:

Do not rip the packets (both Grigory and Andrew should get unbroken packets);

Distribute all packets (every packet should be given to someone);

Give both Grigory and Andrew at least one packet;

To provide more fun, the total number of balloons in Grigory's packets should not be equal to the total number of balloons in Andrew's packets.

Help them to divide the balloons or determine that it's impossible under these conditions.

# Input

The first line of input contains a single integer n (1≤n≤10) — the number of packets with balloons.

The second line contains nn integers: a1, a2, ……, an (1≤ai≤1000) — the number of balloons inside the corresponding packet.

# Output

If it's impossible to divide the balloons satisfying the conditions above, print −1.

Otherwise, print an integer kk — the number of packets to give to Grigory followed by k distinct integers from 1 to n — the indices of those. The order of packets doesn't matter.

If there are multiple ways to divide balloons, output any of them.

# Examples

**Input**

3

1 2 1

**Output**

2

1 2

**Input**

2

5 5

**Output**

-1

**Input**

1

10

**Output**

-1

————————————————————————————————————————————

题意：给出 n 个数列，表示有 n 袋气球，分给两个人，每个人至少有一袋，且两人气球总数不能一样，输出其中任意一个人所分得的气球袋数，以及这袋气球的下标。

思路：如果只有一袋气球，或者有两袋气球，并且这两袋气球的量一样的话，那么不可能按要求分气球，输出-1，否则，给任意一个人最小的那袋气球，其他全部分给另外一个人即可。

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
#define N 101
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
int a[N];
int vis[N];
int main()
{

     int n;
     cin>>n;

     int p;
     int sum=0,minn=INF;
     for(int i=1;i<=n;i++)
     {
         scanf("%d",&a[i]);

         if(i!=n)
            sum+=a[i];
         if(minn>a[i])
         {
            minn=a[i];
            p=i;
         }
     }

     if(n==1)
        cout<<-1<<endl;
     else if(n==2&&sum==a[n])
        cout<<-1<<endl;
     else
     {
         cout<<1<<endl;
         cout<<p<<endl;
     }
    return 0;
}
```






