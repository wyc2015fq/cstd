# Pseudoprime numbers（POJ-3641） - Alex_McAvoy的博客 - CSDN博客





2018年08月10日 16:34:36[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：72








> 
# Problem Description

Fermat's theorem states that for any prime number p and for any integer a > 1, ap = a (mod p). That is, if we raise a to the pth power and divide by p, the remainder is a. Some (but not very many) non-prime values of p, known as base-a pseudoprimes, have this property for some a. (And some, known as Carmichael Numbers, are base-a pseudoprimes for all a.)

Given 2 < p ≤ 1000000000 and 1 < a < p, determine whether or not p is a base-a pseudoprime.

# **Input**

Input contains several test cases followed by a line containing "0 0". Each test case consists of a line containing p and a.

# Output

For each test case, output "yes" if p is a base-a pseudoprime; otherwise output "no".

# Sample Input

**3 210 3341 2341 31105 21105 30 0**

# Sample Output

**nonoyesnoyes**
**yes**


题意：给出 p 和 a，若 a^p 对 p 取余且 p 不是素数，则输出 yes，否则输出 no

思路：大整数快速幂求模，加素数判断

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
LL Pow_Mod(LL a, LL b, LL m)
{
    LL res=1;
    while(b)
    {
        if(b&1)
            res=(res*a)%m;
        a=(a*a)%m;
        b>>=1;
    }
    return res;
}
bool prime(LL n)
{
    if(n==2)
        return true;

    for(LL i=2;i*i<=n;i++)
        if(n%i==0)
            return false;

    return true;
}
int main()
{
    LL p,a;
    while(scanf("%lld%lld",&p,&a)!=EOF&&(p+a))
    {
        if( !prime(p) && Pow_Mod(a,p,p)==a )
            printf("yes\n");
        else
            printf("no\n");
    }
    return 0;
}
```





