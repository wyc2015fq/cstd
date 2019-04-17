# Combinations（POJ-1306） - Alex_McAvoy的博客 - CSDN博客





2018年08月13日 06:12:46[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：124








> 
# Problem Description

Computing the exact number of ways that N things can be taken M at a time can be a great challenge when N and/or M become very large. Challenges are the stuff of contests. Therefore, you are to make just such a computation given the following: 

GIVEN: 5 <= N <= 100; 5 <= M <= 100; M <= N 

Compute the EXACT value of: C = N! / (N-M)!M! 

You may assume that the final value of C will fit in a 32-bit Pascal LongInt or a C long. For the record, the exact value of 100! is: 

93,326,215,443,944,152,681,699,238,856,266,700,490,715,968,264,381,621, 468,592,963,895,217,599,993,229,915,608,941,463,976,156,518,286,253, 697,920,827,223,758,251,185,210,916,864,000,000,000,000,000,000,000,000 

# **Input**

The input to this program will be one or more lines each containing zero or more leading spaces, a value for N, one or more spaces, and a value for M. The last line of the input file will contain a dummy N, M pair with both values equal to zero. Your program should terminate when this line is read.

# Output

The output from this program should be in the form: 

N things taken M at a time is C exactly. 

# Sample Input

**100  620  518  60  0**

# Sample Output

**100 things taken 6 at a time is 1192052400 exactly.20 things taken 5 at a time is 15504 exactly.18 things taken 6 at a time is 18564 exactly.**


题意：给出 n、m，求 C(n,m)，跟 [Binomial Showdown（POJ-2249）](https://blog.csdn.net/u011815404/article/details/81623385)没什么区别，只是输出格式不同

思路：模板：[点击这里](https://blog.csdn.net/u011815404/article/details/81366594)

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
#define N 100000001
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
LL calculate(LL n,LL m)//sum即为C(n,m)的值
{
    if(2*m>n)
        m=n-m;
    LL sum=1;
    for(LL i=1,j=n;i<=m;i++,j--)
        sum=sum*j/i;
    return sum;
}
int main()
{
    LL n,m;
    while(scanf("%lld%lld",&n,&m)!=EOF&&(n+m))
    {
        LL res=calculate(n,m);
        printf("%lld things taken %lld at a time is %lld exactly.\n",n,m,res);
    }
    return 0;
}
```





