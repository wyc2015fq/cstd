# Combinations（LightOJ-1067） - Alex_McAvoy的博客 - CSDN博客





2019年03月23日 16:18:38[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：78








> 
# Problem Description

Given n different objects, you want to take k of them. How many ways to can do it?

For example, say there are 4 items; you want to take 2 of them. So, you can do it 6 ways.

Take 1, 2

Take 1, 3

Take 1, 4

Take 2, 3

Take 2, 4

Take 3, 4

# **Input**

Input starts with an integer T (≤ 2000), denoting the number of test cases.

Each test case contains two integers n (1 ≤ n ≤ 106), k (0 ≤ k ≤ n).

# Output

For each case, output the case number and the desired value. Since the result can be very large, you have to print the result modulo 1000003.

# Sample Input

**34 25 06 4**

# Sample Output

**Case 1: 6Case 2: 1Case 3: 15**


题意：t 组数据，每组给出 n、k 两个数，求 C(n,k)%1000003

思路：卢卡斯定理模版题

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
#define E 1e-9
#define INF 0x3f3f3f3f
#define LL long long
const LL MOD=1000003;
const int N=1000000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

LL fac[N];
void getFac(){//构造阶乘
    fac[0]=1;
    for(int i=1;i<=1000000;i++){
        fac[i]=fac[i-1]*i%MOD;
    }
}
LL quickPowMod(LL a,LL b,LL mod){//快速幂
    LL res=1;
    while(b){
        if(b&1)
            res=res*a%mod;
        b>>=1;
        a=a*a%mod;
    }
    return res;
}
LL getC(LL n,LL m,LL mod){//获取C(n,m)%mod
    if(m>n)
        return 0;
    return fac[n]*(quickPowMod(fac[m]*fac[n-m]%mod,mod-2,mod))%mod;
}
LL Lucas(LL n,LL m,LL mod){//卢卡斯定理
    if(m==0)
        return 1;
    return getC(n%mod,m%mod,mod)*Lucas(n/mod,m/mod,mod)%mod;
}
int main(){
    getFac();

    int t;
    scanf("%d",&t);

    int Case=1;
    while(t--){
        LL n,k;
        scanf("%lld%lld",&n,&k);
        printf("Case %d: %lld\n",Case++,Lucas(n,k,(LL)MOD));
    }
    return 0;
}
```





