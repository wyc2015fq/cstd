# Milking Time（POJ-3616） - Alex_McAvoy的博客 - CSDN博客





2018年06月06日 00:58:20[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：51








> 
# Problem Description

Bessie is such a hard-working cow. In fact, she is so focused on maximizing her productivity that she decides to schedule her next N (1 ≤ N ≤ 1,000,000) hours (conveniently labeled 0..N-1) so that she produces as much milk as possible.

Farmer John has a list of M (1 ≤ M ≤ 1,000) possibly overlapping intervals in which he is available for milking. Each interval i has a starting hour (0 ≤ starting_houri ≤ N), an ending hour (starting_houri < ending_houri ≤ N), and a corresponding efficiency (1 ≤ efficiencyi ≤ 1,000,000) which indicates how many gallons of milk that he can get out of Bessie in that interval. Farmer John starts and stops milking at the beginning of the starting hour and ending hour, respectively. When being milked, Bessie must be milked through an entire interval.

Even Bessie has her limitations, though. After being milked during any interval, she must rest R (1 ≤ R ≤ N) hours before she can start milking again. Given Farmer Johns list of intervals, determine the maximum amount of milk that Bessie can produce in the N hours.

# **Input**

* Line 1: Three space-separated integers: N, M, and R

* Lines 2..M+1: Line i+1 describes FJ's ith milking interval withthree space-separated integers: starting_houri , ending_houri , and efficiencyi

# Output

* Line 1: The maximum number of gallons of milk that Bessie can product in the N hours

# Sample Input

**12 4 2**

**1 2 810 12 193 6 247 10 31**

# Sample Output

**43**


题意：已知一时间段n，在这个时间段内有m段时间，以及工作后需休息r小时后才能再次工作，其后分别给出这m段时间内每段的开始时间、结束时间、工作量，求最大工作量。

思路：将m段时间段按照他们的结束时间升序排序，用f[i]表示取到第i段时间时的最大值，如果第i个时间段之前的一个时间段的结束时间加上r小于第i段的开始时间，更新最大值。

即：if(time[i].start>=time[j].end+r)

         f[i]=max(f[i],f[j]+time[i].value);

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
#define N 100001
#define MOD 1001
#define E 1e-12
using namespace std;
int f[N];
struct Node{
    int start;
    int endd;
    int value;
}time[N];
bool cmp(Node a,Node b)
{return a.endd<b.endd;}
int main()
{
    int n,m,r;
    scanf("%d%d%d",&n,&m,&r);

    for(int i=1;i<=m;i++)
        scanf("%d%d%d",&time[i].start,&time[i].endd,&time[i].value);

    sort(time+1,time+m+1,cmp);//按结束时间升序排序
    int maxx=-INF;
    time[0].endd=-r;
    
    for(int i=1;i<=m;i++)
    {
        for(int j=0;j<i;j++)
            if(time[i].start>=time[j].endd+r)//如果第i个时间段之前的一个时间段的结束时间加上r小于第i段的开始时间
                f[i]=max(f[i],f[j]+time[i].value);//更新最大值

        maxx=max(maxx,f[i]);
    }
    printf("%d\n",maxx);
}
```





