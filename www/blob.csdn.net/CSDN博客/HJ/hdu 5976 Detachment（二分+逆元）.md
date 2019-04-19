# hdu 5976 Detachment（二分+逆元） - HJ - CSDN博客
2018年08月15日 15:44:36[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：54
个人分类：[====ACM====																[排列组合																[----------数学----------																[---------OJ---------																[HDU](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6130298)](https://blog.csdn.net/feizaoSYUACM/article/category/6308090)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.hdu.edu.cn/showproblem.php?pid=5976](http://acm.hdu.edu.cn/showproblem.php?pid=5976)
解题思路：
首先需要判断怎么分割才能保证乘积最大？对于连续数，我们从2开始
2+3 
2+3+4 
2+3+4+5 
…. 
2+3+4+….+m  
其中m满足 2*m+(m-1)*m/2 >= 1e9 大概是1e5的样子
对于一个数n，如果不能恰好等于从2开始一直到k的连续数之和，那么就找到这个最大的k，满足 2+3+..+k<=n ，剩下的余数，就从k往前每个数都累加一次，一直到余数用完。其中，必然有一个数是空着的。比如当n=12的时候，k为4 （即2+3+4=9），余数为3，那么就有(3+4+5=12) 其中2便是空着的没有算进去，因此需要除去，即4!/2。
这样，我们便可以预处理好所有连续数的和，已经每个数的阶乘。对于每个输入的n，找到最大的k，通过求和取余找到那个需要除去的数，进行费马小定理求逆元，利用快速幂，即可求出答案。
```
#include<iostream>
#include<algorithm>
#include<map>
#include<vector>
#include<set>
#include<cstring>
#include<cstdio>
#include<cmath>
using namespace std;
#define mod 1000000007
typedef long long ll;
const int maxn=1e5+10;
ll sum[maxn+10],p[maxn+10];
ll quick_mod(ll x,ll n)
{
    ll res=1;
    while(n)
    {
        if(n%2)
          res=res*x%mod;
        x=x*x%mod;
        n/=2;
    }
    return res;
}
void Init()
{
    memset(sum,0,sizeof(sum));
    sum[3]=5;
    for(int i=4;i<=maxn;i++)
      sum[i]=sum[i-1]+i;
    p[1]=1;
    for(int i=2;i<=maxn;i++)
      p[i]=p[i-1]*i%mod;
}
int main()
{
    Init();
    int t;
    ll n;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%lld",&n);
        if(n<5)
        {
            printf("%lld\n",n);
            continue;
        }
        int k=upper_bound(sum,sum+maxn,n)-sum;
        ll m=n-sum[k-1],ans;
        if(m==k-1)
          ans=p[k+1]*quick_mod(2,mod-2)%mod*quick_mod(m+1,mod-2)%mod;
        else 
          ans=p[k]*quick_mod(k-m,mod-2)%mod;
        printf("%lld\n",ans);
    }
    return 0;
}
```
