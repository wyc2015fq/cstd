# Looooops（POJ-2115） - Alex_McAvoy的博客 - CSDN博客





2018年08月10日 10:05:29[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：51








> 
# Problem Description

A Compiler Mystery: We are given a C-language style for loop of type 

for (variable = A; variable != B; variable += C)

  statement;

I.e., a loop which starts by setting variable to value A and while variable is not equal to B, repeats statement followed by increasing the variable by C. We want to know how many times does the statement get executed for particular values of A, B and C, assuming that all arithmetics is calculated in a k-bit unsigned integer type (with values 0 <= x < 2k) modulo 2k. 

# Input

The input consists of several instances. Each instance is described by a single line with four integers A, B, C, k separated by a single space. The integer k (1 <= k <= 32) is the number of bits of the control variable of the loop and A, B, C (0 <= A, B, C < 2k) are the parameters of the loop. 

The input is finished by a line containing four zeros. 

# Output

The output consists of several lines corresponding to the instances on the input. The i-th line contains either the number of executions of the statement in the i-th instance (a single integer number) or the word FOREVER if the loop does not terminate.

# Sample Input

**3 3 2 163 7 2 167 3 2 163 4 2 160 0 0 0**

# Sample Output

**0232766FOREVER**


题意：给出 A、B、C、k，求最小的 y，满足：![A+C*y\equiv B\:mod\:(2^k)](https://private.codecogs.com/gif.latex?A&plus;C*y%5Cequiv%20B%5C%3Amod%5C%3A%282%5Ek%29)

思路：解线性同余方程，扩展欧几里得即可解出

化简得：![(2^k)*x + y*C = B-A](https://private.codecogs.com/gif.latex?%282%5Ek%29*x%20&plus;%20y*C%20%3D%20B-A)

即：当且仅当![(B-A) \:mod\: GCD(2^k, C) = 0](https://private.codecogs.com/gif.latex?%28B-A%29%20%5C%3Amod%5C%3A%20GCD%282%5Ek%2C%20C%29%20%3D%200) 时有解

设：![t = (B-A)/GCD(2^k, C)](https://private.codecogs.com/gif.latex?t%20%3D%20%28B-A%29/GCD%282%5Ek%2C%20C%29)

则：![(2^k)*x/t + y*C/t = GCD(2^k, C)](https://private.codecogs.com/gif.latex?%282%5Ek%29*x/t%20&plus;%20y*C/t%20%3D%20GCD%282%5Ek%2C%20C%29)

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
#define N 1005
#define MOD 1e9+7
#define E 1e-6
#define LL long long
using namespace std;
LL Quick_Pow(LL a,LL b)
{
    LL res=1,base=a;
    while(b)
    {
        if(b&1)
            res*=base;
        base*=base;
        b>>=1;
    }
    return res;
}
LL Extended_GCD(LL a,LL b,LL &x,LL &y)
{
    if(b==0)
    {
        x=1;
        y=0;
        return a;
    }
    LL gcd=Extended_GCD(b,a%b,x,y);
    LL temp=x;
    x=y;
    y=temp-a/b*y;
    return gcd;
}
int main()
{
    int a,b,c,k;
    while(scanf("%d%d%d%d",&a,&b,&c,&k)!=EOF&&(a+b+c+k))
    {
        LL x,y;
        LL num=Quick_Pow(2,k);
        LL gcd=Extended_GCD(num,c,x,y);

        b-=a;
        if(b%gcd)
            printf("FOREVER\n");
        else
        {
            x*=(b/gcd);
            y*=(b/gcd);
            y=(y%(num/gcd)+num/gcd)%(num/gcd);
			printf("%lld\n",y);
        }
    }

    return 0;
}
```





