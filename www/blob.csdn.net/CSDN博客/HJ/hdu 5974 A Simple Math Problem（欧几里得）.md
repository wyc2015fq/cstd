# hdu  5974 A Simple Math Problem（欧几里得） - HJ - CSDN博客
2017年10月27日 13:55:33[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：172
[http://acm.hdu.edu.cn/showproblem.php?pid=5974](http://acm.hdu.edu.cn/showproblem.php?pid=5974)
题目大意： 
x+y=a，lcm(x,y)=b。给出a和b，如果x和y有解，求出x和y。
解题思路：
```
gcd(x,y) * k1 = x
gcd(x,y) * k2 = y
=> gcd(x,y) * k1 * k2 = b
=> gcd(x,y) * k1 + gcd(x,y) * k2 = a
=> k1 * k2 = b / gcd(x,y)
=> k1 + k2 = a / gcd(x,y)
```
因为k1与k2 互质！ 
所以k1 * k2 和 k1 + k2 也一定互质 
所以 
```
gcd(x,y) == gcd(a,b)
```
因此可以知道x + y 与 x * y，解一元二次方程即可！
```
#include<iostream>
#include<cmath>
#include<cstdio>
using namespace std;
int gcd(int a,int b)
{
    return a%b==0?b:gcd(b,a%b);
}
bool check(int x)
{
    int k=sqrt(x);
    return k*k==x;
} 
int main()
{
    int a,b;
    while(cin>>a>>b)
    {
        int c=gcd(a,b)*b,d=a*a-4*c;
        if(check(d)&&((a&1)==(d&1)))
        {
            printf("%d %d\n",(int)(a-sqrt(d))/2,(int)(a+sqrt(d))/2);
        }
        else
          printf("No Solution\n");
    }
    return 0;
}
```
