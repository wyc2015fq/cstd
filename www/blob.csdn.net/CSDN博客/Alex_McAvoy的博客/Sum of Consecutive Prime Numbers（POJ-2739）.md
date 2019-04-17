# Sum of Consecutive Prime Numbers（POJ-2739） - Alex_McAvoy的博客 - CSDN博客





2018年08月10日 11:15:41[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：73
个人分类：[POJ																[数论——素性测试](https://blog.csdn.net/u011815404/article/category/8796920)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

Some positive integers can be represented by a sum of one or more consecutive prime numbers. How many such representations does a given positive integer have? For example, the integer 53 has two representations 5 + 7 + 11 + 13 + 17 and 53. The integer 41 has three representations 2+3+5+7+11+13, 11+13+17, and 41. The integer 3 has only one representation, which is 3. The integer 20 has no such representations. Note that summands must be consecutive prime 

numbers, so neither 7 + 13 nor 3 + 5 + 5 + 7 is a valid representation for the integer 20. 

Your mission is to write a program that reports the number of representations for the given positive integer.

# **Input**

The input is a sequence of positive integers each in a separate line. The integers are between 2 and 10 000, inclusive. The end of the input is indicated by a zero.

# Output

The output should be composed of lines each corresponding to an input line except the last zero. An output line includes the number of representations for the input integer as the sum of one or more consecutive prime numbers. No other characters should be inserted in the output.

# Sample Input

**2317412066612530**

# Sample Output

**11230012**


题意：给出一个数 n，要将其分解为几个素数之和，求一共有多少种方法

思路：先线性筛打表求素数，然后深搜即可

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
int prime[N],cnt;
int ans,n;
void getPrimes(int n)
{
	bool bPrime[N];// 素数标志数组

	cnt = 0;// 素数个数
	memset(bPrime, true, sizeof(bPrime));// 假设全部为素数
	bPrime[0] = false;// 0: 非素数
	bPrime[1] = false;// 1: 非素数

	for(int i = 2; i <= n; i++)//枚举2-n的所有数
	{
		if(bPrime[i])//保存素数i
			prime[cnt++] = i;

		for(int j = 0; j < cnt && i * prime[j] < N; j++)
		{
			bPrime[i * prime[j]] = false;//置非素数标志
			if(i % prime[j] == 0)//i为素数的倍数
				break;
		}
	}
}
void dfs(int sum,int num)
{
    if(sum==n)
    {
        ans++;
        return;
    }

    if(sum+prime[num]>n)
        return;
    else
        dfs(sum+prime[num],num+1);

    return;
}
int main()
{
    getPrimes(N);

    while(scanf("%d",&n)!=EOF&&n)
    {
        ans=0;
        for(int i=0;i<n;i++)
            dfs(0,i);
        printf("%d\n",ans);
    }
    return 0;
}
```






