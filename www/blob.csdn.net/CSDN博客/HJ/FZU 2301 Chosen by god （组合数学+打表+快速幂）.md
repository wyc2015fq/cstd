# FZU  2301 Chosen by god （组合数学+打表+快速幂） - HJ - CSDN博客
2018年08月08日 10:33:13[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：163
[http://acm.fzu.edu.cn/problem.php?pid=2301](http://acm.fzu.edu.cn/problem.php?pid=2301)
题目大意：
有n次攻击回合，每个回合的攻击有一点伤害，但是可能攻击到敌方无限血的英雄，也可能攻击到敌方的一个血量为m的小兵，问在n次攻击回合内把敌方小兵杀死的概率为多少？答案对1e9+7取膜。
解题思路：
敌方小兵血量为m，那么至少要有C(n,m)的方法攻击或者以上才能将小兵杀死，因此分子的值为C(n,m)+C(n,m+1)+…+C(n,n)，而分母则为攻击所有可能的方案总数，每次攻击有两种方法，且每次攻击相互独立，那么总数就为2^n。
数据稍大，提前打表即可。
```cpp
#include<iostream>
using namespace std;
typedef long long ll;
#define mod 1000000007
const int maxn=1e3;
ll res[maxn+10],C[maxn+10][maxn+10];
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
void Init()
{
    ll cnt=2;
    for(int i=1;i<=maxn;i++)
    {
        res[i]=quick_mod(cnt,mod-2);
        cnt=cnt*2%mod;
    }
    C[0][1]=C[1][0]=C[1][1]=1;
    for(int i=1;i<=maxn;i++)
    {
        C[i][0]=C[i][i]=1;
        for(int j=1;j<=i;j++)
          C[i][j]=(C[i-1][j]+C[i-1][j-1])%mod;        
    }
    for(int i=1;i<=maxn;i++)
      for(int j=1;j<=i;j++)
        C[i][j]=(C[i][j]+C[i][j-1])%mod;
}
int main()
{
    Init();
    int t,n,m;
    cin>>t;
    while(t--)
    {
        cin>>n>>m;
        ll ans=(m==0)?1:(n<m)?0:(ll)(C[n][n-m]*res[n])%mod;
        cout<<ans<<endl;
    }
    return 0;
}
```
