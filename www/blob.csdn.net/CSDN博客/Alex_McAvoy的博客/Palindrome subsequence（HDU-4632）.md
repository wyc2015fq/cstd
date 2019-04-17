# Palindrome subsequence（HDU-4632） - Alex_McAvoy的博客 - CSDN博客





2018年08月17日 12:47:46[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：53
个人分类：[HDU																[动态规划——区间 DP](https://blog.csdn.net/u011815404/article/category/7813197)](https://blog.csdn.net/u011815404/article/category/7923257)








# Problem Description

In mathematics, a subsequence is a sequence that can be derived from another sequence by deleting some elements without changing the order of the remaining elements. For example, the sequence <A, B, D> is a subsequence of <A, B, C, D, E, F>.

(http://en.wikipedia.org/wiki/Subsequence)

Given a string S, your task is to find out how many different subsequence of S is palindrome. Note that for any two subsequence X = <Sx1, Sx2, ..., Sxk> and Y = <Sy1, Sy2, ..., Syk> , if there exist an integer i (1<=i<=k) such that xi != yi, the subsequence X and Y should be consider different even if Sxi = Syi. Also two subsequences with different length should be considered different.

# **Input**

The first line contains only one integer T (T<=50), which is the number of test cases. Each test case contains a string S, the length of S is not greater than 1000 and only contains lowercase letters.

# Output

For each test case, output the case number first, then output the number of different subsequence of the given string, the answer should be module 10007.

# Sample Input

**4aaaaaagoodafternooneveryonewelcometoooxxourproblems**

# Sample Output

**Case 1: 1Case 2: 31Case 3: 421Case 4: 960**

————————————————————————————————————————————————————

题意：给出 n 组字符串，要求找到每组字符串中有多少个回文串子序列

思路：一道典型的区间DP应用题

设 dp[i][j] 为 i 到 j 内回文子串的个数，由于要记录子序列的个数，因此 i-j 的子序列可以就前一个状态推过来，而 i~j 的前一个状态有两个：(i+1)~j 与 i~(j-1)，由于这两个状态都包含 (i+1)~(j-1) 的状态，相加后会重复，因此要减去一个，因此得到状态转移方程：dp[i][j]=dp[i][j-1]+dp[i+1][j]-dp[i+1][j-1]

当字符串两个字母相同，即：str[i]=str[j] 时，就可以组成回文，那么就是和 (i+1)~(j-1) 状态的回文组成新的回文，因此得到状态转移方程：dp[i][j]=dp[i][j]+dp[i+1][j-1]+1

还有一注意的点是，记得求模

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
#define N 10001
#define MOD 10007
#define E 1e-6
#define LL long long
using namespace std;
int dp[N][N];
int main()
{
    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--)
    {
        string str;
        cin>>str;
        int len=str.length();

        for(int i=0;i<len;i++)//初始化，单个字符一定是一个回文子串
            dp[i][i]=1;

        for(int i=1;i<len;i++)
        {
            for(int j=i-1;j>=0;j--)
            {
                dp[j][i]=(dp[j+1][i]+dp[j][i-1]-dp[j+1][i-1]+MOD)%MOD;//状态一
                
                if(str[i]==str[j])//状态二
                    dp[j][i]=(dp[j][i]+dp[j+1][i-1]+1+MOD)%MOD;
            }
        }
        printf("Case %d: %d\n",Case++,dp[0][len-1]);
    }


    return 0;
}
```






