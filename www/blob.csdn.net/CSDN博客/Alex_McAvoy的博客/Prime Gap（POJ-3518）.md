# Prime Gap（POJ-3518） - Alex_McAvoy的博客 - CSDN博客





2018年08月11日 12:23:57[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：59
个人分类：[POJ																[数论——素性测试](https://blog.csdn.net/u011815404/article/category/8796920)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

The sequence of n − 1 consecutive composite numbers (positive integers that are not prime and not equal to 1) lying between two successive prime numbers p and p + n is called a prime gap of length n. For example, ‹24, 25, 26, 27, 28› between 23 and 29 is a prime gap of length 6.

Your mission is to write a program to calculate, for a given positive integer k, the length of the prime gap that contains k. For convenience, the length is considered 0 in case no prime gap contains k.

# **Input**

The input is a sequence of lines each of which contains a single positive integer. Each positive integer is greater than 1 and less than or equal to the 100000th prime number, which is 1299709. The end of the input is indicated by a line containing a single zero.

# Output

The output should be composed of lines each of which contains a single non-negative integer. It is the length of the prime gap that contains the corresponding positive integer in the input if it is a composite number, or 0 otherwise. No other characters should occur in the output.

# Sample Input

**10112724921700**

# Sample Output

**4060114**


题意：两个素数 a、b 之间的区间 (a,b] 称谓非素数区间，给出一个数 n，求 n 所在的非素数区间长度。

思路：先打表求素数，若 n 为素数，则长度为0，若 n 为合数，找出相邻两素数，长度为两素数差

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
#define N 5000005
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
LL prime[N],cnt;
bool bprime[N];
void make_prime()
{
	memset(bprime,true,sizeof(bprime));
	bprime[0]=false;
	bprime[1]=false;

    for(LL i=2;i<=N;i++)
    {
        if(bprime[i])
        {
            prime[cnt++]=i;
            for(LL j=i*i;j<=N;j+=i)
                bprime[j]=false;
        }
    }
}
int main()
{
    make_prime();

    LL n;
    while(scanf("%lld",&n)!=EOF&&n)
    {
        if(bprime[n])
            printf("0\n");
        else
        {
            for(LL i=0;i<cnt;i++)
                if(prime[i]<n&&prime[i+1]>n)
                    printf("%lld\n",prime[i+1]-prime[i]);
        }
    }

    return 0;
}
```






