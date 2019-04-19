# codeforces-678-C. Joty and Chocolate（数学） - HJ - CSDN博客
2017年02月02日 01:44:45[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：582
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
Little Joty has got a task to do. She has a line of n tiles indexed from 1 to n. She has to paint them in a strange pattern.
An unpainted tile should be painted Red if it’s index is divisible by a and an unpainted tile should be painted Blue if it’s index is divisible by b. So the tile with the number divisible by a and b can be either painted Red or Blue.
After her painting is done, she will get p chocolates for each tile that is painted Red and q chocolates for each tile that is painted Blue.
Note that she can paint tiles in any order she wants.
Given the required information, find the maximum number of chocolates Joty can get. 
Input
The only line contains five integers n, a, b, p and q (1 ≤ n, a, b, p, q ≤ 109). 
Output
Print the only integer s — the maximum number of chocolates Joty can get.
Note that the answer can be too large, so you should use 64-bit integer type to store it. In C++ you can use the long long integer type and in Java you can use long integer type. 
Examples 
Input
5 2 3 12 15
Output
39
Input
20 2 3 3 5
Output
51
题目大意：
现在有标号从1到n的没有涂色的气球，有红色和绿色两种颜色可以涂上去，但是都规定着一定距离涂一个，并且有花费，要求如何满足最后红色和绿色两种颜色最大化涂上去的前提下花费最小。
解题思路：
求最小公倍数，比较两种花费，如果遇到这个气球两种颜色都可以涂，选择花费小的。
```cpp
#include<bits/stdc++.h>
using namespace std;
long long gcd(long long a,long long b)
{
    return a%b==0?b:gcd(b,a%b); 
}
int main()
{
    long long n,p,q,a,b;
    while(cin>>n>>a>>b>>p>>q)
    {
        long long lcm=a*b/gcd(a,b);
        long long x=n/a,y=n/b,z=n/lcm;
        long long sum=x*p+y*q;
        if(z)
          sum=(p>q)?sum-z*q:sum-z*p;
        cout<<sum<<endl;
    }
    return 0;
}
```
