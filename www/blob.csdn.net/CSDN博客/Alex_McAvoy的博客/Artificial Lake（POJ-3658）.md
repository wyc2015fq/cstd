# Artificial Lake（POJ-3658） - Alex_McAvoy的博客 - CSDN博客





2018年05月29日 20:32:07[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：73
个人分类：[POJ																[查找——二分查找](https://blog.csdn.net/u011815404/article/category/8116900)](https://blog.csdn.net/u011815404/article/category/7504500)








# Problem Description

The oppressively hot summer days have raised the cows' clamoring to its highest level. Farmer John has finally decided to build an artificial lake. For his engineering studies, he is modeling the lake as a two-dimensional landscape consisting of a contiguous sequence of N soon-to-be-submerged levels (1 ≤ N ≤ 100,000) conveniently numbered 1..N from left to right.

Each level i is described by two integers, its width Wi (1 ≤ Wi ≤ 1,000) and height (like a relative elevation) Hi (1 ≤ Hi ≤ 1,000,000). The heights of FJ's levels are unique. An infinitely tall barrier encloses the lake's model on the left and right. One example lake profile is shown below.


         *                    *  :

         *                    *  :

         *                    *  8

         *    ***           *  7

         *    ***           *  6

         *    ***           *  5

         *    **********    4 <- height

         *    **********    3

         ***************  2

         ***************  1

Level    |  1 |2|  3   |



In FJ's model, he starts filling his lake at sunrise by flowing water into the bottom of the lowest elevation at a rate of 1 square unit of water per minute. The water falls directly downward until it hits something, and then it flows and spreads as room-temperature water always does. As in all good models, assume that falling and flowing happen instantly. Determine the time at which each elevation's becomes submerged by a single unit of water.

```
WATER              WATER OVERFLOWS                     

       |                       |                           

     * |          *      *     |      *      *            *

     * V          *      *     V      *      *            *

     *            *      *    ....    *      *~~~~~~~~~~~~*

     *    **      *      *~~~~** :    *      *~~~~**~~~~~~*

     *    **      *      *~~~~** :    *      *~~~~**~~~~~~*

     *    **      *      *~~~~**~~~~~~*      *~~~~**~~~~~~*

     *    *********      *~~~~*********      *~~~~*********

     *~~~~*********      *~~~~*********      *~~~~*********

     **************      **************      **************

     **************      **************      **************
```

```
After 4 mins        After 26 mins       After 50 mins
     Lvl 1 submerged     Lvl 3 submerged     Lvl 2 submerged
```

Warning: The answer will not always fit in 32 bits.

# **Input**

Line 1: A single integer: N

Lines 2..N+1: Line i+1 describes level i with two space-separated integers: Wi and Hi

# Output

Lines 1..N: Line i contains a single integer that is the number of minutes that since sunrise when level #i is covered by water of height 1.

# Sample Input

**3**

**4 22 76 4**

# Sample Output

**45026**

———————————————————————————————————————————————————————

题意：给出N个连续的平台，他们各有宽度，且高度不同。先向高度最低的平台灌水，直到灌满溢出，流向其他的平台，直至所有平台都被覆盖。已知每分钟注入高为1宽为1的水，求每个平台恰好被高为1的水覆盖的时间。

思路：先记录每个平台的高度与宽度以及他们的左、右平台，找到最低的平台，开始灌水。灌满最低平台后，相当于该平台消失，寻找下一流向哪个平台，再更新左右平台，直到最高的平台被水覆盖。

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
#define N 100050
#define MOD 123
#define E 1e-12
using namespace std;
struct node
{
    long long wide;
    long long high;
    int left;
    int right;
}a[N];
int n;
long long ans[N];
int main()
{
    while(scanf("%d",&n)!=EOF)
    {
        for(int i=1;i<=n;i++)
            scanf("%lld%lld",&a[i].wide,&a[i].high);
        for(int i=0;i<=n+1;i++)//左右平台初始化
        {
            a[i].left=i-1;
            a[i].right=i+1;
        }
        a[0].wide=a[n+1].wide=1;//宽度设为无限高
        a[0].high=a[n+1].high=INF;//高度设为无限高

        int minn=INF;
        int platform;
        for(int i=1;i<=n;i++)//寻找高度最低的平台
            if(a[i].high<minn)
            {
                minn=a[i].high;
                platform=i;
            }

        long long num=1,sum=0;
        int left,right;
        while(num<=N)
        {
            num++;
            sum+=a[platform].wide;
            ans[platform]=sum;

            left=a[platform].left;
            right=a[platform].right;
            sum+=(min(a[left].high,a[right].high)-a[platform].high-1)*a[platform].wide;//-1是因为开始先加了一层

            a[left].right=right;//更新左平台
            a[right].left=left;//更新右平台
            if(a[left].high<a[right].high)//更新宽度
            {
                a[left].wide+=a[platform].wide;
                platform=left;
            }
            else
            {
                a[right].wide+=a[platform].wide;
                platform=right;
            }

            //更新下一流向平台标号
            while(1)
            {
                left=a[platform].left;
                right=a[platform].right;
                if(a[left].high<a[platform].high)
                    platform=left;
                else if(a[right].high<a[platform].high)
                    platform=right;
                else
                    break;
           }
        }

        for(int i=1;i<=n;i++)
            printf("%lld\n",ans[i]);
    }
    return 0;
}
```






