# Amount of Degrees（Ural-1057） - Alex_McAvoy的博客 - CSDN博客





2018年09月05日 19:57:02[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：67








# Problem Description

Create a code to determine the amount of integers, lying in the set [ X; Y] and being a sum of exactly K different integer degrees of B.

Example. Let X=15, Y=20, K=2, B=2. By this example 3 numbers are the sum of exactly two integer degrees of number 2:

17 = 2^4+2^0, 

18 = 2^4+2^1, 

20 = 2^4+2^2.

# **Input**

he first line of input contains integers X and Y, separated with a space (1 ≤  X ≤  Y ≤ 2 31−1). The next two lines contain integers K and B (1 ≤  K ≤ 20; 2 ≤  B ≤ 10).

# Output

Output should contain a single integer — the amount of integers, lying between X and Y, being a sum of exactly K different integer degrees of B.

# Sample Input

**15 2022**

# Sample Output

**3**

————————————————————————————————————————————————————

题意：求一个区间内转化为 c 进制 1 的个数为 k 的数的出现的次数

思路：数位DP入门题，套模板即可出

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
#define N 1001
#define MOD 10007
#define E 1e-6
typedef long long LL;
using namespace std;
int bit[N];
int dp[N][N];
int x,y,k,b;
int f(int x)
{
    if(x==0)
        return 0;
    int sum=f(x/10);
    return sum*2+(x%10);
}
int dfs(int pos,int sta,bool limit)
{
    if(pos<=0)
        return sta==0?1:0;
    if(sta<0)
        return 0;

    if(!limit&&dp[pos][sta]!=-1)
        return dp[pos][sta];

    int res=0;
    if(bit[pos]||!limit)
        res+=dfs(pos-1,sta-1,limit);
    res+=dfs(pos-1,sta,limit&&!bit[pos]);

    if(!limit)
        dp[pos][sta]=res;

    return res;
}
int solve(int x)
{
    int len=0;
    while(x)
    {
        bit[++len]=x%b;
        x/=b;
    }

    int i;
    for(i=len;i>=1;i--)
        if(bit[i]>1)
            break;

    while(i>=1)
    {
        bit[i]=1;
        i--;
    }

    return dfs(len,k,1);
}
int main()
{
    memset(dp,-1,sizeof(dp));

    int Case=1;
    while(scanf("%d%d%d%d",&x,&y,&k,&b)!=EOF)
        printf("%d\n",solve(y)-solve(x-1));

    return 0;
}
```





