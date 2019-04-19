# hdu  5690 All X（快速幂） - HJ - CSDN博客
2017年08月23日 16:47:45[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：172
[http://acm.hdu.edu.cn/showproblem.php?pid=5690](http://acm.hdu.edu.cn/showproblem.php?pid=5690)
公式推导一下：
```
（10^m-1）/9*x%mod ==c
```
```
#include<iostream>
#include<cstdio> 
using namespace std;
typedef long long LL;
LL quick_mod(LL a,LL b,LL mod)
{
    LL ans=1;
    while(b)
    {
        if(b%2)
          ans=ans*a%mod;
        a=a*a%mod;
        b/=2;
    }
    return ans;
}
int main()
{
    int t,x,k,c,tt=0;
    LL m;
    cin>>t;
    while(t--)
    {
        cin>>x>>m>>k>>c;
        LL mod=9*k,ans=quick_mod(10,m,mod);
        printf("Case #%d:\n",++tt);
        if((ans-1)*x%mod==9*c)
          printf("Yes\n");
        else
          printf("No\n");
    }
    return 0;
}
```
