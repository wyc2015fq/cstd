# codeforces  735-D. Taxes（数学） - HJ - CSDN博客
2017年07月11日 13:20:53[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：235
个人分类：[====ACM====																[codeforce																[---------OJ---------](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
Mr. Funt now lives in a country with a very specific tax laws. The total income of mr. Funt during this year is equal to n (n ≥ 2) burles and the amount of tax he has to pay is calculated as the maximum divisor of n (not equal to n, of course). For example, if n = 6 then Funt has to pay 3 burles, while for n = 25 he needs to pay 5 and if n = 2 he pays only 1 burle.
As mr. Funt is a very opportunistic person he wants to cheat a bit. In particular, he wants to split the initial n in several parts n1 + n2 + … + nk = n (here k is arbitrary, even k = 1 is allowed) and pay the taxes for each part separately. He can’t make some part equal to 1 because it will reveal him. So, the condition ni ≥ 2 should hold for all i from 1 to k.
Ostap Bender wonders, how many money Funt has to pay (i.e. minimal) if he chooses and optimal way to split n in parts. 
Input
The first line of the input contains a single integer n (2 ≤ n ≤ 2·109) — the total year income of mr. Funt. 
Output
Print one integer — minimum possible number of burles that mr. Funt has to pay as a tax. 
Examples 
Input
4
Output
2
Input
27
Output
3
题目大意： 
某人要交税，交的税钱是收入n的最大因子（注意最大因子不是n，但是若该数是质数则是1）。但是这人作弊，把钱拆成几份，使交税最少，请输出这个人最少要交多少税钱。
解题思路： 
如果n为素数，答案就是1，如果n-2为素数，答案就是2，否则如果n为偶数，那么任意一个偶数都可以表示成两个素数和，任意一个奇数都可以表示成三个素数和。
```cpp
#include<iostream>
using namespace std;
int isprime(int x)
{
    for(int i=2;i*i<=x;i++)
      if(x%i==0)
        return 0;
    return 1;
}
int main()
{
    int n,ans;
    while(cin>>n)
    {
        if(isprime(n))
          ans=1;
        else if(isprime(n-2))
          ans=2;
        else
          ans=2+n%2;
        cout<<ans<<endl;
    }
    return 0;
}
```
