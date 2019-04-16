# ZOJ3557 How Many Sets II  插板法求组合数 - 紫芝的博客 - CSDN博客





2018年09月13日 18:17:06[紫芝](https://me.csdn.net/qq_40507857)阅读数：1406








# [How Many Sets II](https://cn.vjudge.net/contest/253889#problem/C)

Given a set *S* = {1, 2, ..., *n*}, number *m* and *p*, your job is to count how many set *T* satisfies the following condition:
- *T* is a subset of *S*
- |*T*| = *m*
- *T* does not contain continuous numbers, that is to say *x* and *x*+1 can not both in *T*



Input

There are multiple cases, each contains 3 integers *n* ( 1 <= *n* <= 109 ), *m* ( 0 <= *m* <= 104, *m* <= *n* ) and *p* ( *p* is prime, 1 <= *p* <= 109 ) in one line seperated by a single space, proceed to the end of file.

Output

Output the total number mod *p*.

Sample Input

5 1 11
5 2 11

Sample Output

5
6

题意：

一共有n个元素，选择m个不相邻的元素，一共有多少种选法

分析：

用插板法求出组合数。从n个元素中选出m个，剩下n-m个元素产生n-m+1个空位，把m个元素插到n-m+1个空位，方法数为C(n-m+1,m);

```cpp
#include<cstdio>
using namespace std;
typedef long long ll;
ll fastPow(ll a,ll b,ll p)
{
    ll ret=1;
    while(b){
        if(b&1) ret=(ret*a)%p;
        a=(a*a)%p;
        b>>=1;
    }
    return ret;
}
ll c(ll n,ll m,ll p)
{

    if(m==0)    return 1;
    //if(m>n-m)   m=n-m;
    ll up=1,down=1;
    for(int i=1;i<=m;i++)
    {
        up=up*(n-i+1)%p;
        down=(down*i)%p;
    }
    return up*fastPow(down,p-2,p)%p;
}

ll lucas(ll n,ll m,ll p)
{
    if(m==0)    return 1;
    return c(n%p,m%p,p)*lucas(n/p,m/p,p)%p;
}
/*
ll lucas(ll n,ll m,ll p)
{
    ll ans=1;
    while(n&&m&&ans){
        ans=ans*c(n%p,m%p,p)%p;
        n/=p;
        m/=p;
    }
    return ans;
}
*/
int main()
{
    ll n,m,p;
    while(scanf("%lld%lld%lld",&n,&m,&p)!=EOF){
        n=n-m+1;
        if(n<m){
            printf("0\n");
            continue;
        }
        if(n==m)
        {
        printf("1\n");
        continue;
        }
        if(m>n-m)
            m=n-m;
        printf("%lld\n",lucas(n,m,p));
    }
    return 0;
}
```





