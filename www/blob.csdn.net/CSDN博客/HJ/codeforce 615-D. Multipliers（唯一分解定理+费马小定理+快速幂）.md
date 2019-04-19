# codeforce 615-D. Multipliers（唯一分解定理+费马小定理+快速幂） - HJ - CSDN博客
2018年08月08日 11:09:35[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：73
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
[http://codeforces.com/problemset/problem/615/D](http://codeforces.com/problemset/problem/615/D)
题目大意：
给出若干个素因子，这些素因子连乘会形成一个很大很大的数M，问这个M所有因子连乘的结果对1e9+7是多少？
解题思路：
枚举素因子出现的种类以及每种出现的素因子个数。
接下来可以利用唯一分解定理求因子个数的，假设
```
M = p1^a1 * p2^a2 * ... * pk^ak
```
那么M的因子个数为
```
Q = (a1+1) * (a2+1) * ... * (ak+1)
```
而对于第 i 种素因子 pi ，可以出现1~n次，统计每次出现的个数，结果就为
```
[Q / (ai + 1) ] * ai / 2
```
但是结果很大，需要用费马小定理将结果对1e9+6进行取膜，所以不可以有除数
因此需要首先就将 ai 挑出去，因此可以利用前缀和和后缀和来枚举计数
最后将结果快速幂取膜即可
```cpp
#include<iostream>
#include<cstring>
#include<vector>
using namespace std;
typedef long long ll;
#define mod 1000000007
const int maxn=2e5+10;
int num[maxn],cnt[maxn];
ll l[maxn],r[maxn];
ll quick_mod(ll a,ll n)
{
    ll ans=1;
    while(n)
    {
        if(n%2)
          ans=ans*a%mod;
        a=a*a%mod;
        n/=2;
    }
    return ans;
}
int main()
{
    ll m,x;
    while(cin>>m)
    {
        int k=0;
        memset(num,0,sizeof(num));
        for(int i=1;i<=m;i++)
        {
            cin>>x;
            if(!num[x])
              cnt[++k]=x;
            num[x]++;
        }
        l[0]=r[k+1]=1;
        for(int i=1;i<=k;i++)
          l[i]=l[i-1]*(num[cnt[i]]+1)%(mod-1);
        for(int i=k;i>=1;i--)
          r[i]=r[i+1]*(num[cnt[i]]+1)%(mod-1);
        ll ans=1;
        for(int i=1;i<=k;i++)
        {
            ll p=num[cnt[i]];
            if(!p)
              continue;
            ll res=l[i-1]*r[i+1]%(mod-1);
            res=p*(p+1)/2%(mod-1)*res%(mod-1);
            ans=ans*quick_mod(cnt[i],res)%mod;
        }
        cout<<ans<<endl;
    }
    return 0;
}
```
