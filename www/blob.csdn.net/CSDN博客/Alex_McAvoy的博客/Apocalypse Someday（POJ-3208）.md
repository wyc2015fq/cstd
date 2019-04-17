# Apocalypse Someday（POJ-3208） - Alex_McAvoy的博客 - CSDN博客





2018年09月08日 22:31:19[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：35
个人分类：[POJ																[动态规划——数位 DP](https://blog.csdn.net/u011815404/article/category/8008418)](https://blog.csdn.net/u011815404/article/category/7504500)








# Problem Description

The number 666 is considered to be the occult “number of the beast” and is a well used number in all major apocalypse themed blockbuster movies. However the number 666 can’t always be used in the script so numbers such as 1666 are used instead. Let us call the numbers containing at least three contiguous sixes beastly numbers. The first few beastly numbers are 666, 1666, 2666, 3666, 4666, 5666…

Given a 1-based index n, your program should return the nth beastly number.

# **Input**

The first line contains the number of test cases T (T ≤ 1,000).

Each of the following T lines contains an integer n (1 ≤ n ≤ 50,000,000) as a test case.

# Output

For each test case, your program should output the nth beastly number.

# Sample Input

**323187**

# Sample Output

**1666266666666**

————————————————————————————————————————————————————

题意：t 组询问，每组输出第 n 个包含连续 的三个 6 的数

思路：数位dp，用 dp[i][0]、dp[i][1]、dp[i][2]、dp[i][3] 分别表示 i 位数中首位不含 666、首位 1个 6 且不含 666、首位连续 2 个6 且不含 666、含有 666 的数的数量，用二分不断划分区间找第 n 个满足条件的数即可，由于数据范围，二分的右边界需要设一极大值，一般的 0x3f3f3f3f 仍达不到上界，需要换成 0x3f3f3f3f3f3f3f3f

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
#define INF 0x3f3f3f3f3f3f3f3fll
#define PI acos(-1.0)
#define N 31
#define MOD 10007
#define E 1e-6
typedef long long LL;
using namespace std;
int bit[N];
LL dp[N][4];
int judge(int sta,int x)
{
    if(sta==3)
        return 3;
    if(x!=6)
        return 0;
    return sta+1;
}
LL dfs(int pos,int sta,bool limit)
{
    if(pos<0)
        return sta==3;

    if(!limit && dp[pos][sta]!=-1)
        return dp[pos][sta];

    LL temp=0;
    int up=limit?bit[pos]:9;
    for(int i=0;i<=up;i++)
        temp+=dfs(pos-1,judge(sta,i),limit&&i==up);

    if(!limit)
        dp[pos][sta]=temp;
    return temp;
}
LL solve(LL x)
{
    int pos=0;
    while(x)
    {
        bit[pos++]=x%10;
        x/=10;
    }
    return dfs(pos-1,0,true);
}
LL Find(int x)
{
    LL left=0,right=INF;
    LL res=-1;
    while(left<=right)
    {
        LL mid=(left+right)/2;
        if(solve(mid)<x)
            left=mid+1;
        else
        {
            res=mid;
            right=mid-1;
        }
    }
    return res;
}
int main()
{
    int t;
    scanf("%d",&t);

    memset(dp,-1,sizeof(dp));
    while(t--)
    {
        int n;
        scanf("%d",&n);
        printf("%lld\n",Find(n));
    }
    return 0;
}
```






