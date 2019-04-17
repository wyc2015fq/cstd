# Prime Test（POJ-1811） - Alex_McAvoy的博客 - CSDN博客





2018年08月11日 19:18:48[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：100








> 
# Problem Description

Given a big integer number, you are required to find out whether it's a prime number.

# **Input**

The first line contains the number of test cases T (1 <= T <= 20 ), then the following T lines each contains an integer number N (2 <= N < 254).

# Output

For each test case, if N is a prime number, output a line containing the word "Prime", otherwise, output a line containing the smallest prime factor of N.

# Sample Input

**2510**

# Sample Output

**Prime2**


题意：给出 t 组数据，每组数据给出一个整数 n，如果 n 是素数就输出 Prime，不是素数就输出其最小因数

思路：组合使用 Pollard Rho 算法与 Miller Rabin 算法，可求出大整数的所有因子。

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
#define N 101
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
LL Mult_Mod(LL a,LL b,LL m)//res=(a*b)%m
{
    a%=m;
    b%=m;
    LL res=0;
    while(b)
    {
        if(b&1)
            res=(res+a)%m;
        a=(a<<=1)%m;
        b>>=1;
    }
    return res%m;
}
LL Pow_Mod(LL a, LL b, LL m)//res=(a^b)%m
{
    LL res=1;
    LL k=a;
    while(b)
    {
        if((b&1))
            res=Mult_Mod(res,k,m)%m;

        k=Mult_Mod(k,k,m)%m;
        b>>=1;
    }
    return res%m;
}

bool Witness(LL a,LL n,LL x,LL sum)
{
    LL judge=Pow_Mod(a,x,n);
    if(judge==n-1||judge==1)
        return 1;

    while(sum--)
    {
        judge=Mult_Mod(judge,judge,n);
        if(judge==n-1)
            return 1;
    }
    return 0;
}

bool Miller_Rabin(LL n)
{
    if(n<2)
        return 0;
    if(n==2)
        return 1;
    if((n&1)==0)
        return 0;

    LL x=n-1;
    LL sum=0;
    while(x%2==0)
    {
        x>>=1;
        sum++;
    }


    int times=20;
    for(LL i=1;i<=times;i++)
    {
        LL a=rand()%(n-1)+1;//取与p互质的整数a
        if(!Witness(a,n,x,sum))//费马小定理的随机数检验
            return 0;
    }
    return 1;
}
LL GCD(LL a,LL b)
{
    return b==0?a:GCD(b,a%b);
}
LL Pollard_Rho(LL n,LL c)//寻找一个因子
{
    LL i=1,k=2;
    LL x=rand()%n;//产生随机数x0(并控制其范围在1 ~ x-1之间)
    LL y=x;
    while(1)
    {
        i++;
        x=(Mult_Mod(x,x,n)+c)%n;
        LL gcd=GCD(y-x,n);

        if(gcd<0)
            gcd=-gcd;

        if(gcd>1&&gcd<n)
            return gcd;

        if(y==x)
            return n;

        if(i==k)
        {
            y=x;
            k<<=1;
        }
    }
}

int total;//因子的个数
LL factor[N];
void Find_fac(LL n)//对n进行素因子分解，存入factor
{
    if(Miller_Rabin(n))//是素数就把这个素因子存起来
    {
        factor[++total]=n;
        return;
    }

    long long p=n;
    while(p>=n)//值变化，防止陷入死循环k
        p=Pollard_Rho(p,rand()%(n-1)+1);

    Find_fac(n/p);
    Find_fac(p);
}
int main()
{
    int t;
    scanf("%d",&t);

    while(t--)
    {
        LL n;
        scanf("%lld",&n);

        if(Miller_Rabin(n))
            printf("Prime\n");
        else
        {
            total=0;
            memset(factor,0,sizeof(factor));

            Find_fac(n);

            sort(factor+1,factor+1+total);

            printf("%lld\n",factor[1]);
        }

    }

    return 0;
}
```





