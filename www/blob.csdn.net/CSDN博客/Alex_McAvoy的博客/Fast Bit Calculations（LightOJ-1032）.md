# Fast Bit Calculations（LightOJ-1032） - Alex_McAvoy的博客 - CSDN博客





2019年01月22日 17:24:05[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：41
个人分类：[LightOJ																[动态规划——数位 DP](https://blog.csdn.net/u011815404/article/category/8008418)](https://blog.csdn.net/u011815404/article/category/8037630)








> 
# Problem Description

A bit is a binary digit, taking a logical value of either 1 or 0 (also referred to as "true" or "false" respectively). And every decimal number has a binary representation which is actually a series of bits. If a bit of a number is 1 and its next bit is also 1 then we can say that the number has a 1 adjacent bit. And you have to find out how many times this scenario occurs for all numbers up to N.

Examples:

      Number         Binary          Adjacent Bits

         12                    1100                        1

         15                    1111                        3

         27                    11011                      2

# Input

Input starts with an integer T (≤ 10000), denoting the number of test cases.

Each case contains an integer N (0 ≤ N < 231).

# Output

For each test case, print the case number and the summation of all adjacent bits from 0 to N.

# **Sample Input**

**706152021222147483647**

# Sample Output

**Case 1: 0Case 2: 2Case 3: 12Case 4: 13Case 5: 13Case 6: 14Case 7: 16106127360**


————————————————————————————————————————————

题意：给一个整数 n，输出从 0 到 n 中的所有数转为二进制后，其连续两个 1 的个数

思路：数位 DP，设 dp[pos][sum][pre] 表示当前位置 pos 时，前一位是 pre，前面有多少个 11

将 0 到 n 范围的内的数分解为二进制，然后套数位 DP 板子即可

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 1000000007
#define INF 0x3f3f3f3f
#define N 101
#define LL long long
using namespace std;
LL dp[N][N][2];
LL bit[N];
LL dfs(int pos,int sum,int pre,bool limit){
    if(pos<0)
        return sum;

    if(!limit&&dp[pos][sum][pre]!=-1)
        return dp[pos][sum][pre];

    LL res=0;
    int up=limit?bit[pos]:1;
    for(int i=0;i<=up;i++){
        if(pre&i)//前一个是1，后一个也是1
            res+=dfs(pos-1,sum+1,i,limit&&i==up);
        else
            res+=dfs(pos-1,sum,i,limit&&i==up);
    }

    if(!limit)
        dp[pos][sum][pre]=res;

    return res;
}
LL solve(LL x){
    int cnt=0;
    while(x){///转为二进制
        bit[cnt++]=x%2;
        x/=2;
    }
    return dfs(cnt-1,0,0,true);
}
int main(){
    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        LL n;
        scanf("%lld",&n);
        memset(dp,-1,sizeof(dp));
        printf("Case %d: %lld\n",Case++,solve(n));
    }

	return 0;
}
```






