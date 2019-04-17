# Ant Counting（POJ-3046） - Alex_McAvoy的博客 - CSDN博客





2018年07月26日 18:58:56[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：84








# Problem Description

The cows are going to space! They plan to achieve orbit by building a sort of space elevator: a giant tower of blocks. They have K (1 <= K <= 400) different types of blocks with which to build the tower. Each block of type i has height h_i (1 <= h_i <= 100) and is available in quantity c_i (1 <= c_i <= 10). Due to possible damage caused by cosmic rays, no part of a block of type i can exceed a maximum altitude a_i (1 <= a_i <= 40000). 

Help the cows build the tallest space elevator possible by stacking blocks on top of each other according to the rules.

# **Input**

* Line 1: A single integer, K 

* Lines 2..K+1: Each line contains three space-separated integers: h_i, a_i, and c_i. Line i+1 describes block type i.

# Output

* Line 1: A single integer H, the maximum height of a tower that can be built

# Sample Input

**37 40 35 23 82 52 6**

# Sample Output

**48**

————————————————————————————————————————————————————

题意：给出 k 块砖，以及每块砖的高度 h，数量 c 以及第 i 块砖不能超过高度 a 垒放，问最高可以垒多高。

思路：先将砖头按照不能超过的高度排序，随后套用多重背包模版即可。

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
int dp[N];
int num[N];
struct Node{
    int h;
    int c;
    int a;
}block[N];
int cmp(Node x,Node y)
{
    return x.a<y.a;
}
int main()
{
    int k;
    scanf("%d",&k);

    int height=0;
    for(int i=0;i<k;i++)
    {
        scanf("%d%d%d",&block[i].h,&block[i].a,&block[i].c);
        height=max(block[i].a,height);
    }
    sort(block,block+k,cmp);

    for(int i=0;i<N;i++)
        dp[i]=-INF;
    //memset(dp,-INF,sizeof(dp));
    dp[0]=0;

    for(int i=0;i<k;i++)
    {
        for(int j=0;j<=height;j++)
            num[j]=0;

        //memset(num,0,sizeof(num));
        for(int j=block[i].h;j<=block[i].a;j++)
        {
            if(dp[j]==-INF&&dp[j-block[i].h]>-INF)
            {
                if(num[j-block[i].h]<block[i].c)
                {
                    dp[j]=j;
                    num[j]=num[j-block[i].h]+1;
                }
            }
        }
    }

    int maxx=0;
    for(int i=height;i>=0;i--)
    {
        if(dp[i]>-INF)
        {
            maxx=i;
            break;
        }
    }

    printf("%d\n",maxx);

    return 0;
}
```





