# Balanced Numbers（SPOJ-BALNUM） - Alex_McAvoy的博客 - CSDN博客





2018年09月12日 20:31:47[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：48








# Problem Description

Balanced numbers have been used by mathematicians for centuries. A positive integer is considered a balanced number if:

1)      Every even digit appears an odd number of times in its decimal representation

2)      Every odd digit appears an even number of times in its decimal representation

For example, 77, 211, 6222 and 112334445555677 are balanced numbers while 351, 21, and 662 are not.

Given an interval [A, B], your task is to find the amount of balanced numbers in [A, B] where both A and B are included.

# **Input**

The first line contains an integer T representing the number of test cases.

A test case consists of two numbers A and B separated by a single space representing the interval. You may assume that 1 <= A <= B <= 1019 

# Output

For each test case, you need to write a number in a single line: the amount of balanced numbers in the corresponding interval

# Sample Input

**21 10001 9**

# Sample Output

**1474**

————————————————————————————————————————————————————

题意：给出区间 [a,b]，求在区间中，有多少个数在 数位 0-9 中，每一奇数数位有偶数个，偶数数位有奇数个

思路：数位dp，用 dp[i][j] 记录第 i 个数的状态，用一三进制数 0、1、2 分别表示未出现过、奇数次、偶数次

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
#define N 31
#define MOD 10007
#define E 1e-6
typedef long long LL;
using namespace std;
int bit[N];
LL dp[N][60000];
bool judge(int x)
{
    int num[N];
    for(int i=0;i<=9;i++)
    {
        num[i]=x%3;
        x/=3;
    }
    for(int i=0;i<=9;i++)
    {
        if(num[i]!=0)
        {
            if(i%2==0&&num[i]==2)
                return false;
            if(i%2==1&&num[i]==1)
                return false;
        }
    }
    return true;
}
int get(int x,int s)
{
    int num[10];
    for(int i=0;i<=9;i++)
    {
        num[i]=s%3;
        s/=3;
    }

    if(num[x]==0)
        num[x]=1;
    else
        num[x]=3-num[x];

    int temp=0;
    for(int i=9;i>=0;i--)
    {
        temp*=3;
        temp+=num[i];
    }
    return temp;
}
LL dfs(int pos,int sta,bool zero,bool limit)
{
    if(pos==0){
        if(judge(sta))
            return 1;
        return 0;
    }

    if(!limit&&dp[pos][sta]!=-1)
        return dp[pos][sta];

    int res=0;
    int up=limit?bit[pos]:9;
    for(int i=0;i<=up;i++)
    {
        int next=(zero&&i==0)?0:get(i,sta);
        res+=dfs(pos-1,next,i==0&&zero,limit&&i==up);
    }

    return limit?res:dp[pos][sta]=res;
}
LL solve(LL x)
{
    int pos=0;
    while(x)
    {
        bit[++pos]=x%10;
        x/=10;
    }

    return dfs(pos,0,true,true);
}
int main()
{
    int t;
    scanf("%d",&t);
    memset(dp,-1,sizeof(dp));
    while(t--)
    {
        LL a,b;
        scanf("%lld%lld",&a,&b);
        printf("%lld\n",solve(b)-solve(a-1));
    }
    return 0;
}
```





