# FZU  2282 Wand（组合） - HJ - CSDN博客
2017年08月23日 16:33:21[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：244
[http://acm.fzu.edu.cn/problem.php?pid=2282](http://acm.fzu.edu.cn/problem.php?pid=2282)
题目大意： 
1~n个数中至少有k个数在对应下标的k个位置，其他的数都为错排。问有多少种排列方式？
解题思路： 
n个数的错排公式
```
D(n)=(n-1) * ( D(n-1) + D(n-2) )
```
然后是从n个数中选取n-i个数进行错排（k<=i<=n），就需要用到递推公式 
和组合公式。考虑到会超时，用快速幂和预处理打表处理。
```
#include<iostream>
#include<cstdio>
using namespace std;
#define mod 1000000007
typedef long long LL;
LL d[10005],a[10005],b[10005];
LL quickmod(LL x,LL y)    //快速幂
{
    LL ans=1;
    while(y)
    {
        if(y&1)
          ans=ans*x%mod;
        x=x*x%mod;
        y>>=1;
    }
    return ans%mod;
}
void Init()            //预处理打表 
{
    a[0]=b[0]=1;
    for(int i=1;i<=10000;i++)
    {
        a[i]=a[i-1]*i%mod;
        b[i]=quickmod(a[i],mod-2);          //除法求逆元 
    }
}
int main()
{
    d[1]=0,d[2]=1;
    for(int i=3;i<=10000;i++)
      d[i]=(i-1)*((d[i-1]+d[i-2])%mod)%mod;         //错排公式 D(n)=(n-1) * ( D(n-1) + D(n-2) ) 
    Init();
    int t,n,k;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d%d",&n,&k); 
        int cnt=n-k;
        LL ans=1;
        for(int i=2;i<=cnt;i++)
          ans=(ans+(a[n]*b[i])%mod*b[n-i]%mod*d[i]%mod)%mod;     
        printf("%lld\n",ans);
    }
    return 0;
}
```
