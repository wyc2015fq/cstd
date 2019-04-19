# FZU 1649 Prime number or not （大素数判定） - HJ - CSDN博客
2017年08月04日 21:39:02[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：395
个人分类：[====ACM====																[----------数学----------																[初等数论																[---------OJ---------																[杂题](https://blog.csdn.net/feizaoSYUACM/article/category/6684121)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308034)](https://blog.csdn.net/feizaoSYUACM/article/category/6130298)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Your task is simple.  
    Give you a number N, you should judge whether N is a prime number or not. 
Input 
    There are multiple test cases. For each test case, there is an integer N(2<=N<=10^18). 
Output 
    For each test case, you should output whether N is a prime number or not.  
    If N is a prime number , you should output “It is a prime number.”; otherwise you should output “It is not a prime number.”;  
Sample Input 
    2 
    4
Sample Output 
    It is a prime number. 
    It is not a prime number.
大素数判定：Miller Rabbin
费马小定理：
```
a为整数，n是素数，且a，n互质，则有a^(n-1)≡1(mod n) ，即：a^(n-1)模n得1。
```
快速判定一个数是否为素数的方法：
如果存在一个整数a，使得a^(n-1)≡1(mod n) ，则称n为基于a的伪素数，当有多个满足关系的a时，则n为素数的概率趋向于1。所以取多个a测试一下即可。
```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<ctime>
using namespace std;
typedef __int64 LL;
#define maxn 50
LL sum(LL a,LL b,LL mod)         //快速乘 
{
    LL ans=0;
    while(b)
    {
        if(b&1) 
          ans=(ans+a)%mod;
        a=a*2%mod;
        b/=2; 
    }
    return ans;
}
LL pow(LL a,LL b,LL mod)       //快速幂 
{
    LL ans=1;
    while(b)
    {
        if(b&1)
          ans=sum(ans,a,mod);
         a=sum(a,a,mod);
         b/=2; 
    }
    return ans;
}
int Miller_Rabbin(LL x)
{
    if(x==2)
      return 1;
    if(x<2||!(x&1))
      return 0;
    srand((unsigned)time(NULL));          //随机数 
    for(int i=1;i<=maxn;i++)
    {
        LL a=rand()%(x-2)+1;
        if(pow(a,x-1,x)!=1)       // a^(n-1) % n == 1 
          return 0;
    }
    return 1;
} 
int main()
{
    LL n;
    while(cin>>n)
    {
        if(Miller_Rabbin(n))
          printf("It is a prime number.\n");
        else
          printf("It is not a prime number.\n");
    }
    return 0;
}
```
