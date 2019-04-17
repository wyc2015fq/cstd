# Problem Makes Problem（LightOJ-1102） - Alex_McAvoy的博客 - CSDN博客





2019年03月23日 16:49:26[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：27
个人分类：[LightOJ																[—————组合数学—————](https://blog.csdn.net/u011815404/article/category/7900800)](https://blog.csdn.net/u011815404/article/category/8037630)








> 
# Problem Description

As I am fond of making easier problems, I discovered a problem. Actually, the problem is 'how can you make n by adding k non-negative integers?' I think a small example will make things clear. Suppose n=4 and k=3. There are 15 solutions. They are

1.    0 0 4

2.    0 1 3

3.    0 2 2

4.    0 3 1

5.    0 4 0

6.    1 0 3

7.    1 1 2

8.    1 2 1

9.    1 3 0

10.  2 0 2

11.  2 1 1

12.  2 2 0

13.  3 0 1

14.  3 1 0

15.  4 0 0

As I have already told you that I use to make problems easier, so, you don't have to find the actual result. You should report the result modulo 1000,000,007.

# **Input**

For each case, print the case number and the result modulo 1000000007.

# Output

For each case, output the case number and the desired value. Since the result can be very large, you have to print the result modulo 1000003.

# Sample Input

**44 33 51000 31000 5**

# Sample Output

**Case 1: 15Case 2: 35Case 3: 501501Case 4: 84793457**


题意：t 组数据，每组给出 n、k 两个数，要求将数 n 分为非负的 k 份，问有几种分法

思路：

实质就是求方程 n=x1+x2+x3+...+xk 非负解的个数，可以将 n+k 分成 k 份，然后分出来的 k 个数每个再减1，这样就可以用隔板法用 k-1 个板隔成 k 份，也就是求 C(n+k-1,k-1)

由于需要模 1000000007，因此接下来就是求组合数 C(n+k-1,k-1)%1000000007，使用卢卡斯定理即可解决

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
const LL MOD=1000000007;
const int N=2000000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

LL fac[N];
void getFac(){//构造阶乘
    fac[0]=1;
    for(int i=1;i<=N;i++){
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
        printf("Case %d: %lld\n",Case++,Lucas(n+k-1,k-1,MOD));
    }
    return 0;
}
```






