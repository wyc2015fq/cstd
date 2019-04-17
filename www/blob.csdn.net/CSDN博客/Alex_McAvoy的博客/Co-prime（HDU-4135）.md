# Co-prime（HDU-4135） - Alex_McAvoy的博客 - CSDN博客





2018年08月12日 21:19:56[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：57








> 
# Problem Description

Given a number N, you are asked to count the number of integers between A and B inclusive which are relatively prime to N.

Two integers are said to be co-prime or relatively prime if they have no common positive divisors other than 1 or, equivalently, if their greatest common divisor is 1. The number 1 is relatively prime to every integer.

# **Input**

The first line on input contains T (0 < T <= 100) the number of test cases, each of the next T lines contains three integers A, B, N where (1 <= A <= B <= 10^15) and (1 <=N <= 10^9).

# Output

For each test case, print the number of integers between A and B inclusive which are relatively prime to N. Follow the output format below.

# Sample Input

**21 10 23 15 5**

# Sample Output

**Case #1: 5Case #2: 10**


题意：给出一个整数区间 [a，b]，与一个整数 n，求区间中与 n 互素的数的个数

思路：

问题可转换为区间 [1,b] 中与 n 互素的数的个数减去区间 [1,a-1]中与 n 互素的数的个数

也即给一个数 m，求从 1 到 m 的所有数中与 n 互质的数个个数

因此，先对 n 进行因子分解，从 m 中减去存在 n 的因子的个数，再根据容斥定理，奇加偶减，对 n 的因子的最小公倍数的个数进行处理即可

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
#define N 1000001
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
LL prime[N],factor[N];
bool bprime[N];
LL cnt;
void isprime()
{
    cnt=0;
    memset(bprime,false,sizeof(bprime));

    for(LL i=2;i<N;i++)
    {
        if(!bprime[i])
        {
            prime[cnt++]=i;
            for(LL j=i*i;j<N;j+=i)
                bprime[i]=true;
        }
    }
}
LL calculate(LL m,LL num)
{
    LL res=0;
    for(LL i=1;i<(1<<num);i++)
    {
        LL sum=0;
        LL temp=1;
        for(LL j=0;j<num;j++)
        {
            if(i&(1<<j))
            {
                sum++;
                temp*=factor[j];
            }
        }
        if(sum%2)
            res+=m/temp;
        else
            res-=m/temp;
    }
    return res;
}

int main()
{
    isprime();

    LL t,k=1;
    scanf("%d",&t);
    while(t--)
    {
        LL a,b,n;
        scanf("%lld%lld%lld",&a,&b,&n);

        LL num=0;
        for(LL i=0;prime[i]*prime[i]<=n&&i<cnt;i++)
        {
            if(n%prime[i]==0)
            {
                factor[num++]=prime[i];
                while(n%prime[i]==0)
                    n/=prime[i];
            }
        }
        if(n>1)
            factor[num++]=n;

        LL res;
        res=b-calculate(b,num)-a+1+calculate(a-1,num);

        printf("Case #%lld: %lld\n",k++,res);
    }
    return 0;
}
```





