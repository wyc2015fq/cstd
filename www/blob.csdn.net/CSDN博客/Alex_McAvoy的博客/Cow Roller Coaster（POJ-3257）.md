# Cow Roller Coaster（POJ-3257） - Alex_McAvoy的博客 - CSDN博客





2018年07月24日 15:29:30[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：58
个人分类：[POJ																[动态规划——背包问题](https://blog.csdn.net/u011815404/article/category/7813163)](https://blog.csdn.net/u011815404/article/category/7504500)








# Problem Description

The cows are building a roller coaster! They want your help to design as fun a roller coaster as possible, while keeping to the budget. The roller coaster will be built on a long linear stretch of land of length L (1 <= L <= 1,000). The roller coaster comprises a collection of some of the N (1 <= N <= 10,000) different interchangable components. Each component i has a fixed length Wi (1 <= Wi <= L). Due to varying terrain, each component i can be only built starting at location Xi (0 <= Xi <= L-Wi). The cows want to string together various roller coaster components starting at 0 and ending at L so that the end of each component (except the last) is the start of the next component. Each component i has a "fun rating" Fi (1 <= Fi <= 1,000,000) and a cost Ci (1 <= Ci <= 1000). The total fun of the roller coster is the sum of the fun from each component used; the total cost is likewise the sum of the costs of each component used. The cows' total budget is B (1 <= B <= 1000). Help the cows determine the most fun roller coaster that they can build with their budget.

# **Input**

* Line 1: Three space-separated integers: L, N and B.

 * Lines 2..N+1: Line i+1 contains four space-separated integers, respectively: Xi, Wi, Fi, and Ci.

# Output

* Line 1: A single integer that is the maximum fun value that a roller-coaster can have while staying within the budget and meeting all the other constraints. If it is not possible to build a roller-coaster within budget, output -1.

# Sample Input

**5 6 100 2 20 62 3 5 60 1 2 11 1 1 31 2 5 43 2 10 2**

# Sample Output

**17**

————————————————————————————————————————————————————

题意：建造一个长度为 L 的铁路，有 N 条轨道，每根轨道的起点为 x ，长度为 w，有趣指数为 f，成本为 c，求选用轨道的有趣指数和最大，成本和不超过 b 

思路：

用 f[i][j] 来表示以 i 为起点，花费为 j 的最大有趣指数，以 x 为开始，以 x +w 为结束，可得转移方程 f[i+w][j+c] = max(f[i+w][j+c],dp[i][j]+f)

要注意的是，只有当起点被选中时才可更新终点，因此要先将 f[N][N] 初始化为 -1，作为标志。

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
#define N 1001
#define MOD 123
#define E 1e-6
using namespace std;

int l,n,b;
int f[N][N];
struct Node{
    int x;
    int w;
    int f;
    int c;
}cow[N*10];
int cmp(Node a,Node b)
{
    if (a.x==b.x)
        return a.w<b.w;
    return a.x<b.x;
}
int main()
{
    scanf("%d%d%d",&l,&n,&b);
    for(int i=0;i<n;i++)
        scanf("%d%d%d%d",&cow[i].x,&cow[i].w,&cow[i].f,&cow[i].c);

    sort(cow,cow+n,cmp);//根据轨道的起点进行排序

    memset(f,-1,sizeof(f));//需要判断状态是否能达到，进行标记
    f[0][0]=0;

    for(int i=0;i<n;i++)
        for(int j=cow[i].c;j<b;j++)
            if(f[cow[i].x][j-cow[i].c]!=(-1))//只有但起点被选中时才能更新终点
                f[cow[i].x+cow[i].w][j]=max(f[cow[i].x][j-cow[i].c]+cow[i].f,f[cow[i].x+cow[i].w][j]);

    int maxx=-1;
    for(int i=0;i<=b;i++)
        maxx=max(maxx,f[l][i]);

    printf("%d\n",maxx);

    return 0;
}
```






