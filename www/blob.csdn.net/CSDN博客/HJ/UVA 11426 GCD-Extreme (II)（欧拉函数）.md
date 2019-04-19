# UVA  11426  GCD - Extreme (II)（欧拉函数） - HJ - CSDN博客
2017年01月17日 02:24:23[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：386
题目链接：[https://vjudge.net/contest/147453#problem/L](https://vjudge.net/contest/147453#problem/L)
题目大意：给定一个数，要求找出所有互为素数对的个数和。
解法： 
    直接暴力会超时。解决这个题目用到的方法是累加，比如先找到（？，j）的素数对个数为m，然后对于每一次存在正整数k，满足（？，j*k）的素数对个数为m*k。因此，先遍历一遍j从2到n，然后再暴力搜k，开一个数组a[]，用来处理a[j]，那么其实最后只需要求和a[2]+a[3]+…+a[n].
```cpp
#include<iostream>
#include<cstring>
using namespace std;
const int maxn=4000001;
const int N=4000010; 
int phi[N];
long long a[N];
void Phi()              //欧拉互素函数打表 
{
    memset(a,0,sizeof(a));
    for(int i=1;i<=maxn;i++)
      phi[i]=i;
    for(int i=2;i<=maxn;i++)
    {
        if(phi[i]==i)
          for(int j=i;j<=maxn;j+=i)
            phi[j]=phi[j]/i*(i-1); 
        for(int j=1;j*i<=maxn;j++) 
          a[j*i]+=j*phi[i];           //更新a[j*i]的数据 
    }
    for(int i=1;i<=maxn;i++)
      a[i]+=a[i-1];
} 
int main()
{
    Phi();
    int n;
    while(cin>>n&&n)
      cout<<a[n]<<endl;
    return 0;
}
```
