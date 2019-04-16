# HDU2837 Calculation 指数循环节 欧拉函数+快速幂 - 紫芝的博客 - CSDN博客





2018年09月19日 16:41:32[紫芝](https://me.csdn.net/qq_40507857)阅读数：32








# Calculation

****Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 32768/32768 K (Java/Others)Total Submission(s): 3848    Accepted Submission(s): 925****

**Problem Description**

Assume that f(0) = 1 and 0^0=1. f(n) = (n%10)^f(n/10) for all n bigger than zero. Please calculate f(n)%m. (2 ≤ n , m ≤ 10^9, x^y means the y th power of x).

**Input**

The first line contains a single positive integer T. which is the number of test cases. T lines follows.Each case consists of one line containing two positive integers n and m.

**Output**

One integer indicating the value of f(n)%m.

**Sample Input**

2 

24 20 

25 20

**Sample Output**

16 

5

**Source**

[2009 Multi-University Training Contest 3 - Host by WHU](http://acm.hdu.edu.cn/search.php?field=problem&key=2009+Multi-University+Training+Contest+3+-+Host+by+WHU+&source=1&searchmode=source)

指数循环节：A^x=A^(x%phi(c)+phi(c))(mod c)

若非常大时，也需要用到这个公式，x较大时才用到这个公式，快速幂取模时要区分x是否较大

## A^x = A^(x % Phi(C) + Phi(C)) (mod C) （x>=Phi(C)）

注意括号里的条件，那么当n比较小的时候，就不能用这个了，n比较小，肯定是暴力嘛。

```cpp
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<queue>
#include<stack>
#include<math.h>
#include<vector>
#include<map>
#include<set>
#include<stdlib.h>
#include<cmath>
#include<string>
#include<algorithm>
#include<iostream>
#define exp 1e-10
#define MAX(a,b) ((a)>(b)?(a):(b))
#define mod 1000000007
using namespace std;
typedef long long ll;
ll phi(ll x)
{
    ll res=x;
    for(ll i=2;i*i<=x;i++)
    {
        if(x%i==0){
        res-=res/i;
        while(x%i==0)   x=x/i;
        }
    }
    if(x>1) res-=res/x;
    return res;
}
ll Pow(ll a,ll b,ll m)
{
    ll ans=1;
    for(int i=1;i<=b;++i)
    {
        ans*=a;
        if(ans>=m)  return ans;
    }
    return ans;
}
ll quickPow(ll a,ll b,ll m)
{
    //注意要区分a^b%m,b是否较大，较大才可以用那个公式
    ll now=Pow(a,b,m);
    if(now>=m)  now=m;
    else now=0;
    ll res=1;
    while(b){
        if(b&1) res=res*a%m;
        a=a*a%m;
        b>>=1;
    }
    return res+now;
}
ll f(ll n,ll m)
{
    if(n<10)    return n;
    ll p=phi(m);
    ll x=n%10,y=f(n/10,p);
    return quickPow(x,y,m);
}
int main()
{
    //printf("%lld\n",quickPow(2,3,11));
    //printf("%lld",phi(10));
    ll n,m;
    int T;
    scanf("%d",&T);
    while(T--){
    scanf("%lld%lld",&n,&m);
        ll ans=f(n,m)%m;
        printf("%lld\n",ans);
    }
}
```





