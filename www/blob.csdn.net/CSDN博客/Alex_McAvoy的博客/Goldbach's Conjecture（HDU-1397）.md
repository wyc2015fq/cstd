# Goldbach's Conjecture（HDU-1397） - Alex_McAvoy的博客 - CSDN博客





2018年08月11日 20:39:19[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：63
个人分类：[HDU																[数论——素性测试](https://blog.csdn.net/u011815404/article/category/8796920)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Goldbach's Conjecture: For any even number n greater than or equal to 4, there exists at least one pair of prime numbers p1 and p2 such that n = p1 + p2. 

This conjecture has not been proved nor refused yet. No one is sure whether this conjecture actually holds. However, one can find such a pair of prime numbers, if any, for a given even number. The problem here is to write a program that reports the number of all the pairs of prime numbers satisfying the condition in the conjecture for a given even number.

A sequence of even numbers is given as input. Corresponding to each number, the program should output the number of pairs mentioned above. Notice that we are interested in the number of essentially different pairs and therefore you should not count (p1, p2) and (p2, p1) separately as two different pairs.

# **Input**

An integer is given in each input line. You may assume that each integer is even, and is greater than or equal to 4 and less than 2^15. The end of the input is indicated by a number 0.

# Output

Each output line should contain an integer number. No other characters should appear in the output.

# Sample Input

**610120**

# Sample Output

**121**


题意：给一个大于等于 4 的偶数，将它分解为两个素数的和，求这样的素数对的个数

思路：先打表求素数，显然每个素数对中，一个数比 n/2 大，另一个比 n/2 小，因此从 1 遍历到 n/2 即可

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
#define N 100001
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

    int n;
    while(scanf("%d",&n)!=EOF&&n)
    {
        int sum=0;
        for(int i=2;i<=n/2;i++)
            if(bprime[i]&&bprime[n-i])
                sum++;

        printf("%d\n",sum);
    }

    return 0;
}
```






