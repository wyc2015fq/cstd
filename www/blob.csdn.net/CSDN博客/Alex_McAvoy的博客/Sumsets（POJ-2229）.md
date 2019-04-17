# Sumsets（POJ-2229） - Alex_McAvoy的博客 - CSDN博客





2018年07月28日 20:41:21[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：62








# Problem Description

Farmer John commanded his cows to search for different sets of numbers that sum to a given number. The cows use only numbers that are an integer power of 2. Here are the possible sets of numbers that sum to 7: 

1) 1+1+1+1+1+1+1 

2) 1+1+1+1+1+2 

3) 1+1+1+2+2 

4) 1+1+1+4 

5) 1+2+2+2 

6) 1+2+4 

Help FJ count all possible representations for a given integer N (1 <= N <= 1,000,000). 

# **Input**

A single line with a single integer, N.

# Output

The number of ways to represent N as the indicated sum. Due to the potential huge size of this number, print only last 9 digits (in base 10 representation).

# Sample Input

**7**

# Sample Output

**6**

————————————————————————————————————————————————————

题意：给一个数 n，以 2 为幂的数的集合的和为 n，求方案数。

思路：递推。

n=1 时，方案数为1，n=2 时，方案数为 2；

当 n 为奇数时，相当于其前的偶数+1，每个式子不变，即：a[n]=a[n-1]；

当 n 为偶数时，除了有 n-2 的式子（n=n-2+1+1），还有以 2 为基数的式子（n=n/2*2），即：a[n]=a[n-2]+a[n/2]。

注意输出时仅打印后九位。

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
#define N 1000001
#define MOD 123
#define E 1e-6
using namespace std;
int a[N];
int main()
{
    int n;
	scanf("%d",&n);

	a[1]=1;
	a[2]=2;
	for(int i=3;i<=n;i++)
    {
        if(i%2)
            a[i]=a[i-1]%1000000000;
        else
            a[i]=(a[i-2]+a[i/2])%1000000000;
    }

    printf("%d\n",a[n]);
	return 0;
}
```





