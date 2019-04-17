# Binomial Showdown（POJ-2249） - Alex_McAvoy的博客 - CSDN博客





2018年08月13日 06:11:42[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：102








> 
# Problem Description

In how many ways can you choose k elements out of n elements, not taking order into account? 

Write a program to compute this number.

# **Input**

The input will contain one or more test cases. 

Each test case consists of one line containing two integers n (n>=1) and k (0<=k<=n). 

Input is terminated by two zeroes for n and k.

# Output

For each test case, print one line containing the required number. This number will always fit into an integer, i.e. it will be less than 231. 

Warning: Don't underestimate the problem. The result will fit into an integer - but if all intermediate results arising during the computation will also fit into an integer depends on your algorithm. The test cases will go to the limit. 

# Sample Input

**4 210 549 60 0**

# Sample Output

**625213983816**


题意：给出两个整数 n、k，求 C(n, k)

思路：套模板即可，模板：[点击这里](https://blog.csdn.net/u011815404/article/details/81366594)

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
    LL n,k;
    while(scanf("%lld%lld",&n,&k)!=EOF&&(n+k))
    {
        LL res=calculate(n,k);
        printf("%lld\n",res);
    }
    return 0;
}
```





