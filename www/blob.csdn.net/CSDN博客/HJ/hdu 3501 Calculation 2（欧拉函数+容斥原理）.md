# hdu  3501 Calculation 2（欧拉函数+容斥原理） - HJ - CSDN博客
2017年08月04日 21:32:48[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：174
Problem Description 
Given a positive integer N, your task is to calculate the sum of the positive integers less than N which are not coprime to N. A is said to be coprime to B if A, B share no common positive divisors except 1.
Input 
For each test case, there is a line containing a positive integer N(1 ≤ N ≤ 1000000000). A line containing a single 0 follows the last test case.
Output 
For each test case, you should print the sum module 1000000007 in a line.
Sample Input
3 
4 
0
Sample Output
0 
2
phi(n)表示比n小且与n互素的整数的个数，而phi(n)*n/2表示比n小且与n互素的整数的和。
因此算一下前n个数的和，然后用容斥原理就行了。
```cpp
#include<iostream>
#include<cmath> 
using namespace std;
typedef long long LL;
#define mod 1000000007
LL euler_phi(LL n)         //欧拉函数 
{
    LL m=sqrt(n+0.5),ans=n,i=2;
    for(;i<=m;i++)
      if(n%i==0)
      {
          ans=ans/i*(i-1);
          while(n%i==0)
            n/=i;
      }
    if(n>1)
      ans=ans/n*(n-1);
    return ans; 
} 
int main()
{
    LL n;
    while(cin>>n&&n)
    {
        LL ans=n*(n+1)/2-n-euler_phi(n)*n/2;         //容斥原理 
        cout<<ans%mod<<endl;
    }
    return 0;
}
```
