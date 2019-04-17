# Cow Acrobats（POJ-3045） - Alex_McAvoy的博客 - CSDN博客





2018年07月26日 16:55:02[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：38
个人分类：[POJ																[基础算法——贪心](https://blog.csdn.net/u011815404/article/category/7628972)](https://blog.csdn.net/u011815404/article/category/7504500)








# Problem Description

Farmer John's N (1 <= N <= 50,000) cows (numbered 1..N) are planning to run away and join the circus. Their hoofed feet prevent them from tightrope walking and swinging from the trapeze (and their last attempt at firing a cow out of a cannon met with a dismal failure). Thus, they have decided to practice performing acrobatic stunts. 

The cows aren't terribly creative and have only come up with one acrobatic stunt: standing on top of each other to form a vertical stack of some height. The cows are trying to figure out the order in which they should arrange themselves ithin this stack. 

Each of the N cows has an associated weight (1 <= W_i <= 10,000) and strength (1 <= S_i <= 1,000,000,000). The risk of a cow collapsing is equal to the combined weight of all cows on top of her (not including her own weight, of course) minus her strength (so that a stronger cow has a lower risk). Your task is to determine an ordering of the cows that minimizes the greatest risk of collapse for any of the cows.

# **Input**

* Line 1: A single line with the integer N. 

* Lines 2..N+1: Line i+1 describes cow i with two space-separated integers, W_i and S_i. 

# Output

* Line 1: A single integer, giving the largest risk of all the cows in any optimal ordering that minimizes the risk.

# Sample Input

**310 32 53 3**

# Sample Output

**2**

————————————————————————————————————————————————————

题意：n 头牛叠罗汉，每头牛都有自己的重量 wi 和力量 si，他们各自的风险值是自己上面的牛总重量减去自己的力量，求叠起来的最小风险值

思路：对于每头牛而言，应该将他与他上面的所有牛看做一个整体，则这头牛的危险值为 所有牛的总重-这头牛的重量-这头牛的力量，要想危险值最小，牛的重量+力量 的值就应该最大。

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
struct Node{
    int w;
    int s;
}cow[N];
int cmp(Node a,Node b)
{
    return a.s+a.w>b.s+b.w;
}
int main()
{
    int n;
    scanf("%d",&n);

    int sum=0;
    for(int i=1;i<=n;i++)
    {
        scanf("%d%d",&cow[i].w,&cow[i].s);
        sum+=cow[i].w;
    }

    sort(cow+1,cow+1+n,cmp);

    int minn=-INF;
    for(int i=1;i<=n;i++)
    {
        sum-=cow[i].w;
        minn=max(minn,sum-cow[i].s);
    }

    printf("%d\n",minn);

    return 0;
}
```






