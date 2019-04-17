# Goldbach's Conjecture（POJ-2262） - Alex_McAvoy的博客 - CSDN博客





2018年08月10日 20:04:47[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：55
个人分类：[POJ																[数论——素性测试](https://blog.csdn.net/u011815404/article/category/8796920)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

In 1742, Christian Goldbach, a German amateur mathematician, sent a letter to Leonhard Euler in which he made the following conjecture: 

Every even number greater than 4 can be 

written as the sum of two odd prime numbers.

For example:

8 = 3 + 5. Both 3 and 5 are odd prime numbers. 

20 = 3 + 17 = 7 + 13. 

42 = 5 + 37 = 11 + 31 = 13 + 29 = 19 + 23.

Today it is still unproven whether the conjecture is right. (Oh wait, I have the proof of course, but it is too long to write it on the margin of this page.) 

Anyway, your task is now to verify Goldbach's conjecture for all even numbers less than a million. 

# **Input**

The input will contain one or more test cases. 

Each test case consists of one even integer n with 6 <= n < 1000000. 

Input will be terminated by a value of 0 for n.

# Output

For each test case, print one line of the form n = a + b, where a and b are odd primes. Numbers and operators should be separated by exactly one blank like in the sample output below. If there is more than one pair of odd primes adding up to n, choose the pair where the difference b - a is maximized. If there is no such pair, print a line saying "Goldbach's conjecture is wrong."

# Sample Input

**820420**

# Sample Output

**8 = 3 + 520 = 3 + 1742 = 5 + 37**


题意：给一个大于 4 的数 n，它是两个素数的和，求两个素数相差最大。

思路：暴力枚举，然后注意一下输出格式即可。。。

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
#define N 1000005
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
bool prime(int n)
{
    if(n==2)
        return true;

    for(int i=2;i*i<=n;i++)
        if(n%i==0)
            return false;

    return true;
}
int main()
{
    int n;
    while(scanf("%d",&n)&&n)
    {
        bool flag=true;
        for(int i=3;i<=n;i+=2)
        {
            if(prime(i)&&prime(n-i))
            {
                printf("%d = %d + %d\n",n,i,n-i);
                flag=false;
                break;
            }
        }

        if(flag)
            printf("Goldbach's conjecture is wrong.\n");
    }

    return 0;
}
```






