# Strange Way to Express Integers（POJ-2891） - Alex_McAvoy的博客 - CSDN博客





2018年08月09日 14:45:09[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：95








> 
# Problem Description

Elina is reading a book written by Rujia Liu, which introduces a strange way to express non-negative integers. The way is described as following:

Choose k different positive integers a1, a2, …, ak. For some non-negative m, divide it by every ai (1 ≤ i ≤ k) to find the remainder ri. If a1, a2, …, ak are properly chosen, m can be determined, then the pairs (ai, ri) can be used to express m.

“It is easy to calculate the pairs from m, ” said Elina. “But how can I find m from the pairs?”

Since Elina is new to programming, this problem is too difficult for her. Can you help her?

# Input

The input contains multiple test cases. Each test cases consists of some lines.

Line 1: Contains the integer k.

Lines 2 ~ k + 1: Each contains a pair of integers ai, ri (1 ≤ i ≤ k).

# Output

Output the non-negative integer m on a separate line for each test case. If there are multiple possible values, output the smallest one. If there are no possible values, output -1.

# Sample Input

**28 711 9**

# Sample Output

**31**


题意：存在一个数 x，满足 x%ai = ri，现给出 n 对 ai 和 ri，求 x 的最小非负整数，如果不存在输出-1

思路：不互素的中国剩余定理模版题。。

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 16007
#define INF 0x3f3f3f3f
#define N 1000001
#define LL long long
using namespace std;
LL Extended_GCD(LL a,LL b,LL &x,LL &y){
    if(b==0){
        x=1;
        y=0;
        return a;
    }

    LL gcd=Extended_GCD(b,a%b,y,x);
    y=y-a/b*x;
    return gcd;
}
LL gcd(LL a,LL b){
    return b==0?a:gcd(b,a%b);
}
LL CRT(LL W[],LL B[],LL n)
{
    LL res=B[0],Wi=W[0];
    for (LL i=1;i<n;i++)
    {
        LL bi=B[i],wi=W[i];
        LL x,y;
        LL gcd=Extended_GCD(Wi,wi,x,y);
        LL c=bi-res;

        if(c%gcd!=0)
            return -1;

        LL M=wi/gcd;
        res+=Wi*( ((c/gcd*x)%M+M) % M);
        Wi*=M;
    }
    if(res==0)
    {
        res=1;
        for(LL i=0;i<n;i++)
            res=res*W[i]/gcd(res,(LL)W[i]);
    }
    return res;
}
LL a[N],b[N];
int main(){
    LL k;
    while(scanf("%lld",&k)!=EOF&&k){
        for(LL i=0;i<k;i++){
            scanf("%lld%lld",&a[i],&b[i]);//先除数后余数
        }
        printf("%lld\n",CRT(a,b,k));
    }

    return 0;
}
```





