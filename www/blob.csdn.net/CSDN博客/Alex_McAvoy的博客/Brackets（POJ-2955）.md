# Brackets（POJ-2955） - Alex_McAvoy的博客 - CSDN博客





2018年08月18日 10:00:16[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：163








> 
# Problem Description

We give the following inductive definition of a “regular brackets” sequence:

the empty sequence is a regular brackets sequence,

if s is a regular brackets sequence, then (s) and [s] are regular brackets sequences, and

if a and b are regular brackets sequences, then ab is a regular brackets sequence.

no other sequence is a regular brackets sequence

For instance, all of the following character sequences are regular brackets sequences:

(), [], (()), ()[], ()[()]

while the following character sequences are not:

(, ], )(, ([)], ([(]

Given a brackets sequence of characters a1a2 … an, your goal is to find the length of the longest regular brackets sequence that is a subsequence of s. That is, you wish to find the largest m such that for indices i1, i2, …, im where 1 ≤ i1 < i2 < … < im ≤ n, ai1ai2 … aim is a regular brackets sequence.

Given the initial sequence ([([]])], the longest regular brackets subsequence is [([])].

# **Input**

The input test file will contain multiple test cases. Each input test case consists of a single line containing only the characters (, ), [, and ]; each input test will have length between 1 and 100, inclusive. The end-of-file is marked by a line containing the word “end” and should not be processed.

# Output

For each input case, the program should print the length of the longest possible regular brackets subsequence on a single line.

# Sample Input

**((()))()()()([]]))[)(([][][)end**

# Sample Output

**66406**


题意：给出一串括号字符，求最多的匹配个数。

思路：

求括号匹配的最多个数，那么可以用区间 DP 来解决

设 dp[i][j] 为从第 i 个位置到第 j 个位置的最大匹配数，那么状态转移方程有两种情况：
- 如果边界匹配，则：dp[i][j] = dp[i+1][j-1]+2 
- 如果边界不匹配，则：dp[i][j] = max(dp[i][j] , dp[i][k]+dp[k+1][j])

此外，由于括号一共只有 4 种，那么可以在读入时转换为数字，然后再进行区间 DP

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
#define E 1e-9
#define INF 0x3f3f3f3f
#define LL long long
const int MOD=10007;
const int N=1000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
int a[N];
char str[N];
int dp[N][N];
int main(){
    while(scanf("%s",&str)!=EOF&&(str[0]!='e')){
        memset(dp,0,sizeof(dp));

        int n=strlen(str);
        for(int i=0;i<n;i++){//转为数字
            if(str[i]=='(')
                a[i]=1;
            else if(str[i]==')')
                a[i]=-1;
            else if(str[i]=='[')
                a[i]=2;
            else if(str[i]==']')
                a[i]=-2;
        }

        for(int len=1;len<n;len++){//枚举长度
            for(int i=0;i<n-1;i++){//枚举起点
                int j=i+len;//终点
                if(a[i]+a[j]==0&&a[i]>0)//匹配
                    dp[i][j]=dp[i+1][j-1]+2;

                for(int k=0;k<len;k++)//不匹配
                    dp[i][j]=max(dp[i][j],dp[i][i+k]+dp[i+k+1][j]);
            }
        }

        printf("%d\n",dp[0][n-1]);
    }
    return 0;
}
```





