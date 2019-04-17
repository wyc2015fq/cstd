# Eating Together（POJ-3670） - Alex_McAvoy的博客 - CSDN博客





2018年06月07日 23:12:27[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：54
个人分类：[POJ																[动态规划——线性 DP](https://blog.csdn.net/u011815404/article/category/7813170)](https://blog.csdn.net/u011815404/article/category/7504500)








# Problem Description

The cows are so very silly about their dinner partners. They have organized themselves into three groups (conveniently numbered 1, 2, and 3) that insist upon dining together. The trouble starts when they line up at the barn to enter the feeding area.

Each cow i carries with her a small card upon which is engraved Di (1 ≤ Di ≤ 3) indicating her dining group membership. The entire set of N (1 ≤ N ≤ 30,000) cows has lined up for dinner but it's easy for anyone to see that they are not grouped by their dinner-partner cards.

FJ's job is not so difficult. He just walks down the line of cows changing their dinner partner assignment by marking out the old number and writing in a new one. By doing so, he creates groups of cows like 111222333 or 333222111 where the cows' dining groups are sorted in either ascending or descending order by their dinner cards.

FJ is just as lazy as the next fellow. He's curious: what is the absolute mminimum number of cards he must change to create a proper grouping of dining partners? He must only change card numbers and must not rearrange the cows standing in line.

# **Input**

Line 1: A single integer: N

Lines 2..N+1: Line i describes the i-th cow's current dining group with a single integer: Di

# Output

Line 1: A single integer representing the minimum number of changes that must be made so that the final sequence of cows is sorted in either ascending or descending order

# Sample Input

**513211**

# Sample Output

**1**

———————————————————————————————————————————————————————

题意：将n头牛分为3个用餐批次，要将已知序列改为不下降序列或不上升序列，求最少要改几次。

思路：对于已知序列，分别从两头寻找最大不上升序列与最大不下降序列，求两序列中元素最大值，表示序列不用更改的个数，最后用n减去不用更改的个数即为答案。

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
#define N 30001
#define MOD 50001
#define E 1e-12
using namespace std;
int a[N];
int q[N];
int main()
{
    int n;
    scanf("%d",&n);
    for(int i=1;i<=n;i++)
        scanf("%d",&a[i]);


    int len1=0,len2=0;
    int leftt,rightt,mid;
    for(int i=1;i<=n;i++)//求最大不上升序列
    {
        leftt=1;
        rightt=len1+1;
        while(leftt!=rightt)
        {
            mid=(leftt+rightt)/2;
            if(q[mid]<=a[i])
                leftt=mid+1;
            else
                rightt=mid;
        }
        len1=max(leftt,len1);//记录最大不上升序列的不需要更改的个数
        q[leftt]=a[i];
    }

    for(int i=n;i>=1;i--)//求最大不下降序列
    {
        leftt=1;
        rightt=len2+1;
        while(leftt!=rightt)
        {
            mid=(leftt+rightt)/2;
            if(q[mid]<=a[i])
                leftt=mid+1;
            else
                rightt=mid;
        }
        len2=max(leftt,len2);//记录最大不下降序列的不需要更改的个数
        q[leftt]=a[i];
    }

    int maxx=max(len1,len2);//求最大不上升序列与最大不下降序列中不用更改的最大值
    printf("%d",n-maxx);
    return 0;
}
```






