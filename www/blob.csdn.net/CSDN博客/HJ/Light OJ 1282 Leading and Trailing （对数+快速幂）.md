# Light OJ 1282 Leading and Trailing （对数+快速幂） - HJ - CSDN博客
2017年02月02日 01:31:23[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：247
```
You are given two integers: n and k, your task is to find the most significant three digits, and least significant three digits of nk.
```
Input
```
Input starts with an integer T (≤ 1000), denoting the number of test cases.
Each case starts with a line containing two integers: n (2 ≤ n < 231) and k (1 ≤ k ≤ 107).
```
Output
```
For each case, print the case number and the three leading digits (most significant) and three trailing digits (least significant). You can assume that the input is given such that nk contains at least six digits.
```
Sample Input
```
5
123456 1
123456 2
2 31
2 32
29 8751919
```
Sample Output
```
Case 1: 123 456
Case 2: 152 936
Case 3: 214 648
Case 4: 429 296
Case 5: 665 669
```
题目大意： 
求出一个数的n的k次方结果的前三位和后三位
解题思路： 
（1）后三位用快速幂取一下就好了
（2）前三位利用对数，公式如下
```
n^k=a.bc*10^m ( m为n^k的位数，即m=(int)lg(n^k)=(int)(k*lgn) );
求对数:  k*lgn=lg(a.bc)+m
即 a.bc=10^(k*lgn-m)=10^(k*lgn-(int)(k*lgn));
abc=a.bc*100;
```
代码如下：
```cpp
#include<bits/stdc++.h>
using namespace std;
#define mod 1000
long long quick_mod(long long a,long long n)   //快速幂取模结果的求后三位 
{
    long long res=1;
    while(n)
    {
        if(n%2)
          res=res*a%mod;
        n>>=1;
        a=a*a%mod;
    }
    return res;
}
int main()
{
    long long ans,tt=0,t,n,m;
    cin>>t;
    while(t--)
    {
        cin>>n>>m;
        double c=m*log10(n)-(long long)(m*log10(n));     //去对数的小数部分 并用科学计数法表示 
        ans=pow(10,c+2);                             //扩大100倍 
        printf("Case %lld: %lld %03lld\n", ++tt,ans,quick_mod(n,m));  
    }
    return 0;
}
```
