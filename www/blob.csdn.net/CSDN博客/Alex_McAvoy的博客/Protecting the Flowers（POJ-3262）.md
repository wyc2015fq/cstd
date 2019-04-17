# Protecting the Flowers（POJ-3262） - Alex_McAvoy的博客 - CSDN博客





2018年06月17日 03:02:20[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：57
个人分类：[POJ																[基础算法——贪心](https://blog.csdn.net/u011815404/article/category/7628972)](https://blog.csdn.net/u011815404/article/category/7504500)








# Problem Description

Farmer John went to cut some wood and left N (2 ≤ N ≤ 100,000) cows eating the grass, as usual. When he returned, he found to his horror that the cluster of cows was in his garden eating his beautiful flowers. Wanting to minimize the subsequent damage, FJ decided to take immediate action and transport each cow back to its own barn.

Each cow i is at a location that is Ti minutes (1 ≤ Ti ≤ 2,000,000) away from its own barn. Furthermore, while waiting for transport, she destroys Di (1 ≤ Di ≤ 100) flowers per minute. No matter how hard he tries, FJ can only transport one cow at a time back to her barn. Moving cow i to its barn requires 2 × Ti minutes (Ti to get there and Ti to return). FJ starts at the flower patch, transports the cow to its barn, and then walks back to the flowers, taking no extra time to get to the next cow that needs transport.

Write a program to determine the order in which FJ should pick up the cows so that the total number of flowers destroyed is minimized.



# **Input**

Line 1: A single integer N 

Lines 2..N+1: Each line contains two space-separated integers, Ti and Di, that describe a single cow's characteristics

# Output

Line 1: A single integer that is the minimum number of destroyed flowers


# Sample Input

**6**

**3 12 52 33 24 11 6**

# Sample Output

**86**

———————————————————————————————————————————————————————

题意：将n头牛赶回牛圈，每头牛在被赶之前每秒破坏Di朵花，赶牛回去要花Ti×2的时间，在赶牛的过程中，牛不能破坏花，求赶完所有牛后，被破坏花的最小值。



思路：

典型贪心算法，先对牛的破坏度进行排序，然后顺序赶牛计算破坏度即可。

设两头牛 A、B，要先赶走破坏大的，留下破坏小的牛。若先赶走 A，则 B 造成 2×TA×DB 的损失；若先赶走B，则 A 造成 2×TA×DB 的损失，因此判断 TA×DB 与 TA×DB 即可。

推广到 n 头牛，则排序标准为：Ti×Dj > Tj×Di

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
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 100100
#define MOD 123
#define E 1e-6
using namespace std;
struct Node
{
   long long x;
   long long y;
}a[N];

int cmp(Node a,Node b)
{
   return b.x*a.y>a.x*b.y;
}

int main()
{
    int n;
    while(scanf("%d",&n)!=EOF)
    {
        memset(a,0,sizeof(a));

        long long sum=0;
        for(int i=0;i<n;i++)
        {
            scanf("%lld%lld",&a[i].x,&a[i].y);
            sum+=a[i].y;//计算所有牛在花园中每秒的破坏度
        }

        sort(a,a+n,cmp);//对牛按破坏度排序

        long long num=0;
        for(int i=0;i<n-1;i++)
        {
           sum-=a[i].y;//赶走一头牛后，减去该牛在花园中每秒的破坏度
           num+=2*sum*(a[i].x);//计算当前总破坏度
        }

        printf("%lld\n",num);
    }

    return 0;
}
```






