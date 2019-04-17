# Fence Repair（POJ-3253） - Alex_McAvoy的博客 - CSDN博客





2018年07月17日 15:24:53[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：62
个人分类：[POJ																[STL 的使用——容器与迭代器](https://blog.csdn.net/u011815404/article/category/8793969)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

Farmer John wants to repair a small length of the fence around the pasture. He measures the fence and finds that he needsN (1 ≤ N ≤ 20,000) planks of wood, each having some integer lengthLi (1 ≤ Li ≤ 50,000) units. He then purchases a single long board just long enough to saw into theN planks (i.e., whose length is the sum of the lengths Li). FJ is ignoring the "kerf", the extra length lost to sawdust when a sawcut is made; you should ignore it, too.

FJ sadly realizes that he doesn't own a saw with which to cut the wood, so he mosies over to Farmer Don's Farm with this long board and politely asks if he may borrow a saw.

Farmer Don, a closet capitalist, doesn't lend FJ a saw but instead offers to charge Farmer John for each of theN-1 cuts in the plank. The charge to cut a piece of wood is exactly equal to its length. Cutting a plank of length 21 costs 21 cents.

Farmer Don then lets Farmer John decide the order and locations to cut the plank. Help Farmer John determine the minimum amount of money he can spend to create theN planks. FJ knows that he can cut the board in various different orders which will result in different charges since the resulting intermediate planks are of different lengths.

# **Input**

Line 1: One integer N, the number of planks

Lines 2..N+1: Each line contains a single integer describing the length of a needed plank

# Output

Line 1: One integer: the minimum amount of money he must spend to makeN-1 cuts

# Sample Input

**3858**

# Sample Output

**34**


题意：要把一个木板截成给定长度的几块小木板，每一次费用是当前木板的长度，给出要截成的小木板的个数 n 与长度，求最小花费。

思路：根据贪心思想，要使费用最小，就要让每次锯成的两块木板的长度和最小。使用优先队列，每次选择最小的两块木头相加，相加后再次进入队列，再选择最小的两块木头相加，依次类推，直至队列为空取得最小值。

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
priority_queue < int,vector<int>,greater<int> > q;//最小值优先的优先队列
int main()
{
    int n;
    scanf("%d",&n);

    for(int i=0;i<n;i++)
    {
        int temp;
        scanf("%d",&temp);
        q.push(temp);
    }

    long long sum=0;
    while(n!=1)//共截取n-1次
    {
        int a,b;

        a=q.top();//取队首元素
        q.pop();//队首元素出队
        b=q.top();//取队首元素
        q.pop();//队首元素出队

        sum+=a+b;//记录花费
        q.push(a+b);//队列中最小值与次小值的和入队

        n--;
    }

    printf("%lld\n",sum);

    return 0;
}
```






